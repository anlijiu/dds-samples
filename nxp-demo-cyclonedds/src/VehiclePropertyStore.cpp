#define LOG_TAG "VehiclePropertyStore"

#include "VehiclePropertyStore.hpp"
#include <iostream>

namespace headunit {
namespace dds {

bool VehiclePropertyStore::RecordId::operator==(const VehiclePropertyStore::RecordId& other) const {
    return prop == other.prop && area == other.area && token == other.token;
}

bool VehiclePropertyStore::RecordId::operator<(const VehiclePropertyStore::RecordId& other) const  {
    return prop < other.prop
           || (prop == other.prop && area < other.area)
           || (prop == other.prop && area == other.area && token < other.token);
}

void VehiclePropertyStore::registerProperty(const VehiclePropConfig& config,
                                            VehiclePropertyStore::TokenFunction tokenFunc) {
    MuxGuard g(mLock);
    mConfigs.insert({ config.prop, RecordConfig { config, tokenFunc } });
}

VehiclePropertyStore::VehiclePropertyStore()
    : mThreadPool(std::make_unique<ThreadPool>(4))
{
}

void VehiclePropertyStore::registerObserver(int32_t prop, int32_t area, std::function<void(const VehiclePropValue&)> callback) {
    RecordId recId = { prop, isGlobalProp(prop) ? 0 : area, 0 };
    auto it = mObservers.find(recId);
    if (it != mObservers.end()) {
        it->second.push_back(callback);
    } else {
        mObservers[recId] = { callback };
    }
}

bool VehiclePropertyStore::writeValue(const VehiclePropValue& propValue,
                                        bool updateStatus) {
    MuxGuard g(mLock);
    if (!mConfigs.count(propValue.prop)) return false;

    RecordId recId = getRecordIdLocked(propValue);
    VehiclePropValue* valueToUpdate = const_cast<VehiclePropValue*>(getValueOrNullLocked(recId));
    if (valueToUpdate == nullptr) {
        mPropertyValues.insert({ recId, propValue });
    } else {
        valueToUpdate->timestamp = propValue.timestamp;
        if(valueToUpdate->updateBy == propValue.updateBy && isEqual(valueToUpdate->value, propValue.value, getPropType(propValue.prop))) {
            // ALOGW("%s: VehiclePropertyStore: 0x%x prop not equal, go to update", __func__, propValue.prop);
            return false;
        }

        // ALOGW("%s: VehiclePropertyStore: 0x%x prop not equal, go to update", __func__, propValue.prop);
        valueToUpdate->updateBy = propValue.updateBy;
        valueToUpdate->value = propValue.value;
        if (updateStatus) {
            // valueToUpdate->status = propValue.status;
        }
    }
    notifyObservers(recId, propValue, updateStatus);
    return true;
}

void VehiclePropertyStore::removeValue(const VehiclePropValue& propValue) {
    MuxGuard g(mLock);
    RecordId recId = getRecordIdLocked(propValue);
    auto it = mPropertyValues.find(recId);
    if (it != mPropertyValues.end()) {
        mPropertyValues.erase(it);
    }
}

void VehiclePropertyStore::removeValuesForProperty(int32_t propId) {
    MuxGuard g(mLock);
    auto range = findRangeLocked(propId);
    mPropertyValues.erase(range.first, range.second);
}

std::vector<VehiclePropValue> VehiclePropertyStore::readAllValues() const {
    MuxGuard g(mLock);
    std::vector<VehiclePropValue> allValues;
    allValues.reserve(mPropertyValues.size());
    for (auto&& it : mPropertyValues) {
        allValues.push_back(it.second);
    }
    return allValues;
}

std::vector<VehiclePropValue> VehiclePropertyStore::readValuesForProperty(int32_t propId) const {
    std::vector<VehiclePropValue> values;
    MuxGuard g(mLock);
    auto range = findRangeLocked(propId);
    for (auto it = range.first; it != range.second; ++it) {
        values.push_back(it->second);
    }

    return values;
}

std::unique_ptr<VehiclePropValue> VehiclePropertyStore::readValueOrNull(
        const VehiclePropValue& request) const {
    MuxGuard g(mLock);
    RecordId recId = getRecordIdLocked(request);
    const VehiclePropValue* internalValue = getValueOrNullLocked(recId);
    return internalValue ? std::make_unique<VehiclePropValue>(*internalValue) : nullptr;
}

std::unique_ptr<VehiclePropValue> VehiclePropertyStore::readValueOrNull(
        int32_t prop, int32_t area, int64_t token) const {
    RecordId recId = {prop, isGlobalProp(prop) ? 0 : area, token };
    MuxGuard g(mLock);
    const VehiclePropValue* internalValue = getValueOrNullLocked(recId);
    return internalValue ? std::make_unique<VehiclePropValue>(*internalValue) : nullptr;
}

void VehiclePropertyStore::notifyObservers(const RecordId& recId, const VehiclePropValue& propValue, bool updateStatus) {

    auto result = mThreadPool->enqueue([this](const RecordId& recId, const VehiclePropValue& propValue, bool updateStatus) {
        auto it = mObservers.find(recId);
        if (it != mObservers.end()) {
            for(auto callback : it->second) {
                callback(propValue);
            }
        }
    }, recId, propValue, updateStatus);
}

std::vector<VehiclePropConfig> VehiclePropertyStore::getAllConfigs() const {
    MuxGuard g(mLock);
    std::vector<VehiclePropConfig> configs;
    configs.reserve(mConfigs.size());
    for (auto&& recordConfigIt: mConfigs) {
        configs.push_back(recordConfigIt.second.propConfig);
    }
    return configs;
}

const VehiclePropConfig* VehiclePropertyStore::getConfigOrNull(int32_t propId) const {
    MuxGuard g(mLock);
    auto recordConfigIt = mConfigs.find(propId);
    return recordConfigIt != mConfigs.end() ? &recordConfigIt->second.propConfig : nullptr;
}

const VehiclePropConfig* VehiclePropertyStore::getConfigOrDie(int32_t propId) const {
    auto cfg = getConfigOrNull(propId);
    if (!cfg) {
        abort();
    }
    return cfg;
}

VehiclePropertyStore::RecordId VehiclePropertyStore::getRecordIdLocked(
        const VehiclePropValue& valuePrototype) const {
    RecordId recId = {
        .prop = valuePrototype.prop,
        .area = isGlobalProp(valuePrototype.prop) ? 0 : valuePrototype.areaId,
        .token = 0
    };

    auto it = mConfigs.find(recId.prop);
    if (it == mConfigs.end()) return {};

    if (it->second.tokenFunction != nullptr) {
        recId.token = it->second.tokenFunction(valuePrototype);
    }
    return recId;
}

const VehiclePropValue* VehiclePropertyStore::getValueOrNullLocked(
        const VehiclePropertyStore::RecordId& recId) const  {
    auto it = mPropertyValues.find(recId);
    return it == mPropertyValues.end() ? nullptr : &it->second;
}

VehiclePropertyStore::PropertyMapRange VehiclePropertyStore::findRangeLocked(int32_t propId) const {
    // Based on the fact that mPropertyValues is a sorted map by RecordId.
    auto beginIt = mPropertyValues.lower_bound( RecordId { propId, INT32_MIN, 0 });
    auto endIt = mPropertyValues.lower_bound( RecordId { propId + 1, INT32_MIN, 0 });

    return  PropertyMapRange { beginIt, endIt };
}

}  // dds
}  // headunit
