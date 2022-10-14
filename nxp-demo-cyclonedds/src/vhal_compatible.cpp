#include "vhal_compatible.hpp"

#include <cmath>
#include <limits>
#include <chrono>


std::unique_ptr<VehiclePropValue> createVehiclePropValue(
    VehiclePropertyType type, size_t vecSize) {
    auto val = std::unique_ptr<VehiclePropValue>(new VehiclePropValue);
    switch (type) {
        case VehiclePropertyType::INT32:      // fall through
        case VehiclePropertyType::INT32_VEC:  // fall through
        case VehiclePropertyType::BOOLEAN:
            val->value.int32Values.resize(vecSize);
            break;
        case VehiclePropertyType::FLOAT:      // fall through
        case VehiclePropertyType::FLOAT_VEC:
            val->value.floatValues.resize(vecSize);
            break;
        case VehiclePropertyType::INT64:
        case VehiclePropertyType::INT64_VEC:
            val->value.int64Values.resize(vecSize);
            break;
        case VehiclePropertyType::BYTES:
            val->value.bytes.resize(vecSize);
            break;
        case VehiclePropertyType::STRING:
        case VehiclePropertyType::COMPLEX:
            break; // Valid, but nothing to do.
        default:
            val.reset(nullptr);
    }
    return val;
}

size_t getVehicleRawValueVectorSize(
    const VehiclePropValue::RawValue& value, VehiclePropertyType type) {
    switch (type) {
        case VehiclePropertyType::INT32:      // fall through
        case VehiclePropertyType::INT32_VEC:  // fall through
        case VehiclePropertyType::BOOLEAN:
            return value.int32Values.size();
        case VehiclePropertyType::FLOAT:      // fall through
        case VehiclePropertyType::FLOAT_VEC:
            return value.floatValues.size();
        case VehiclePropertyType::INT64:
        case VehiclePropertyType::INT64_VEC:
            return value.int64Values.size();
        case VehiclePropertyType::BYTES:
            return value.bytes.size();
        default:
            return 0;
    }
}

bool isEqual(const VehiclePropValue::RawValue& value1, const VehiclePropValue::RawValue& value2, VehiclePropertyType type) {
    switch (type) {
        case VehiclePropertyType::INT32:      // fall through
        case VehiclePropertyType::BOOLEAN:
            return value1.int32Values[0] == value2.int32Values[0];
        case VehiclePropertyType::INT64:
            return value1.int64Values[0] == value2.int64Values[0];
        case VehiclePropertyType::INT32_VEC:  // fall through
            {
                //ALOGE("%s 11111111111111", __func__);
                size_t int32vecSize = value1.int32Values.size();
                //ALOGE("%s 222222222222      size is %d", __func__, (int)int32vecSize);
                //ALOGE("%s 3333333333333     another size is %d", __func__, (int)(value2.int32Values.size()));
                if(int32vecSize == value2.int32Values.size()) {
                    //ALOGE("%s 444444444444444", __func__);
                    for(size_t i = 0; i < int32vecSize; ++i) {
                        if(value1.int32Values[i] != value2.int32Values[i]) return false;
                        else continue;
                    }
                    //ALOGE("%s 55555555555555555", __func__);
                } else {
                    return false;
                }
                return true;
            }
        case VehiclePropertyType::FLOAT:      // fall through
            return value1.floatValues[0] == value2.floatValues[0];
        case VehiclePropertyType::FLOAT_VEC:
            {
                size_t floatVecSize = value1.floatValues.size();
                if (floatVecSize == value2.floatValues.size()) {
                    for (size_t i = 0; i < floatVecSize; ++i) {
                        if (value1.floatValues[i] == value2.floatValues[i]) continue;
                        else return false;
                    }
                } else {
                    return false;
                }
                return true;
            }
        case VehiclePropertyType::INT64_VEC:
            {
                size_t int64VecSize = value1.int64Values.size();
                if (int64VecSize == value2.int64Values.size()) {
                    for (size_t i = 0; i < int64VecSize; ++i) {
                        if (value1.int64Values[i] == value2.int64Values[i]) continue;
                        else return false;
                    }
                }
                return true;
            }
        case VehiclePropertyType::BYTES:
            {
                size_t bytesSize = value1.bytes.size();
                if (bytesSize == value2.bytes.size()) {
                    for (size_t i = 0; i < bytesSize; ++i) {
                        if (value1.bytes[i] == value2.bytes[i]) continue;
                        else return false;
                    }
                }
                return true;
            }
        default:
            return false;
    }
}

bool isSame(double a, double b)
{
    return std::fabs(a - b) <  std::numeric_limits<double>::epsilon() ;
}

bool isSame(float a, float b)
{
    return std::fabs(a - b) < std::numeric_limits<float>::epsilon() ;
}

template<typename T>
inline void copyHidlVec(std::vector <T>* dest, const std::vector <T>& src) {
    for (size_t i = 0; i < std::min(dest->size(), src.size()); i++) {
        (*dest)[i] = src[i];
    }
}

void copyVehicleRawValue(VehiclePropValue::RawValue* dest,
                         const VehiclePropValue::RawValue& src) {
    dest->int32Values = src.int32Values;
    dest->floatValues = src.floatValues;
    dest->int64Values = src.int64Values;
    dest->bytes = src.bytes;
    dest->stringValue = src.stringValue;
}

template<typename T>
void shallowCopyHidlVec(std::vector <T>* dest, const std::vector <T>& src) {
    if (src.size() > 0) {
        *dest = src;
    } else if (dest->size() > 0) {
        dest->resize(0);
    }
}

void shallowCopyHidlStr(std::string* dest, const std::string& src) {
    if (src.empty()) {
        dest->clear();
    } else {
        *dest = src;
    }
}

void shallowCopy(VehiclePropValue* dest, const VehiclePropValue& src) {
    dest->prop = src.prop;
    dest->areaId = src.areaId;
    // dest->status = src.status;
    dest->timestamp = src.timestamp;
    shallowCopyHidlVec(&dest->value.int32Values, src.value.int32Values);
    shallowCopyHidlVec(&dest->value.int64Values, src.value.int64Values);
    shallowCopyHidlVec(&dest->value.floatValues, src.value.floatValues);
    shallowCopyHidlVec(&dest->value.bytes, src.value.bytes);
    shallowCopyHidlStr(&dest->value.stringValue, src.value.stringValue);
}



















uint64_t elapsedRealtimeNano()
{
    uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::
                  now().time_since_epoch()).count();
    return ns; 
}


std::chrono::time_point<std::chrono::high_resolution_clock> get_time_point(uint64_t timestamp) {
    return std::chrono::time_point<std::chrono::high_resolution_clock>()
        + std::chrono::seconds(timestamp);
}
