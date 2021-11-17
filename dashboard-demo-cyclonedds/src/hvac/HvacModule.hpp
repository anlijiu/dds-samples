#ifndef __HVAC_MODULE_HXX__
#define __HVAC_MODULE_HXX__


#include <cstdint>
#include <unordered_map>
#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>
#include "hvac_msgs/msg/Ac.hpp"
#include "hvac_msgs/srv/SetAc.hpp"
#include "std_msgs/srv/Response.hpp"
#include "AbsModule.hpp"
#include "TimeoutTrigger.hpp"
#include "CanMessageDistributor.hpp"
// #include "VehicleUtils.hpp"

namespace dashboard {
namespace dds {
namespace hvac {

class HvacModule : public dashboard::dds::AbsModule {
    using ParseFrame = std::function<void(const uint8_t * data)>;
    using ParseMapFromCan = std::unordered_map<uint32_t, ParseFrame>;

public:

    HvacModule(
        TimeoutTrigger* timer,
        int domain_id = 0,
        std::string qos_profile_file = "",
        std::string qos_profile = ""
    );
    virtual ~HvacModule();

    uint8_t moduleId();

    void parseCan0x334(const uint8_t * data);
    void parseAcFromCAN(const uint8_t * data);

    std_msgs::srv::Response setAcFromDdsRequest(hvac_msgs::srv::SetAcRequest request);

    const ParseMapFromCan & getParseMapFromCan();
private:
    void parseAcGlobalFromCan(const uint8_t * data);

private:
    ::dds::domain::DomainParticipant mParticipant;
    ::dds::topic::Topic<hvac_msgs::msg::Ac> mTopicAc;
    ::dds::pub::DataWriter<hvac_msgs::msg::Ac> mAcWriter;

    hvac_msgs::msg::Ac mGlobalAc;//

    uint16_t mModuleId = 0x01;
    uint16_t mFunctionIdAc = 0x02;

    uint32_t mPropAc = 0x00010002;//static_cast<uint32_t>(moduleId) << 16 | static_cast<uint32_t>(0x02);
    std::function<void(bool)> mReplyAcFunc;

    ParseMapFromCan mParseMapFromCan;
    TimeoutTrigger* mTimer;
};

}//namespace hvac
}//namespace dds
}//namespace dashboard
#endif //__HVAC_MODULE_HXX__
