#include "DataHub.hpp"
#include "CanMessageDistributor.hpp"

namespace dashboard {
namespace dds {

DataHub::DataHub()
    : mTimeoutTrigger(nullptr)
    , mCanMessageDistributor(nullptr)
    , mSeatModule(nullptr)
    , mAsyncWaitset(AsyncWaitSet(4))
{

    std::vector<std::string> qosFileNames;
    std::cout<< "DDSMessageAdapter init " << std::endl;

    mTimeoutTrigger = new TimeoutTrigger();
    mCanMessageDistributor = new CanMessageDistributor();
    mSeatModule = new seat::SeatModule(mAsyncWaitset, mTimeoutTrigger);
    mCanMessageDistributor->start();
    mAsyncWaitset.start();
}



} //namespace dds
} //namespace dashboard
