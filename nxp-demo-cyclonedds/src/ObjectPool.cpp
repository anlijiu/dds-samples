#include "ObjectPool.hpp"
#include <string.h>

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

CanFramePool::RecyclableType CanFramePool::obtain() {
    auto val = mPool->obtain();
    bzero(val.get(), sizeof(struct can_frame_s));
    return val;
}

can_frame_t* CanFramePool::InternalPool::createObject() {
    auto val = std::unique_ptr<can_frame_t>(new can_frame_t);
    return val.release();
}
