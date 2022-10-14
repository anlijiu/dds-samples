#include <dds/core/ddscore.hpp>
#include <std_msgs/srv/Response.hpp>

#include "SeatModule.hpp"
#include "DdsUtils.hpp"

namespace dashboard {
namespace dds {
namespace seat {

using ParseFrame = std::function<void(const uint8_t * data)>;
using ParseMapFromCan = std::unordered_map<uint32_t, ParseFrame>;

SeatModule::SeatModule(
    AsyncWaitSet& async_waitset,
    TimeoutTrigger* timer,
    int domain_id,
    std::string qos_profile_file,
    std::string qos_profile
) : mAsyncWaitSet(async_waitset),
    mTimer(timer),
    mParticipant(::dds::core::null),
    mTopicPosition(::dds::core::null),
    mPositionWriter(::dds::core::null),
    mTopicSetHeat(::dds::core::null),
    mSetHeatReader(::dds::core::null),
    mSetHeatReadStatusCondition(::dds::core::null),
    mParseMapFromCan({
        { 0x334, std::bind(&dashboard::dds::seat::SeatModule::parseCan0x334, this,  std::placeholders::_1) }
    })
{
    utils::find_or_create_domain_participant(domain_id, mParticipant);

    utils::find_or_create_topic<seat_msgs::msg::Position>(
            mParticipant, mTopicPosition, seat_msgs::msg::TOPIC_SEAT_POSITION);

    ::dds::pub::qos::PublisherQos defaultPublisherQos = mParticipant.default_publisher_qos();
    ::dds::pub::qos::DataWriterQos positionDataWriterQos(mTopicPosition.qos());

    utils::create_datawriter<seat_msgs::msg::Position>(
            mPositionWriter,
            mParticipant,
            mTopicPosition,
            positionDataWriterQos,
            defaultPublisherQos);

    utils::find_or_create_topic<seat_msgs::msg::Heat>(
            mParticipant, mTopicSetHeat, seat_msgs::srv::TOPIC_SET_SEAT_HEAT);

    ::dds::sub::qos::SubscriberQos subscriberQos = mParticipant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos dataReaderQos(mTopicSetHeat.qos());

    utils::create_datareader<seat_msgs::msg::Heat>(
            mSetHeatReader,
            mParticipant,
            mTopicSetHeat,
            dataReaderQos,
            subscriberQos);

    mSetHeatReadStatusCondition = ::dds::core::cond::StatusCondition(mSetHeatReader);
    mSetHeatReadStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    mSetHeatReadStatusCondition.handler(std::bind(&SeatModule::handle_set_heat, this));
    mAsyncWaitSet.attach_condition(mSetHeatReadStatusCondition);
}


SeatModule::~SeatModule() {
}


void SeatModule::handle_set_heat()
{
    ::dds::sub::LoanedSamples<seat_msgs::msg::Heat> samples = mSetHeatReader.take();
    for (::dds::sub::LoanedSamples<seat_msgs::msg::Heat>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            const seat_msgs::msg::Heat& heatValue = sample_it->data();
            std::cout << "Received set heat request, level:" << heatValue.level()
                << ", zone:" << heatValue.zone() << std::endl;
        }
    }
}

void SeatModule::parseCan0x334(const uint8_t * data) {
    parsePositionFromCAN(data);
}

const std::unordered_map<uint32_t, ParseFrame>& SeatModule::getParseMapFromCan() {
    return mParseMapFromCan;
}

void SeatModule::parsePositionFromCAN(const uint8_t * data) {
    std::cout << "SeatModule::parsePositionFromCAN in " << data << std::endl;
    // module -> 多个CANID ， 本module内所有功能 -> canid->signals 
    //这个canid之内的信号与功能的对应关系， 例如 mGlobalPosition 对应的 startbit,length,scale,offset
    uint32_t startBit = 0;
    uint32_t len= 1;
    double scaling = 1;
    double offset = 0;

    uint64_t origvalue = ::extract(data, startBit, len, UNSIGNED, MOTOROLA);
    double value = origvalue * scaling + offset;
    int32_t angle = static_cast<int32_t>(value);

    if(angle != mGlobalPosition.angle()) {
        // if(need) {
        //     replier.send_reply(error_reply, request.info());
        //     timer.unregisterEvent(ac);
        // }
        mGlobalPosition.angle(angle);

        auto propzone = (static_cast<uint64_t>(0) << 32) | static_cast<uint64_t>(mPropPosition);//getPropZone(mPropPosition, request.zone());
        if(mReplyPositionFunc) {
            mReplyPositionFunc(angle);
            mReplyPositionFunc = nullptr;
        }
    }

    std::cout << "SeatModule::parsePositionFromCAN before write" << std::endl;
    mPositionWriter->write(mGlobalPosition);
    std::cout << "SeatModule::parsePositionFromCAN after write" << std::endl;
}



}//namespace seat
}//namespace dds
}//namespace dashboard
