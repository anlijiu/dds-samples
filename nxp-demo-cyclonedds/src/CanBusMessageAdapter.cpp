#include "CanBusMessageAdapter.hpp"
#include "Log.hpp"


namespace headunit {
namespace dds {

CanBusMessageAdapter::CanBusMessageAdapter(VehiclePropertyStore* propStore, const OnHalEvent& onHalEvent)
    : MessageAdapter(onHalEvent)
    , mPropStore(propStore)
    , mCanTransformer(getValuePool())
    , mCanTranseiver(CanTranseiver(std::bind(&CanBusMessageAdapter::onCanFramesIncoming, this, std::placeholders::_1), getCanFramePool()))
{
    std::cout << "CanBusMessageAdapter constructor " << std::endl;

}

void CanBusMessageAdapter::send(const VehiclePropValue& propValue) {
    uint32_t canId = mCanTransformer.canIdFromProperty(propValue);
    std::cout << "CanBusMessageAdapter send prop:" << propValue.prop << ", canId:" << canId << std::endl;
    if(canId == 0) {
        // TODO 打印错误
        std::cout << "CanBusMessageAdapter send error , no canid for prop:" << propValue.prop << std::endl;
        return;
    }

    /* 并没有实际发送， 而是缓存之后打包发送 */
    mSendCanCacheMap.write( [&] (auto&& m) {
        auto it = m.find(canId);
        if(it != m.end()) {
            mCanTransformer.fillCanFrame(propValue, m[canId].get());
        } else {
            auto frame = mCanFramePool.obtain();
            Log::printCanFrame(frame.get());

            frame->can_id = canId;
            mCanTransformer.fillCanFrame(propValue, frame.get());
            m.insert(std::make_pair(canId, std::move(frame)));
        }
    });
}

/* 打包发送， 真正的发送 */
void CanBusMessageAdapter::batchSend() {
    while(true) {
        mSendCanCacheMap.write( [&](auto&& m) {
            auto it = m.begin();
            for(; it != m.end(); ) {
                mCanTranseiver.send(it->second.get());
                m.erase(it++);
            }
        });
 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void CanBusMessageAdapter::start() {
    mSendCanThread = std::thread(
                    &CanBusMessageAdapter::batchSend, this);
}
void CanBusMessageAdapter::stop() {}
void CanBusMessageAdapter::onStart() {}

void CanBusMessageAdapter::onCanFramesIncoming(const std::vector<CanFramePtr>& frames) {
    for (auto const& frame: frames) {
        auto values = mCanTransformer.fromCan(frame.get());
        for(auto & vehiclePropValuePtr : values) {
            onNewProperty(std::move(vehiclePropValuePtr));
        }
    }
}

}//dds
}//headunit 
