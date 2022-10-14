#include <vector>
#include <string>

#include "DdsMessageAdapter.hpp"


namespace headunit {
namespace dds {


DdsMessageAdapter::DdsMessageAdapter(VehiclePropertyStore* propStore, const OnHalEvent& onHalEvent)
    : MessageAdapter(onHalEvent)
    , mPropStore(propStore)
    , mAsyncWaitset(AsyncWaitSet(4))
    , mDdsTranseiver(DdsTranseiver(onHalEvent, getValuePool(), mAsyncWaitset))
{
    std::cout << __func__ << std::endl;
}

DdsMessageAdapter::~DdsMessageAdapter() {
    std::cout << __func__ << std::endl;
}

void DdsMessageAdapter::receive_thread() {
    try {
        mAsyncWaitset.start();

        std::cout << "Wait for samples..." << std::endl;

        while (1) {
            usleep(1000000);
        }


    } catch (const std::exception &ex) {
        // This will catch DDS exceptions
        std::cerr << "Exception in DdsMessageAdapter::receive_thread(): " << ex.what() << std::endl;
    }


}

void DdsMessageAdapter::start() {
    mReceiveThread = std::thread(
                &DdsMessageAdapter::receive_thread, this);
}

void DdsMessageAdapter::stop() {}

void DdsMessageAdapter::send(const VehiclePropValue& propValue) {
    mDdsTranseiver.send(propValue);
}

void DdsMessageAdapter::onStart() {
}

} // dds
} // headunit
