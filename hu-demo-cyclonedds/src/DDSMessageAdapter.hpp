#ifndef __DDSMessageAdapter__hpp__
#define __DDSMessageAdapter__hpp__

#include <thread>
#include <dds/dds.hpp>
#include "AsyncWaitSet.hpp"
#include "HvacModule.hpp"
#include "SeatModule.hpp"
#include "HabitModule.hpp"
#include "parallel_hashmap/phmap.h"

#include "vhal_compatible.hpp"

namespace headunit {
namespace dds {

class DDSMessageAdapter : public MessageAdapter {
public:
    DDSMessageAdapter();

    virtual void start();
    virtual void stop();

    virtual StatusCode writeValue(const VehiclePropValue& propValue);
    virtual void onStart();
    virtual ~DDSMessageAdapter();
private:
    void addWriteMap(const phmap::flat_hash_map<int32_t, std::function<StatusCode(const VehiclePropValue &)>>& writeMap);
    void receive_thread();
    std::thread mReceiveThread;

    std::unique_ptr<hvac::HvacModule> mHvacModulePtr;
    std::unique_ptr<seat::SeatModule> mSeatModulePtr;
    std::unique_ptr<habit::HabitModule> mHabitModulePtr;
    AsyncWaitSet mAsyncWaitset;
    phmap::flat_hash_map<int32_t, std::function<StatusCode(const VehiclePropValue &)>> mWriteMap;
};


} // dds
} // headunit


#endif //__DDSMessageAdapter__hpp__
