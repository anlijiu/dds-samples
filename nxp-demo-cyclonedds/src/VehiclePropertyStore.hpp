#ifndef _Vehicle_Property_Store_hpp_
#define _Vehicle_Property_Store_hpp_

#include <cstdint>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "ThreadPool.hpp"
#include "vhal_compatible.hpp"

namespace headunit {
namespace dds {

class VehiclePropertyStore {
public:
    /* Function that used to calculate unique token for given VehiclePropValue */
    using TokenFunction = std::function<int64_t(const VehiclePropValue& value)>;

    VehiclePropertyStore();
private:
    struct RecordConfig {
        VehiclePropConfig propConfig;
        TokenFunction tokenFunction;
    };

    struct RecordId {
        int32_t prop;
        int32_t area;
        int64_t token;

        bool operator==(const RecordId& other) const;
        bool operator<(const RecordId& other) const;
    };

    using PropertyMap = std::map<RecordId, VehiclePropValue>;
    using PropertyMapRange = std::pair<PropertyMap::const_iterator, PropertyMap::const_iterator>;

public:
    void registerProperty(const VehiclePropConfig& config, TokenFunction tokenFunc = nullptr);
    void registerObserver(int32_t prop, int32_t area, std::function<void(const VehiclePropValue&)> callback);

    /* Stores provided value. Returns true if value was written returns false if config for
     * example wasn't registered. */
    bool writeValue(const VehiclePropValue& propValue, bool updateStatus);

    void removeValue(const VehiclePropValue& propValue);
    void removeValuesForProperty(int32_t propId);


    std::vector<VehiclePropValue> readAllValues() const;
    std::vector<VehiclePropValue> readValuesForProperty(int32_t propId) const;
    std::unique_ptr<VehiclePropValue> readValueOrNull(const VehiclePropValue& request) const;
    std::unique_ptr<VehiclePropValue> readValueOrNull(int32_t prop, int32_t area = 0,
                                                      int64_t token = 0) const;

    std::vector<VehiclePropConfig> getAllConfigs() const;
    const VehiclePropConfig* getConfigOrNull(int32_t propId) const;
    const VehiclePropConfig* getConfigOrDie(int32_t propId) const;

private:
    void notifyObservers(const RecordId& recId, const VehiclePropValue& propValue, bool updateStatus);
    RecordId getRecordIdLocked(const VehiclePropValue& valuePrototype) const;
    const VehiclePropValue* getValueOrNullLocked(const RecordId& recId) const;
    PropertyMapRange findRangeLocked(int32_t propId) const;

private:
    using MuxGuard = std::lock_guard<std::mutex>;
    mutable std::mutex mLock;
    std::unordered_map<int32_t /* VehicleProperty */, RecordConfig> mConfigs;

    PropertyMap mPropertyValues;  // Sorted map of RecordId : VehiclePropValue.
    
    std::map<RecordId, std::vector<std::function<void(const VehiclePropValue&)>>> mObservers;
    std::unique_ptr<ThreadPool> mThreadPool;
};

}  // namespace V2_0
}  // namespace vehicle

#endif //_Vehicle_Property_Store_hpp_
