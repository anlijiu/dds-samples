#include "HabitModule.hpp"

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
namespace habit {

HabitModule::HabitModule(
    AsyncWaitSet& async_waitset,
    std::function<void(VehiclePropValuePtr)> func,
    VehiclePropValuePool* pool,
    int domain_id
) : mAppointmentDriveRequestWriter(::dds::core::null),
    mParticipant(::dds::core::null),
    mAppointmentDriveRequestTopic(::dds::core::null),
    mAsyncWaitSet(async_waitset),
    mWritePropValueMap({
        { toInt(VehicleProperty::APPOINTMENT_DRIVE), std::bind(&HabitModule::appointDrive, this, std::placeholders::_1) }
    })

{
    std::cout << __func__ << std::endl;

    utils::find_or_create_domain_participant(
              domain_id, mParticipant);

    utils::find_or_create_topic<habit_msgs::srv::AppointmentDriveRequest>(
            mParticipant, mAppointmentDriveRequestTopic, habit_msgs::srv::TOPIC_APPOINTMENT_DRIVE);

    ::dds::pub::qos::PublisherQos defaultPublisherQos = mParticipant.default_publisher_qos();
    ::dds::pub::qos::DataWriterQos dataWriterQos(mAppointmentDriveRequestTopic.qos());

    utils::create_datawriter<habit_msgs::srv::AppointmentDriveRequest>(
            mAppointmentDriveRequestWriter,
            mParticipant,
            mAppointmentDriveRequestTopic,
            dataWriterQos,
            defaultPublisherQos);
}

HabitModule::~HabitModule()
{
    std::cout << __func__ << std::endl;
}

const phmap::flat_hash_map<int32_t, WritePropValue>& HabitModule::getWritePropValueMap() {
    return mWritePropValueMap;
}

StatusCode HabitModule::appointDrive(const VehiclePropValue& propValue) {
    std::cout << " HabitModule::appointmentDrive thread id: " << std::this_thread::get_id() << std::endl;

    std::cout << "appointmentDrive timestamp: " << propValue.value.int64Values[0] <<  std::endl;
    // auto request_id =
    //         mSetAcRequester.send_request(habit_msgs::srv::SetAcRequest(propValue.value.int32Values[0], propValue.areaId));

    // Receive replies
    const auto MAX_WAIT = ::dds::core::Duration::from_secs(20);

    habit_msgs::srv::AppointmentDriveRequest request(propValue.value.int64Values[0]);
    mAppointmentDriveRequestWriter->write(request);
    return StatusCode::OK;
}

} // habit
} // dds
} // headunit
