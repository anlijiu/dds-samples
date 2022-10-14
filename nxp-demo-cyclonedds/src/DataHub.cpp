#include <functional>

#include "DataHub.hpp"
#include "DefaultConfig.hpp"
#include "Log.hpp"

namespace headunit {
namespace dds {

DataHub::DataHub()
    : mPropStore(std::make_unique<VehiclePropertyStore>())
    , mCanMessageAdapter(std::make_unique<CanBusMessageAdapter>(mPropStore.get(), std::bind(&DataHub::onNewValueFromAdapter, this, std::placeholders::_1)))
    , mDdsMessageAdapter(std::make_unique<DdsMessageAdapter>(mPropStore.get(), std::bind(&DataHub::onNewValueFromAdapter, this, std::placeholders::_1)))
    , mServiceManager(NxpServiceManager(mPropStore.get(), std::bind(&DataHub::send, this, std::placeholders::_1)))
{
    std::cout << "DataHub constructor " << mPropStore.get() << std::endl;

    for (size_t i = 0; i < arraysize(kVehicleProperties); i++) {
        mPropStore->registerProperty(kVehicleProperties[i].config);
    }
    mCanMessageAdapter->start();
    mDdsMessageAdapter->start();
}


void DataHub::send(const VehiclePropValue& propValue) {
    auto config = mPropStore->getConfigOrNull(propValue.prop);
    if (config == nullptr) {
        std::cout << "DataHub send error , no config for prop:" << propValue.prop << std::endl;
        return;
    }
    if(config->channelType == PropertyChannelType::CAN) {
        mCanMessageAdapter->send(propValue);
    } else if(config->channelType == PropertyChannelType::DDS) {
        mDdsMessageAdapter->send(propValue);
    } else {
        mDdsMessageAdapter->send(propValue);
    }
}

bool DataHub::onNewValueFromAdapter(VehiclePropValuePtr propValue) {

    if (mPropStore->writeValue(*propValue, true)) {
        std::cout << __func__ << " thread id: " << std::this_thread::get_id() << std::endl;
        Log::printVehicleProperty(*propValue);
    }
}

} //dds
} //headunit 
