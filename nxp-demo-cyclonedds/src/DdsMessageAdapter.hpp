#ifndef __DdsMessageAdapter__hpp__
#define __DdsMessageAdapter__hpp__

#include <thread>
#include <dds/dds.hpp>
#include "AsyncWaitSet.hpp"
#include "DdsTranseiver.hpp"
#include "VehiclePropertyStore.hpp"
#include "MessageAdapter.hpp"
#include "parallel_hashmap/phmap.h"

#include "vhal_compatible.hpp"

namespace headunit {
namespace dds {

/**
 * **注意** dds 中每一个message (一个struct) 都对应唯一一个 vehicle property
 */
class DdsMessageAdapter : public MessageAdapter {
public:
    // DdsMessageAdapter();
    DdsMessageAdapter(VehiclePropertyStore* propStore, const OnHalEvent& onHalEvent);

    virtual void start();
    virtual void stop();

    virtual void send(const VehiclePropValue& propValue);
    virtual void onStart();
    virtual ~DdsMessageAdapter();
private:
    void receive_thread();

private:
    AsyncWaitSet mAsyncWaitset;
    VehiclePropertyStore* mPropStore;
    std::thread mReceiveThread;
    DdsTranseiver mDdsTranseiver;
};


} // dds
} // headunit


#endif //__DdsMessageAdapter__hpp__
