#ifndef __ABSTRACT_MODULE_HPP__
#define __ABSTRACT_MODULE_HPP__

#include <functional>
#include "parallel_hashmap/phmap.h"
#include "vhal_compatible.hpp"

namespace headunit {
namespace dds {
   
class AbsModule {
public:
    using WritePropValue = std::function<StatusCode(const VehiclePropValue& propValue)>;
    using WritePropValueMap = phmap::flat_hash_map<int32_t, WritePropValue>;
public:
    virtual ~AbsModule () = 0;
    virtual const phmap::flat_hash_map<int32_t, WritePropValue>& getWritePropValueMap() = 0;
private:
};

} // namespace dds
} // namespace headunit

#endif //__ABSTRACT_MODULE_HPP__
