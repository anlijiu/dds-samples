#ifndef __CONSTANTS__HPP
#define __CONSTANTS__HPP

#include <functional>
#include <vector>
#include <unordered_map>

using ParseFrame = std::function<void(const uint8_t * data)>;

typedef struct _read_value {
    size_t size;
    const ParseFrame* const parsers;
} ReadValue;

using ReadMap = std::unordered_map<uint32_t, ReadValue>;

#endif /* ifndef __CONSTANTS__HPP


 */
