#include "NxpService.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include "vhal_compatible.hpp"

namespace headunit {
namespace dds {

NxpServiceManager::NxpServiceManager(VehiclePropertyStore* store, std::function<void(const VehiclePropValue&)> send)
{
    mServices.emplace_back(std::make_unique<AppointmentDriveService>(store, send));
    size_t len = mServices.size();
    for (size_t i =0; i < len; i ++) {
        auto vec = mServices[i]->getSupport();
        for(auto iter = vec.begin(); iter != vec.end(); iter++) {
            store->registerObserver(iter->first, iter->second, std::bind(&NxpService::handle, mServices[i].get(), std::placeholders::_1));
        }
    }
}

NxpService::NxpService(VehiclePropertyStore* store)
    : mStore(store)
{
    init();
}

void NxpService::init() {
}

AppointmentDriveService::AppointmentDriveService(VehiclePropertyStore* store,
        std::function<void(const VehiclePropValue&)> send)
    : NxpService(store)
    , mSendFunc(send)
    , mAwarenessVec({ { toInt(VehicleProperty::APPOINTMENT_DRIVE), toInt(VehicleArea::GLOBAL)} })
{
}

const std::vector<std::pair<int, int>>& AppointmentDriveService::getSupport() {
    return mAwarenessVec;
}

void AppointmentDriveService::handle(const VehiclePropValue& propValue) {
    std::cout << "AppointmentDriveService::handle prop: " << propValue.prop << " time:" << propValue.value.int64Values[0] << std::endl;
    auto timepoint = get_time_point(propValue.value.int64Values[0]);

 //    std::string strTime = fmt::format("{:%H:%M:%S}", timepoint);
	// fmt::print(fmt::fg(fmt::color::red) | fmt::bg(fmt::color::green), "AppointmentDriveService time : {0} \n", strTime);
 
    auto batteryPreHeat = getValuePool()->obtainBoolean(true);
    batteryPreHeat->prop = static_cast<int32_t>(VehicleProperty::BATTERY_SET_PREHEAT);
    batteryPreHeat->areaId = static_cast<int32_t>(VehicleAreaBattery::ELECTRIC_VEHICLE_BATTERY);
    batteryPreHeat->timestamp = elapsedRealtimeNano();
    mSendFunc(*batteryPreHeat);

    auto seatHeat = getValuePool()->obtainBoolean(true);
    seatHeat->prop = static_cast<int32_t>(VehicleProperty::SEAT_SET_HEAT);
    seatHeat->areaId = static_cast<int32_t>(VehicleAreaSeat::ROW_1_LEFT);
    seatHeat->timestamp = elapsedRealtimeNano();
    mSendFunc(*seatHeat);
}


} // dds
} // headunit
