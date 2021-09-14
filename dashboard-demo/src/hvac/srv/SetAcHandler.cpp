#include "SetAcHandler.hpp"

#include <rti/request/Replier.hpp>
#include <rti/request/rtirequest.hpp> // full request-reply API
#include <rti/config/Logger.hpp> // Logger to configure logging verbosity
#include <cmath>
 
SetAcHandler::SetAcHandler(int domainId) :
    participant(domainId),
    replier(dds::core::null)
{

    auto qos_provider = dds::core::QosProvider::Default();
    rti::request::ReplierParams replier_params(participant);
    replier_params.service_name("HvacAcOperator");
    replier_params.datawriter_qos(qos_provider.datawriter_qos(
            "DashboardProfiles::ReplierProfile"));
    replier_params.datareader_qos(qos_provider.datareader_qos(
            "DashboardProfiles::ReplierProfile"));

    replier = rti::request::Replier<hvac_msgs::srv::SetAcRequest, std_msgs::srv::Response>(
                replier_params);
}
 
 
SetAcHandler::~SetAcHandler()
{
//dtor 
}

