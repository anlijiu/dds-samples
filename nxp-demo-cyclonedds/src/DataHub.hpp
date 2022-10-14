#ifndef __DataHub_hpp__
#define __DataHub_hpp__

#include <memory>

#include "NxpService.hpp"
#include "MessageAdapter.hpp"
#include "VehiclePropertyStore.hpp"
#include "DdsMessageAdapter.hpp"
#include "CanBusMessageAdapter.hpp"


namespace headunit {
namespace dds {

class DataHub {
public:
    DataHub();
    void send(const VehiclePropValue& propValue);

    bool onNewValueFromAdapter(VehiclePropValuePtr propValue);
private:
private:
    std::unique_ptr<VehiclePropertyStore> mPropStore;
    std::unique_ptr<MessageAdapter> mCanMessageAdapter;
    std::unique_ptr<MessageAdapter> mDdsMessageAdapter;
    NxpServiceManager mServiceManager;
};

} // dds
} // headunit

#endif //__DataHub_hpp__
