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
        TimeoutTrigger* timer,
        int domain_id,
        std::string qos_profile_file,
        std::string qos_profile
) : mTimer(timer),
    mParticipant(::dds::core::null),
    mTopicPosition(::dds::core::null),
    mPositionWriter(::dds::core::null),
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
}


SeatModule::~SeatModule() {
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
    bool on = static_cast<bool>(value);

    // std::cout << __func__ << " value:" << value << " on:" << on << " orig on:" << mGlobalPosition.on();
    mGlobalPosition.zone(0);
    if(on != mGlobalPosition.on()) {
        // if(need) {
        //     replier.send_reply(error_reply, request.info());
        //     timer.unregisterEvent(ac);
        // }
        mGlobalPosition.on(on);

        auto propzone = (static_cast<uint64_t>(0) << 32) | static_cast<uint64_t>(mPropPosition);//getPropZone(mPropPosition, request.zone());
        if(mReplyPositionFunc) {
            mReplyPositionFunc(on);
            mReplyPositionFunc = nullptr;
        }
    }

    std::cout << "SeatModule::parsePositionFromCAN before write" << std::endl;
    mPositionWriter->write(mGlobalPosition);
    std::cout << "SeatModule::parsePositionFromCAN after write" << std::endl;
}

std_msgs::srv::Response SeatModule::setPositionFromDdsRequest(seat_msgs::srv::SetPositionRequest request)
{
    std::cout << " SeatModule::setPositionFromDdsRequest thread id: " << std::this_thread::get_id() << std::endl;

    bool on = request.on();
    std::cout << __func__ << " in, position: " << on << std::endl;

    //下发CAN ，加入timeout map ， 加入检测CAN 对应数据， 然后看哪个先reply
    //功能对应到 信号集合， 大概率为同一ID下面的1个或多个信号
    //小概率为多个ID多个信号
    //对于这个setPosition 就是   set position&zone&axle   <->  canid & start bit & length & offset
    //
    //void canMessageAdapter->write(uint32_t canid, uint8_t startbit, uint8_t len, uint64_t value);

}




}//namespace seat
}//namespace dds
}//namespace dashboard
