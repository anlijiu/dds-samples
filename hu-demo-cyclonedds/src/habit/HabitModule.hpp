#ifndef __HABIT_MODULE_HPP__
#define __HABIT_MODULE_HPP__

#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>
#include <dds/core/cond/WaitSet.hpp>

#include <std_msgs/Response.hpp>
#include <habit_msgs/AppointmentDrive.hpp>

#include "topic_subscriber.hpp"
#include "vhal_compatible.hpp"
#include "parallel_hashmap/phmap.h"
#include "AbsModule.hpp"
#include "AsyncWaitSet.hpp"

namespace headunit {
namespace dds {
namespace habit {

using WritePropValue = std::function<StatusCode(const VehiclePropValue& propValue)>;

// class HabitModule : public headunit::dds::AbsModule {
class HabitModule : public AbsModule {
public:
    using WritePropValueMap = phmap::flat_hash_map<int32_t, WritePropValue>;
public:
    HabitModule (
        AsyncWaitSet& async_waitset,
        std::function<void(VehiclePropValuePtr)> func,
        VehiclePropValuePool* pool,
        int domain_id = 0);

    virtual ~HabitModule ();

    StatusCode appointDrive(const VehiclePropValue& propValue);

    const phmap::flat_hash_map<int32_t, WritePropValue>& getWritePropValueMap();
private:
    ::dds::domain::DomainParticipant mParticipant;
    ::dds::topic::Topic<habit_msgs::srv::AppointmentDriveRequest> mAppointmentDriveRequestTopic;
    ::dds::pub::DataWriter<habit_msgs::srv::AppointmentDriveRequest> mAppointmentDriveRequestWriter;

    AsyncWaitSet& mAsyncWaitSet;
    WritePropValueMap mWritePropValueMap;
};

} // habit
} // dds
} // headunit

#endif // ifndef __HABIT_MODULE_HPP__
