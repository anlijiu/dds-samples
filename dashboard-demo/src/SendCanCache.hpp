#ifndef __SEND_CAN_CACHE__
#define __SEND_CAN_CACHE__

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <set>
#include <thread>
#include <unordered_map>
#include <vector>
#include "parallel_hashmap/phmap.h"
#include "memory_pool.h"
#include "Can.h"
#include "libwecan.h"

class SendCanCache {
private:
    // 1秒 == 1000毫秒 == 1000 000 微秒 == 1000 000 000 纳秒
    using Nanos = std::chrono::nanoseconds;//纳秒
    using Micros = std::chrono::microseconds;//毫秒
    using Millis = std::chrono::milliseconds;//微秒
    using Clock = std::chrono::system_clock;
    using TimePoint = std::chrono::time_point<Clock, Micros>;
public:
    using Action = std::function<void()>;
    SendCanCache(SendCanCache &&) = default;
    SendCanCache(std::function<void(const phmap::flat_hash_map<uint32_t, udp_can_packet_t *>&)> onFlush) : mOnFlush(onFlush) {
        mTimerThread = std::thread(&SendCanCache::loop, this);
    }

    virtual ~SendCanCache() {
        stop();
    }

    /**
     *
     */
    void write(uint32_t canid, uint8_t startbit, uint8_t len, uint64_t value) {
        if(absoluteTime == kInvalidTime) {
            absoluteTime = std::chrono::time_point_cast<std::chrono::milliseconds>(Clock::now()) + Micros(30);
        }

        {
            std::lock_guard<std::mutex> g(mLock);
            auto got = mCanPacketMap.find (canid);
            
            if ( got == mCanPacketMap.end() ) {
                auto packet = reinterpret_cast<udp_can_packet_t*>(mCanPacketPool.new_element());
                ::insert(packet->data, startbit, len, value, MOTOROLA);
                mCanPacketMap[canid] = packet;
            } else {
                auto packet = mCanPacketMap[canid];
                ::insert(packet->data, startbit, len, value, MOTOROLA);
            }
        }

        mCond.notify_one();
    }

private:

    void loop() {
        // static constexpr auto kInvalidTime = TimePoint(Nanos::max()); 
        while (!mStopRequested) {
            auto now = Clock::now();

            {
                std::unique_lock<std::mutex> g(mLock);

                if (absoluteTime <= now) {
                    absoluteTime = kInvalidTime;
                    mOnFlush(mCanPacketMap);

                    for (auto it = mCanPacketMap.cbegin(), next_it = it; it != mCanPacketMap.cend(); it = next_it ) {
                        ++next_it;
                        auto packet = it->second;
                        mCanPacketPool.delete_element(packet);
                        mCanPacketMap.erase(it);
                    }
                }
            }

            std::unique_lock<std::mutex> g(mLock);
            mCond.wait_until(g, absoluteTime);  // nextEventTime can be nanoseconds::max()
        }
    }

    void stop() {
        mStopRequested = true;
        {
            std::lock_guard<std::mutex> g(mLock);
            mCanPacketMap.clear();
        }
        mCond.notify_one();
        if (mTimerThread.joinable()) {
            mTimerThread.join();
        }
    }
private:
    TimePoint kInvalidTime = TimePoint(Micros::max());
    std::function<void(const phmap::flat_hash_map<uint32_t, udp_can_packet_t *>&)> mOnFlush;
    MemoryPool<udp_can_packet_s, 100> mCanPacketPool;
    TimePoint absoluteTime;  // Absolute time of timeout.
    mutable std::mutex mLock;
    std::thread mTimerThread;
    std::condition_variable mCond;
    std::atomic_bool mStopRequested { false };
    // std::unordered_map<uint32_t, udp_can_packet_t *> mCanPacketMap;
    phmap::flat_hash_map<uint32_t, udp_can_packet_t *> mCanPacketMap;
};


#endif //__SEND_CAN_CACHE__
