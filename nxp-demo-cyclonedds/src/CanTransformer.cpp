#include <time.h>
#include <iostream>
#include <unordered_map>

#include "libwecan.h"
#include "CanTransformer.hpp"

namespace headunit {
namespace dds {

void fillFrameWithSignalList(const std::vector<CanSignalMeta*> & signalMetas, const VehiclePropValue::RawValue& rawValue, VehiclePropertyType propType, uint8_t * data, bool useDefaultValue) {
    uint64_t value = 0;
    int rawInt  = 0;
    int64_t rawInt64 = 0;
    float rawFloat = 0;
    switch (propType) {
        case VehiclePropertyType::INT32_VEC:
            for(size_t i = 0; i < signalMetas.size(); ++i) {
                rawInt = useDefaultValue ? signalMetas[i]->defaultValue : rawValue.int32Values[i];
                value = signalMetas[i]->offset + signalMetas[i]->scaling * rawInt;

                ::insert(data, signalMetas[i]->startBit, signalMetas[i]->length, value, MOTOROLA);
            }
            break;
        case VehiclePropertyType::INT64_VEC:
            for(size_t i = 0; i < signalMetas.size(); ++i) {
                rawInt64 = useDefaultValue ? signalMetas[i]->defaultValue : rawValue.int64Values[i];
                value = signalMetas[i]->offset + signalMetas[i]->scaling * rawInt64;
                ::insert(data, signalMetas[i]->startBit, signalMetas[i]->length, value, MOTOROLA);
            }
            break;
        case VehiclePropertyType::STRING:
            if(rawValue.stringValue.size() > signalMetas.size()) return;
            for(size_t i = 0; i < rawValue.stringValue.size(); ++i) {
                value = rawValue.stringValue.c_str()[i];
                ::insert(data, signalMetas[i]->startBit, signalMetas[i]->length, value, MOTOROLA);
            }
            break;
        case VehiclePropertyType::FLOAT_VEC:
            for(size_t i = 0; i < signalMetas.size(); ++i) {
                rawFloat = useDefaultValue ? signalMetas[i]->defaultValue : rawValue.floatValues[i];
                value = static_cast<uint64_t>((rawFloat- signalMetas[i]->offset)/(signalMetas[i]->scaling));
                ::insert(data, signalMetas[i]->startBit, signalMetas[i]->length, value, MOTOROLA);
            }
            break;
        case VehiclePropertyType::INT32:
        case VehiclePropertyType::BOOLEAN:
        case VehiclePropertyType::FLOAT:
            break;
        default:
            break;
    }
}

void fillFrame(CanSignalMeta * signalMeta, const VehiclePropValue::RawValue& rawValue, VehiclePropertyType propType, uint8_t * data, bool useDefaultValue) {
    uint64_t value = 0;
    int rawInt = 0;
    float rawFloat = 0;

    if(useDefaultValue) {
        value = signalMeta->defaultValue;
        //ALOGE("%s set %s to default %x", __func__, signalMeta->name.c_str(), (int)value);
    } else {
        switch (propType) {
            case VehiclePropertyType::INT32:
            case VehiclePropertyType::BOOLEAN:
                rawInt = useDefaultValue ? signalMeta->defaultValue : rawValue.int32Values[0];
                value = (rawInt - signalMeta->offset)/(signalMeta->scaling);
                break;
            case VehiclePropertyType::FLOAT:
                rawFloat = useDefaultValue ? signalMeta->defaultValue : rawValue.floatValues[0];
                value = static_cast<uint64_t>((rawFloat- signalMeta->offset)/(signalMeta->scaling));
            	break;
           case VehiclePropertyType::INT64:
                value = useDefaultValue ? signalMeta->defaultValue : rawValue.int64Values[0];
                break;

	    default:
                break;
        }
    }

    std::cout << "fillFrame in value:" << value << ", startBit:" <<signalMeta->startBit << ", length:" <<  signalMeta->length << std::endl;
    ::insert(data, signalMeta->startBit, signalMeta->length, value, MOTOROLA);
}

bool extractPropValue(int prop, int zone, CanSignalMeta * signalMeta, const uint8_t * data, VehiclePropValue * propValue) {
    propValue->prop = prop;
    propValue->areaId = zone;
    propValue->timestamp = elapsedRealtimeNano();


    uint64_t origvalue = ::extract(data, signalMeta->startBit, signalMeta->length, UNSIGNED, MOTOROLA);

    double value = origvalue * signalMeta->scaling + signalMeta->offset;
     
    VehiclePropertyType type = getPropType(prop);
    switch (type) {
        case VehiclePropertyType::INT32:      // fall through
        case VehiclePropertyType::BOOLEAN:
            propValue->value.int32Values.resize(1);
            propValue->value.int32Values[0] = static_cast<int32_t>(value);
            break;
        case VehiclePropertyType::FLOAT:      // fall through
            propValue->value.floatValues.resize(1);
            propValue->value.floatValues[0] = static_cast<float>(value);
            break;
        case VehiclePropertyType::INT64:
            propValue->value.int64Values.resize(1);
            propValue->value.int64Values[0] = static_cast<int64_t>(value);
            break;
        case VehiclePropertyType::STRING:
            break;
        case VehiclePropertyType::INT32_VEC:
        case VehiclePropertyType::INT64_VEC:
        case VehiclePropertyType::BYTES:
        case VehiclePropertyType::COMPLEX:
            return false;
            break; // Valid, but nothing to do.
        default:
            return false;
    };
    return true;
}

/**
 * 发送VehiclePropValue时， 找到与prop&zone 对应的 dbc 定义
 * 以便下一步填充can数据帧对应的bit位
 */
static const std::unordered_map<uint64_t, std::pair<int, FillFrame>> kWriteablePropMaps {
    {
        getPropZone(toInt(VehicleProperty::HU_TIME_SET), toInt(VehicleArea::GLOBAL)),
        {
            0x3A5,
            std::bind(fillFrameWithSignalList,
                  std::vector<CanSignalMeta*> {
                      new CanSignalMeta("HU_TimeSet_Hour", 12, 5, 1, 0, 0, 31, false, "", 0x0 ),
                      new CanSignalMeta("HU_TimeSet_Minutes", 21, 6, 1, 0, 0, 60, false, "", 0x0 ),
                  }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
        }
    },
    {

        getPropZone(toInt(VehicleProperty::BATTERY_SET_PREHEAT), toInt(VehicleAreaBattery::ELECTRIC_VEHICLE_BATTERY)),
        {
            0x3B8,
            std::bind(fillFrame, new CanSignalMeta( "batter_set_preheat", 25, 2, 1, 0, 0, 3, false, "", 0x0 ), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
        }

    },
};


/**
 * 所有本机支持的CAN帧列表
 * key 为CAN ID, value 为CanMessageMeta 
 * CanMessageMeta可包含多个 prop&zone
 * 当有新的CAN数据帧到来时， 首先根据ID 找到对应的
 * prop&zone,  然后在kReadablePropMaps中找到具体的解析函数解析之
 */
const std::unordered_map<uint32_t, std::shared_ptr<CanMessageMeta>> kMsgMetaMap {
      { 0x3A5, std::make_shared<CanMessageMeta>( CanMessageMeta { "HU_0x3A5", 8, {
          getPropZone(toInt(VehicleProperty::HU_TIME_SET), toInt(VehicleArea::GLOBAL)),
      }})},
      { 0x3B6, std::make_shared<CanMessageMeta>( CanMessageMeta { "read Battery1", 8, {
          getPropZone(toInt(VehicleProperty::BATTERY_REMAINING_POWER), toInt(VehicleAreaBattery::ELECTRIC_VEHICLE_BATTERY)),
      }})},
      { 0x3B8, std::make_shared<CanMessageMeta>( CanMessageMeta { "write Battery1", 8, {
          getPropZone(toInt(VehicleProperty::BATTERY_SET_PREHEAT), toInt(VehicleAreaBattery::ELECTRIC_VEHICLE_BATTERY))
      }})}
};

/**
 * 给定一个prop&zone 和一个CAN数据帧， 如何从中提取数据的函数集
 */
static const std::unordered_map<uint64_t, ParseFrame> kReadablePropMaps {
    /* 电池剩余能量 */
    {
        getPropZone(toInt(VehicleProperty::BATTERY_REMAINING_POWER), toInt(VehicleAreaBattery::ELECTRIC_VEHICLE_BATTERY)),
        std::bind(extractPropValue, toInt(VehicleProperty::BATTERY_REMAINING_POWER), toInt(VehicleAreaBattery::ELECTRIC_VEHICLE_BATTERY),
                  new CanSignalMeta("VBU_BMS_PackSOE", 21, 10, 0.1, 0, 0, 102.3, false, "kWh", 0x3FE ),
                  std::placeholders::_1, std::placeholders::_2)
    },
};


CanTransformer::CanTransformer(VehiclePropValuePool* pool)
    : mVehiclePropValuePool(pool)
{
}

std::vector<VehiclePropValuePtr> CanTransformer::fromCan(can_frame_t * frame) {
    std::vector<VehiclePropValuePtr> vec;
    auto msgMetaIterator = kMsgMetaMap.find(frame->can_id);
    if(msgMetaIterator != kMsgMetaMap.end()) {
        for(auto& propZone : msgMetaIterator->second->propZoneIds) {
            uint32_t prop = getProp(propZone);
            auto readIterator = kReadablePropMaps.find(propZone);
            if(readIterator != kReadablePropMaps.end()) {
                auto parseFrame = readIterator->second;
                VehiclePropertyType type = getPropType(prop);
                auto v = mVehiclePropValuePool->obtain(type);

                if(parseFrame(frame->data, v.get())) {
                    vec.push_back(std::move(v));
                }
            }
        }
    }

    return vec;
}

uint32_t CanTransformer::canIdFromProperty(const VehiclePropValue& propValue) {
    int areaId = propValue.areaId == 0 ? toInt(VehicleArea::GLOBAL) : propValue.areaId;
    auto iter = kWriteablePropMaps.find(getPropZone(propValue.prop, areaId));
    if(iter != kWriteablePropMaps.end()) {
        auto pair = iter->second;
        return pair.first;
    }

    return 0;
}

void CanTransformer::fillCanFrame(const VehiclePropValue& propValue, can_frame_t * frame) {
    int areaId = propValue.areaId == 0 ? toInt(VehicleArea::GLOBAL) : propValue.areaId;
    auto iter = kWriteablePropMaps.find(getPropZone(propValue.prop, areaId));
    if(iter != kWriteablePropMaps.end()) {
        auto pair = iter->second;
        uint32_t canId = pair.first;
        auto fillFrame = pair.second;
        fillFrame(propValue.value, getPropType(propValue.prop), frame->data, false);
    }
}

} // namespace dds
} // namespace headunit
