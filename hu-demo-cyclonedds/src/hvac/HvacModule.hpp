#ifndef __HVAC_MODULE_HPP__
#define __HVAC_MODULE_HPP__

#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>

#include <std_msgs/srv/Response.hpp>
#include <hvac_msgs/srv/SetAc.hpp>

#include "topic_subscriber.hpp"
#include "hvac_msgs/msg/Ac.hpp"
#include "vhal_compatible.hpp"
#include "parallel_hashmap/phmap.h"
#include "AbsModule.hpp"
#include "request/Requester.hpp"
#include "AsyncWaitSet.hpp"

namespace headunit {
namespace dds {
namespace hvac {

using WritePropValue = std::function<StatusCode(const VehiclePropValue& propValue)>;

// class HvacModule : public headunit::dds::AbsModule {
class HvacModule : public AbsModule {
public:
    using WritePropValueMap = phmap::flat_hash_map<int32_t, WritePropValue>;
public:
    HvacModule (
        rti::core::cond::AsyncWaitSet async_waitset,
        std::function<void(VehiclePropValuePtr)> func,
        VehiclePropValuePool* pool,
        DDS_DomainId_t domain_id = 0);

    virtual ~HvacModule ();

    void process_samples_ac(std::function<void(VehiclePropValuePtr)> func);
    VehiclePropValuePtr properties_from_ac(const hvac_msgs::msg::Ac& ac);
    StatusCode set_ac(const VehiclePropValue& propValue);
    const phmap::flat_hash_map<int32_t, WritePropValue>& getWritePropValueMap();
private:
    ::dds::domain::DomainParticipant mParticipant;
    ::dds::topic::Topic<hvac_msgs::msg::Ac> mTopicAc;
    ::dds::sub::DataReader<hvac_msgs::msg::Ac> mReaderAc;
    request::Requester<hvac_msgs::srv::SetAcRequest, std_msgs::srv::Response> mSetAcRequester;

    const AsyncWaitSet& mAsyncWaitSet;
    VehiclePropValuePool* mValueObjectPoolPtr;
    WritePropValueMap mWritePropValueMap;
};
        
} // hvac
} // dds
} // headunit

#endif // ifndef __HVAC_MODULE_HPP__ 
