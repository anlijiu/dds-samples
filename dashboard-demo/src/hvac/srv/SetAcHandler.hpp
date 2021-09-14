#ifndef __SETACHANDLER_H__
#define __SETACHANDLER_H__

#include <dds/core/ddscore.hpp>
#include <rti/request/Replier.hpp>
#include "std_msgs/srv/Response.hpp"
#include "hvac_msgs/srv/SetAc.hpp"

class SetAcHandler {
public:
    SetAcHandler(int domainId);
    virtual ~SetAcHandler();
protected:
private:

    dds::domain::DomainParticipant participant;
    rti::request::Replier<hvac_msgs::srv::SetAcRequest, std_msgs::srv::Response> replier;
};
#endif // __SETACHANDLER_H__


