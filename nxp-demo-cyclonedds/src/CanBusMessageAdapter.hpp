
#ifndef __CanBusMessageAdapter_hpp__
#define __CanBusMessageAdapter_hpp__

#include "ConcurrentQueue.hpp"
#include "thread_safe.hpp"
#include "ObjectPool.hpp"
#include "CanTranseiver.hpp"
#include "CanTransformer.hpp"
#include "MessageAdapter.hpp"
#include "VehiclePropertyStore.hpp"

namespace headunit {
namespace dds {

class CanBusMessageAdapter: public MessageAdapter {
public:
    using SendCanCacheMap = thread_safe< std::map< uint32_t, CanFramePtr > >;

    CanBusMessageAdapter(VehiclePropertyStore* propStore, const OnHalEvent& onHalEvent);
    ~CanBusMessageAdapter() {};

    virtual void start();
    virtual void stop();
    virtual void send(const VehiclePropValue& propValue);
    virtual void onStart();

private:
    CanFramePool* getCanFramePool() {
        return &mCanFramePool;
    }

    void batchSend();
    void onCanFramesIncoming(const std::vector<CanFramePtr>& frames);

private:
    CanFramePool mCanFramePool;
    CanTranseiver mCanTranseiver;
    CanTransformer mCanTransformer;
    SendCanCacheMap mSendCanCacheMap;
    VehiclePropertyStore* mPropStore;
    std::condition_variable mCond;
    std::thread mSendCanThread;
};

} // namespace dds
} // namespace headunit
 
#endif // __CanBusMessageAdapter_hpp__
