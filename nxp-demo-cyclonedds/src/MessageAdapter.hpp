#ifndef __MessageAdapter_HPP__
#define __MessageAdapter_HPP__

#include <map>
#include <memory>
#include <sys/socket.h>
#include <thread>
#include <unordered_set>

#include "ObjectPool.hpp"

namespace headunit {
namespace dds {

class MessageAdapter {
public:
    using VehiclePropValuePtr = recyclable_ptr<VehiclePropValue>;
    using OnHalEvent = std::function<bool(VehiclePropValuePtr event)>;

    MessageAdapter(const OnHalEvent& onHalEvent) : mOnHalEvent(onHalEvent) {
    }

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void send(const VehiclePropValue& propValue) = 0;

    // void bind(const OnHalEvent& onHalEvent) {
    //     mOnHalEvent = onHalEvent;
    // }

    virtual void onStart() = 0;
    virtual ~MessageAdapter() {};

    VehiclePropValuePool* getValuePool() {
        return &mValueObjectPool;
    }

protected:
    /**
     * MessageAdapter的实现类需要在总线有新值到来的时候调用此函数。
     * @param event 
     */
    void onNewProperty(VehiclePropValuePtr propValue) {
        mOnHalEvent(std::move(propValue));
    }


private:
    OnHalEvent mOnHalEvent;
    VehiclePropValuePool mValueObjectPool;
};

} // dds
} // headunit


#endif  // __MessageAdapter_HPP__
