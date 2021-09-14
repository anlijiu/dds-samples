#include "DataHub.hpp"
#include "CanMessageDistributor.hpp"

namespace dashboard {
namespace dds {

DataHub::DataHub()
    : mTimeoutTrigger(nullptr)
    , mCanMessageDistributor(nullptr)
    , mHvacModule(nullptr)
{

    std::vector<std::string> qosFileNames;
    std::cout<< "DDSMessageAdapter init " << std::endl;

    qosFileNames.push_back("file:///home/anlijiu/disk/workspace/dds/dds-samples/hu-demo/base_qos_profiles.xml");
    qosFileNames.push_back("file:///home/anlijiu/disk/workspace/dds/dds-samples/hu-demo/lowlatency_sensor_qos_profiles.xml");
    qosFileNames.push_back("file:///home/anlijiu/disk/workspace/dds/dds-samples/hu-demo/state_qos_profiles.xml");
    qosFileNames.push_back("file:///home/anlijiu/disk/workspace/dds/dds-samples/hu-demo/request_reply_qos_profiles.xml");

    rti::core::QosProviderParams params;
    params.url_profile(qosFileNames);                                                                                             
    rti::core::default_qos_provider_params(params);

    mTimeoutTrigger = new TimeoutTrigger();
    mCanMessageDistributor = new CanMessageDistributor();
    mHvacModule = new hvac::HvacModule(mTimeoutTrigger);
    mCanMessageDistributor->addParseMap(mHvacModule->getParseMapFromCan());
    mCanMessageDistributor->start();
}



} //namespace dds
} //namespace dashboard
