#ifndef __DATA_HUB_HPP__
#define __DATA_HUB_HPP__

#include "CanMessageDistributor.hpp"
#include "HvacModule.hpp"
#include "TimeoutTrigger.hpp"

namespace dashboard {
namespace dds {

class DataHub {
public:
    DataHub();
private:
    CanMessageDistributor * mCanMessageDistributor;
    hvac::HvacModule * mHvacModule;
    TimeoutTrigger * mTimeoutTrigger;
};

} //namespace dds
} //namespace dashboard


#endif //__DATA_HUB_HPP__
