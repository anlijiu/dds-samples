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

    mTimeoutTrigger = new TimeoutTrigger();
    mCanMessageDistributor = new CanMessageDistributor();
    mHvacModule = new hvac::HvacModule(mTimeoutTrigger);
    mCanMessageDistributor->addParseMap(mHvacModule->getParseMapFromCan());
    mCanMessageDistributor->start();
}



} //namespace dds
} //namespace dashboard
