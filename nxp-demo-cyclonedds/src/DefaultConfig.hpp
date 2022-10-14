#ifndef __DefaultConfig_H__
#define __DefaultConfig_H__

#include <map>


struct ConfigDeclaration {
    VehiclePropConfig config;

    /* This value will be used as an initial value for the property. If this field is specified for
     * property that supports multiple areas then it will be used for all areas unless particular
     * area is overridden in initialAreaValue field. */
    VehiclePropValue::RawValue initialValue;
    /* Use initialAreaValues if it is necessary to specify different values per each area. */
    std::map<int32_t, VehiclePropValue::RawValue> initialAreaValues;
};

const ConfigDeclaration kVehicleProperties[] {
        {.config = 
                 {
                         .prop = toInt(VehicleProperty::BATTERY_SET_PREHEAT),
                         .access = VehiclePropertyAccess::READ_WRITE,
                         .changeMode = VehiclePropertyChangeMode::ON_CHANGE,
                         .channelType = PropertyChannelType::CAN
                 },
         .initialValue = {.int32Values = {0}}},
        {.config =
                 {
                         .prop = toInt(VehicleProperty::APPOINTMENT_DRIVE),
                         .access = VehiclePropertyAccess::READ,
                         .changeMode = VehiclePropertyChangeMode::ON_CHANGE,
                         .channelType = PropertyChannelType::DDS
                 },
         .initialValue = {.int32Values = {0, 0, 0, 0, 0}}},
        {.config = 
                 {
                         .prop = toInt(VehicleProperty::SEAT_HEAT),
                         .access = VehiclePropertyAccess::READ,
                         .changeMode = VehiclePropertyChangeMode::ON_CHANGE,
                         .channelType = PropertyChannelType::DDS
                 },
         .initialValue = {.int32Values = {0}}},
        {.config = 
                 {
                         .prop = toInt(VehicleProperty::SEAT_SET_HEAT),
                         .access = VehiclePropertyAccess::WRITE,
                         .changeMode = VehiclePropertyChangeMode::ON_CHANGE,
                         .channelType = PropertyChannelType::DDS
                 },
         .initialValue = {.int32Values = {0}}},
        {.config = 
                 {
                         .prop = toInt(VehicleProperty::HVAC_AC_ON),
                         .access = VehiclePropertyAccess::READ_WRITE,
                         .changeMode = VehiclePropertyChangeMode::ON_CHANGE,
                         .channelType = PropertyChannelType::DDS
                 },
         .initialValue = {.int32Values = {0}}},
        {.config = 
                 {
                         .prop = toInt(VehicleProperty::HVAC_SET_AC_ON),
                         .access = VehiclePropertyAccess::READ_WRITE,
                         .changeMode = VehiclePropertyChangeMode::ON_CHANGE,
                         .channelType = PropertyChannelType::DDS
                 },
         .initialValue = {.int32Values = {0}}},
        {.config =
                 {
                         .prop = toInt(VehicleProperty::HU_TIME_SET),
                         .access = VehiclePropertyAccess::READ,
                         .changeMode = VehiclePropertyChangeMode::ON_CHANGE,
                         .channelType = PropertyChannelType::DDS
                 },
         .initialValue = {.int32Values = {0, 0}}},

        {.config = 
            {
                .prop = toInt(VehicleProperty::BATTERY_REMAINING_POWER),
                .access = VehiclePropertyAccess::READ,
                .changeMode = VehiclePropertyChangeMode::ON_CHANGE,
                .areaConfigs = {
                    VehicleAreaConfig {
                        .areaId = toInt(VehicleAreaBattery::ELECTRIC_VEHICLE_BATTERY) },
                    VehicleAreaConfig {
                        .areaId = toInt(VehicleAreaBattery::STARTING_LIGHTING_IGNITION)}
                },
                .channelType = PropertyChannelType::DDS
            },
            .initialValue = {.int32Values = {0}}
        }
};

#endif //__DefaultConfig_H__
