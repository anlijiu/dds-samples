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



















VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtain(
        VehiclePropertyType type, size_t vecSize) {
    return isDisposable(type, vecSize)
           ? obtainDisposable(type, vecSize)
           : obtainRecylable(type, vecSize);
}

VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtain(
        const VehiclePropValue& src) {
    if (src.prop == toInt(VehicleProperty::INVALID)) {
        return RecyclableType();
    }
    VehiclePropertyType type = getPropType(src.prop);
    size_t vecSize = getVehicleRawValueVectorSize(src.value, type);;
    auto dest = obtain(type, vecSize);

    dest->prop = src.prop;
    dest->areaId = src.areaId;
    // dest->status = src.status;
    dest->timestamp = src.timestamp;
    copyVehicleRawValue(&dest->value, src.value);

    return dest;
}

VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtainInt32(
        int32_t value) {
    auto val = obtain(VehiclePropertyType::INT32);
    val->value.int32Values[0] = value;
    return val;
}

VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtainInt64(
        int64_t value) {
    auto val = obtain(VehiclePropertyType::INT64);
    val->value.int64Values[0] = value;
    return val;
}

VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtainFloat(
        float value)  {
    auto val = obtain(VehiclePropertyType::FLOAT);
    val->value.floatValues[0] = value;
    return val;
}

VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtainString(
        const char* cstr) {
    auto val = obtain(VehiclePropertyType::STRING);
    val->value.stringValue = cstr;
    return val;
}

VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtainComplex() {
    return obtain(VehiclePropertyType::COMPLEX);
}

VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtainRecylable(
        VehiclePropertyType type, size_t vecSize) {
    // VehiclePropertyType is not overlapping with vectorSize.
    int32_t key = static_cast<int32_t>(type)
                  | static_cast<int32_t>(vecSize);

    std::lock_guard<std::mutex> g(mLock);
    auto it = mValueTypePools.find(key);

    if (it == mValueTypePools.end()) {
        auto newPool(std::make_unique<InternalPool>(type, vecSize));
        it = mValueTypePools.emplace(key, std::move(newPool)).first;
    }
    return it->second->obtain();
}

VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtainBoolean(
        bool value)  {
    return obtainInt32(value);
}

VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtainDisposable(
        VehiclePropertyType valueType, size_t vectorSize) const {
    return RecyclableType {
        createVehiclePropValue(valueType, vectorSize).release(),
        mDisposableDeleter
    };
}

VehiclePropValuePool::RecyclableType VehiclePropValuePool::obtain(
        VehiclePropertyType type) {
    return obtain(type, 1);
}


void VehiclePropValuePool::InternalPool::recycle(VehiclePropValue* o) {
    if (o == nullptr) {
        return;
    }

    if (!check(&o->value)) {
        delete o;
    } else {
        ObjectPool<VehiclePropValue>::recycle(o);
    }
}

bool VehiclePropValuePool::InternalPool::check(VehiclePropValue::RawValue* v) {
    return check(&v->int32Values, (VehiclePropertyType::INT32 == mPropType ||
                                   VehiclePropertyType::INT32_VEC == mPropType ||
                                   VehiclePropertyType::BOOLEAN == mPropType)) &&
           check(&v->floatValues, (VehiclePropertyType::FLOAT == mPropType ||
                                   VehiclePropertyType::FLOAT_VEC == mPropType)) &&
           check(&v->int64Values, (VehiclePropertyType::INT64 == mPropType ||
                                   VehiclePropertyType::INT64_VEC == mPropType)) &&
           check(&v->bytes, VehiclePropertyType::BYTES == mPropType) && v->stringValue.size() == 0;
}

VehiclePropValue* VehiclePropValuePool::InternalPool::createObject() {
    return createVehiclePropValue(mPropType, mVectorSize).release();
}

uint64_t elapsedRealtimeNano()
{
    uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::
                  now().time_since_epoch()).count();
    return ns; 
}
