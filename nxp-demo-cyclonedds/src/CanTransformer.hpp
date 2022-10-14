/**
 * 将 VehiclePropertyValue 转为 CAN Frame 
 * 将 CAN Frame 转为 VehiclePropertyValue 
 */
#ifndef __CanTransformer_hpp__
#define __CanTransformer_hpp__


#include "ObjectPool.hpp"
#include "vhal_compatible.hpp"

namespace headunit {
namespace dds {

using ParseFrame = std::function<bool(const uint8_t * data, VehiclePropValue * v)>;
using FillFrame = std::function<void(const VehiclePropValue::RawValue& rawValue, VehiclePropertyType propType, uint8_t * data, bool useDefaultValue)>;

class CanTransformer {
public:

    CanTransformer(VehiclePropValuePool*);
    std::vector<VehiclePropValuePtr> fromCan(can_frame_t * frame);

    /**
     * 查找property 对应的CANID ， 如果未找到则返回0
     */
    uint32_t canIdFromProperty(const VehiclePropValue& val);

    void fillCanFrame(const VehiclePropValue&, can_frame_t *);

private:
    VehiclePropValuePool* mVehiclePropValuePool;
};

} // namespace dds
} // namespace headunit


#endif //__CanTransformer_hpp__

