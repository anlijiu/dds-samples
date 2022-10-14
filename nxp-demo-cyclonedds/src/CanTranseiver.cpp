#include "CanTranseiver.hpp" 
#include "Log.hpp"

namespace headunit {
namespace dds {

/* 打包CAN 数据帧时间间隔 10 ms */
constexpr std::chrono::milliseconds kCanFrameBatchingTimeWindow(10);


CanTranseiver::CanTranseiver(const OnCanFramesIncoming& onCanFramesIncoming, CanFramePool* canFramePool)
    : mOnCanFramesIncoming(onCanFramesIncoming),
      mCanFramePool(canFramePool)
{
    init();
}

CanTranseiver::~CanTranseiver() {
    mBatchingConsumer.requestStop();
    mEventQueue.deactivate();
    // We have to wait until consumer thread is fully stopped because it may
    // be in a state of running callback (onBatchHalEvent).
    mBatchingConsumer.waitStopped();
}

void CanTranseiver::init() {
    std::cout << "CanTranseiver::init start " << std::endl;
    mBatchingConsumer.run(&mEventQueue,
                          kCanFrameBatchingTimeWindow,
                          mOnCanFramesIncoming);
    mReceiveDataThread = std::thread(
            &CanTranseiver::receiveDataLoop, this);
}

void CanTranseiver::send(can_frame_t * frame) {
    //只是将数据打印出来， 真实项目替换为真实的通道， SPI或者其他什么的。
    std::cout << "CanTranseiver::send canId: " << frame->can_id << ", data: ";
    Log::printCanFrame(frame);
}

void CanTranseiver::receiveDataLoop() {
    while(true) {
        // fake impl  TODO 替换为真正的接收实现
        CanFramePtr frame1 = mCanFramePool->obtain();
        frame1->can_id = 0x3B6;
        frame1->can_dlc = 8;
        for(uint8_t i = 0; i < 8; ++i) {
            frame1->data[i] = i;
        }
        CanFramePtr frame2 = mCanFramePool->obtain();
        frame2->can_id = 123;
        frame2->can_dlc = 8;
        for(uint8_t i = 0; i < 8; ++i) {
            frame1->data[i] = 8 - i;
        }
        mEventQueue.push(std::move(frame1));
        mEventQueue.push(std::move(frame2));
        std::this_thread::sleep_for( std::chrono::milliseconds(30));
    }
}

} // namespace dds
} // namespace headunit

