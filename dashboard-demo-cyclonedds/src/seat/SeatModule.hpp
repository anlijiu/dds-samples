#ifndef __SEAT_MODULE_HXX__
#define __SEAT_MODULE_HXX__


#include <cstdint>
#include <unordered_map>
#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>
#include "seat_msgs/msg/Position.hpp"
#include <seat_msgs/srv/SetPosition.hpp>
#include "std_msgs/srv/Response.hpp"
#include "AbsModule.hpp"
#include "TimeoutTrigger.hpp"
#include "CanMessageDistributor.hpp"
// #include "VehicleUtils.hpp"

namespace dashboard {
namespace dds {
namespace seat {

class SeatModule : public dashboard::dds::AbsModule {
    using ParseFrame = std::function<void(const uint8_t * data)>;
    using ParseMapFromCan = std::unordered_map<uint32_t, ParseFrame>;

public:

    SeatModule(
        TimeoutTrigger* timer,
        int domain_id = 0,
        std::string qos_profile_file = "",
        std::string qos_profile = ""
    );
    virtual ~SeatModule();

    uint8_t moduleId();

    void parseCan0x334(const uint8_t * data);
    void parsePositionFromCAN(const uint8_t * data);

    std_msgs::srv::Response setPositionFromDdsRequest(seat_msgs::srv::SetPositionRequest request);

    const ParseMapFromCan & getParseMapFromCan();
private:
    void parsePositionGlobalFromCan(const uint8_t * data);

private:
    ::dds::domain::DomainParticipant mParticipant;
    ::dds::topic::Topic<seat_msgs::msg::Position> mTopicPosition;
    ::dds::pub::DataWriter<seat_msgs::msg::Position> mPositionWriter;

    seat_msgs::msg::Position mGlobalPosition;//

    uint16_t mModuleId = 0x01;
    uint16_t mFunctionIdPosition = 0x02;

    uint32_t mPropPosition  = 0x00010002;//static_cast<uint32_t>(moduleId) << 16 | static_cast<uint32_t>(0x02);
    std::function<void(bool)> mReplyPositionFunc;

    ParseMapFromCan mParseMapFromCan;
    TimeoutTrigger* mTimer;
};

}//namespace seat
}//namespace dds
}//namespace dashboard
#endif //__SEAT_MODULE_HXX__
