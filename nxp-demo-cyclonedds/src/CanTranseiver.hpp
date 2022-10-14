#ifndef __CanTranseiver_hpp__
#define __CanTranseiver_hpp__

#include "ConcurrentQueue.hpp"
#include "ObjectPool.hpp"

namespace headunit {
namespace dds {

/**
 * CAN数据帧收发单元
 * 对外提供发送CAN数据帧接口（不会立即发送，而是窗口10ms 的打包发送）
 * 接受CAN数据帧之后的callback 接口
 * 启动两个线程， 一个打包发送的线程， 一个接受数据帧的线程
 */
class CanTranseiver {
public:
    using OnCanFramesIncoming = std::function<void(const std::vector<CanFramePtr>&)>;
    CanTranseiver(const OnCanFramesIncoming&, CanFramePool*);
    ~CanTranseiver();
    void init();
    void send(can_frame_t*);
private:
    void receiveDataLoop();
private:
    std::thread mReceiveDataThread;
    CanFramePool* mCanFramePool;
    const OnCanFramesIncoming& mOnCanFramesIncoming;
    ConcurrentQueue<CanFramePtr> mEventQueue;
    BatchingConsumer<CanFramePtr> mBatchingConsumer;
};

} // namespace dds
} // namespace headunit
 
#endif //__CanTranseiver_hpp__
