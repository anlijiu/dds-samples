#ifndef __SEAT_MODULE_HPP__
#define __SEAT_MODULE_HPP__

#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>
#include <dds/core/cond/WaitSet.hpp>

#include <std_msgs/Response.hpp>
#include "seat_msgs/Position.hpp"
#include <seat_msgs/SetPosition.hpp>

#include "topic_subscriber.hpp"
#include "vhal_compatible.hpp"
#include "parallel_hashmap/phmap.h"
#include "AbsModule.hpp"
#include "AsyncWaitSet.hpp"

namespace headunit {
namespace dds {
namespace seat {

using WritePropValue = std::function<StatusCode(const VehiclePropValue& propValue)>;

// class SeatModule : public headunit::dds::AbsModule {
class SeatModule : public AbsModule {
public:
    using WritePropValueMap = phmap::flat_hash_map<int32_t, WritePropValue>;
public:
    SeatModule (
        AsyncWaitSet& async_waitset,
        std::function<void(VehiclePropValuePtr)> func,
        VehiclePropValuePool* pool,
        int domain_id = 0);

    virtual ~SeatModule ();

    void process_samples_position(std::function<void(VehiclePropValuePtr)> func);
    VehiclePropValuePtr properties_from_position(const seat_msgs::msg::Position& position);
    StatusCode set_position(const VehiclePropValue& propValue);
    const phmap::flat_hash_map<int32_t, WritePropValue>& getWritePropValueMap();
private:
    ::dds::domain::DomainParticipant mParticipant;
    ::dds::topic::Topic<seat_msgs::msg::Position> mTopicPosition;
    ::dds::sub::DataReader<seat_msgs::msg::Position> mReaderPosition;

    AsyncWaitSet& mAsyncWaitSet;
    VehiclePropValuePool* mValueObjectPoolPtr;
    WritePropValueMap mWritePropValueMap;
};

} // seat
} // dds
} // headunit

#endif // ifndef __SEAT_MODULE_HPP__
