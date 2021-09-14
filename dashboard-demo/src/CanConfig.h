#ifndef __CAN_CONFIG_H__
#define __CAN_CONFIG_H__

#include <functional>
#include <vector>
#include <parallel_hashmap/phmap.h>

#include "hvac/msg/AcPublisher.hpp"

//////////////////////////////////// 本文件自动生成， 不要修改 ////////////////////////////////////


// using ParseFrame = std::function<void(const uint8_t * data)>;
// typedef struct _read_value {
//     size_t size;
//     const ParseFrame* const parsers;
// } ReadValue;
// 

/////////////////////// 生成所有的publisher  TODO 摆脱全局变量
// static hvac::msg::AcPublisher acPubliser(0);
// static dashboard::dds::hvac::HvacModule hvacModule = dashboard::dds::hvac::HvacModule();



/////////////////////// 每一个CANID对应的publisher 数组
// const ParseFrame parsers_0x334[1] = { std::bind(&hvac::msg::AcPublisher::parseCanFrameForGlobal, &acPubliser,  std::placeholders::_1) };
// const ParseFrame parsers_0x334[1] = { std::bind(&dashboard::dds::hvac::HvacModule::parseCan0x334, &hvacModule,  std::placeholders::_1) };


// using ParseMapFromCan = phmap::flat_hash_map<uint32_t, ReadValue>;



// 1 message 1 signal  -> 1 struct
// 1 message n signals -> 1 struct
// n messages -> 1 struct
// 


// enum ParamType : int32_t {
//     STRING         = 0x00100000,
//     BOOLEAN        = 0x00200000,
//     INT32          = 0x00400000,
//     FLOAT          = 0x00800000,
// }


/**
 * key: CANID
 * value: 解析函数
 */
// const ParseMapFromCan kReadMaps {
//     { 0x334, { .size = 1, .parsers = parsers_0x334,  }
//     }
// };

#endif // __CAN_CONFIG_H__
