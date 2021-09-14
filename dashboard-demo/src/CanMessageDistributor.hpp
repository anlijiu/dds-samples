#ifndef  __CAN_MESSAGE_ADAPTER_HPP__
#define __CAN_MESSAGE_ADAPTER_HPP__

#include <thread>
#include <unordered_map>
#include "parallel_hashmap/phmap.h"
#include "CanMessageDistributor.hpp"
#include "SendCanCache.hpp"
#include "Can.h"

typedef struct _ParseArrayPerID {
    size_t size;
    const std::function<void(const uint8_t * data)>* const parsers;
} ParseArrayPerID;

class CanMessageDistributor {
using ParseMapFromCan = phmap::flat_hash_map<uint32_t, std::vector<std::function<void(const uint8_t * data)>>>;

public:
    CanMessageDistributor();

    void addParseMap(const phmap::flat_hash_map<uint32_t, std::function<void(const uint8_t * data)>>&);
    /**
    * @brief: 开启接收线程和发送线程
    */
    void start();

    /**
    * @brief: 写一个信号到CAN总线
    * 会加入SendCanCache 然后由cache做throttle/cache等
    * 然后回调到onFlushCan 集中发送
    *
    * @param: canid
    * @param: startbit
    * @param: len: length
    * @param: value
    */
    void write(uint32_t canid, uint8_t startbit, uint8_t len, uint64_t value);

    /**
    * @brief: SendCanCache 写入回调, 将本轮cache一次写入CAN线
    * 每轮cache多久在SendCanCache中指定
    *
    * @param: packetMap 
    *         <key: canid,  value: udp_can_packet_t*>
    */
    void onFlushCan(const phmap::flat_hash_map<uint32_t, udp_can_packet_t *>& packetMap);
private:
    void receive_loop();
    SendCanCache mSendCanCache;
    std::thread mReceiveDataThread;
    ParseMapFromCan mParseMap;
};

#endif //__CAN_MESSAGE_ADAPTER_HPP__
