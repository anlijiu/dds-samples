#ifndef __DATA_HUB_HPP__
#define __DATA_HUB_HPP__

#include "CanMessageDistributor.hpp"
#include "SeatModule.hpp"
#include "TimeoutTrigger.hpp"
#include "AsyncWaitSet.hpp"

namespace dashboard {
namespace dds {

class DataHub {
public:
    DataHub();
private:
    CanMessageDistributor * mCanMessageDistributor;
    seat::SeatModule * mSeatModule;
    TimeoutTrigger * mTimeoutTrigger;

    AsyncWaitSet mAsyncWaitset;
};

} //namespace dds
} //namespace dashboard


#endif //__DATA_HUB_HPP__
