#include "SeatModule.hpp"

#include <iostream>
#include <thread>
#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>

#include "vhal_compatible.hpp"
#include "DdsUtils.hpp"
#include "AsyncWaitSet.hpp"

namespace headunit {
namespace dds {
namespace seat {

SeatModule::SeatModule(
    AsyncWaitSet& async_waitset,
    std::function<void(VehiclePropValuePtr)> func,
    VehiclePropValuePool* pool,
    int domain_id
) : mReaderPosition(::dds::core::null),
    mParticipant(::dds::core::null),
    mTopicPosition(::dds::core::null),
    mAsyncWaitSet(async_waitset),
    mValueObjectPoolPtr(pool),
    mWritePropValueMap({
        { toInt(VehicleProperty::SEAT_HEIGHT_POS), std::bind(&SeatModule::set_position, this, std::placeholders::_1) }
    })
{

    std::cout << __func__ << std::endl;

    utils::find_or_create_domain_participant(
              domain_id, mParticipant);

    utils::find_or_create_topic<seat_msgs::msg::Position>(
            mParticipant, mTopicPosition, seat_msgs::msg::TOPIC_SEAT_POSITION);

    ::dds::sub::qos::SubscriberQos positionSubscriberQos = mParticipant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos positionDataReaderQos(mTopicPosition.qos());

    utils::create_datareader<seat_msgs::msg::Position>(
            mReaderPosition,
            mParticipant,
            mTopicPosition,
            positionDataReaderQos,
            positionSubscriberQos);

    // DataReader status condition: to process the reception of samples
    ::dds::core::cond::StatusCondition positionReadStatusCondition(mReaderPosition);
    positionReadStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    positionReadStatusCondition.handler(std::bind(&SeatModule::process_samples_position, this, func));
    mAsyncWaitSet.attach_condition(positionReadStatusCondition);
}

SeatModule::~SeatModule()
{
    std::cout << __func__ << std::endl;
    mAsyncWaitSet.detach_condition(
            ::dds::core::cond::StatusCondition(mReaderPosition));
}

void SeatModule::process_samples_position(std::function<void(VehiclePropValuePtr)> func)
{

    std::cout << " SeatModule::process_samples_position thread id: " << std::this_thread::get_id() << std::endl;
    std::cout << __func__ << std::endl;
    // Take all samples This will reset the StatusCondition
    ::dds::sub::LoanedSamples<seat_msgs::msg::Position> samples = mReaderPosition.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    // Release status condition in case other threads can process outstanding
    // samples
    // mAsyncWaitSet.unlock_condition(
    //         ::dds::core::cond::StatusCondition(mReaderPosition));

    // Process sample
    for (::dds::sub::LoanedSamples<seat_msgs::msg::Position>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            // std::cout << "Received sample:\n\t" << sample_it->data()
            //           << std::endl;
            func(properties_from_position(sample_it->data()));
        }
    }
}

VehiclePropValuePtr SeatModule::properties_from_position(const seat_msgs::msg::Position& position) {
    auto v = mValueObjectPoolPtr->obtain(VehiclePropertyType::INT32, 2);
    v->prop = static_cast<int32_t>(VehicleProperty::SEAT_HEIGHT_POS);
    v->value.int32Values.resize(1);
    v->value.int32Values[0] = position.angle();
    v->areaId = position.id().zone();
    v->timestamp = elapsedRealtimeNano();
    return v;
}

const phmap::flat_hash_map<int32_t, WritePropValue>& SeatModule::getWritePropValueMap() {
    return mWritePropValueMap;
}

StatusCode SeatModule::set_position(const VehiclePropValue& propValue) {
    std::cout << " SeatModule::set_position thread id: " << std::this_thread::get_id() << std::endl;

    std::cout << "set_position in" << propValue.value.int32Values[0] <<  std::endl;
    // auto request_id =
    //         mSetPositionRequester.send_request(seat_msgs::srv::SetPositionRequest(propValue.value.int32Values[0], propValue.areaId));

    // Receive replies
    const auto MAX_WAIT = ::dds::core::Duration::from_secs(20);
    return StatusCode::OK;

}

} // seat
} // dds
} // headunit
