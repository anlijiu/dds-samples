#ifndef __NxpService_HPP__
#define __NxpService_HPP__

#include <vector>

#include "VehiclePropertyStore.hpp"
#include "ObjectPool.hpp"

namespace headunit {
namespace dds {

class NxpService;

class NxpServiceManager {
public:
    NxpServiceManager(VehiclePropertyStore* store, std::function<void(const VehiclePropValue&)>);
private:
    std::vector<std::unique_ptr<NxpService>> mServices;
};

class NxpService {
public:
    using VehiclePropValuePtr = recyclable_ptr<VehiclePropValue>;
    using OnHalEvent = std::function<bool(VehiclePropValuePtr event)>;

    NxpService(VehiclePropertyStore* store);
    void init();

    virtual const std::vector<std::pair<int, int>>& getSupport() = 0;
    virtual void handle(const VehiclePropValue&) = 0 ;

    virtual ~NxpService() {};

    VehiclePropValuePool* getValuePool() {
        return &mValueObjectPool;
    }

protected:

private:
    VehiclePropValuePool mValueObjectPool;
    VehiclePropertyStore* mStore;
    // std::vector<std::pair<int prop, int area>> mAwarenessVec;
};

class AppointmentDriveService : public NxpService {
public:
    AppointmentDriveService(VehiclePropertyStore* store, std::function<void(const VehiclePropValue&)> send); 
    virtual const std::vector<std::pair<int, int>>& getSupport();
    virtual void handle(const VehiclePropValue& propValue);
private:
    std::vector<std::pair<int, int>> mAwarenessVec;
    std::function<void(const VehiclePropValue&)> mSendFunc;
};

} // dds
} // headunit


#endif  // __NxpService_HPP__
