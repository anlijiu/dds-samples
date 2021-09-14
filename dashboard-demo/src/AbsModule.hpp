#ifndef __ABSTRACT_MODULE_HPP__
#define __ABSTRACT_MODULE_HPP__

#include <functional>
#include "parallel_hashmap/phmap.h"

namespace dashboard {
namespace dds {
   
class AbsModule {

public:
    using ParseFrame = std::function<void(const uint8_t * data)>;
    using ParseMapFromCan = phmap::flat_hash_map<uint32_t, ParseFrame>;
public:
    virtual ~AbsModule () = 0;

    virtual const ParseMapFromCan & getParseMapFromCan() = 0;
private:
};

} // namespace dds
} // namespace dashboard

#endif //__ABSTRACT_MODULE_HPP__
