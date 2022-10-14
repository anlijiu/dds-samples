#ifndef __ConcurrentQueue_hpp__
#define __ConcurrentQueue_hpp__

#include <queue>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <functional>

template<typename T>
class ConcurrentQueue {
public:
    void waitForItems() {
        std::unique_lock<std::mutex> g(mLock);
        while (mQueue.empty() && mIsActive) {
            mCond.wait(g);
        }
    }

    std::vector<T> flush() {
        std::vector<T> items;

        MuxGuard g(mLock);
        if (mQueue.empty() || !mIsActive) {
            return items;
        }
        while (!mQueue.empty()) {
            items.push_back(std::move(mQueue.front()));
            mQueue.pop();
        }
        return items;
    }

    void push(T&& item) {
        {
            MuxGuard g(mLock);
            if (!mIsActive) {
                return;
            }
            mQueue.push(std::move(item));
        }
        mCond.notify_one();
    }

    /* Deactivates the queue, thus no one can push items to it, also
     * notifies all waiting thread.
     */
    void deactivate() {
        {
            MuxGuard g(mLock);
            mIsActive = false;
        }
        mCond.notify_all();  // To unblock all waiting consumers.
    }

    ConcurrentQueue() = default;

    ConcurrentQueue(const ConcurrentQueue &) = delete;
    ConcurrentQueue &operator=(const ConcurrentQueue &) = delete;
private:
    using MuxGuard = std::lock_guard<std::mutex>;

    bool mIsActive = true;
    mutable std::mutex mLock;
    std::condition_variable mCond;
    std::queue<T> mQueue;
};

template<typename T>
class BatchingConsumer {
private:
    enum class State {
        INIT = 0,
        RUNNING = 1,
        STOP_REQUESTED = 2,
        STOPPED = 3,
    };

public:
    BatchingConsumer() : mState(State::INIT) {}

    BatchingConsumer(const BatchingConsumer &) = delete;
    BatchingConsumer &operator=(const BatchingConsumer &) = delete;

    using OnBatchReceivedFunc = std::function<void(const std::vector<T>& vec)>;

    void run(ConcurrentQueue<T>* queue,
             std::chrono::nanoseconds batchInterval,
             const OnBatchReceivedFunc& func) {
        mQueue = queue;
        mBatchInterval = batchInterval;

        mWorkerThread = std::thread(
            &BatchingConsumer<T>::runInternal, this, func);
    }

    void requestStop() {
        mState = State::STOP_REQUESTED;
    }

    void waitStopped() {
        if (mWorkerThread.joinable()) {
            mWorkerThread.join();
        }
    }

private:
    void runInternal(const OnBatchReceivedFunc& onBatchReceived) {
        if (mState.exchange(State::RUNNING) == State::INIT) {
            while (State::RUNNING == mState) {
                mQueue->waitForItems();
                if (State::STOP_REQUESTED == mState) break;

                std::this_thread::sleep_for(mBatchInterval);
                if (State::STOP_REQUESTED == mState) break;

                std::vector<T> items = mQueue->flush();

                if (items.size() > 0) {
                    onBatchReceived(items);
                }
            }
        }

        mState = State::STOPPED;
    }

private:
    std::thread mWorkerThread;

    std::atomic<State> mState;
    std::chrono::nanoseconds mBatchInterval;
    ConcurrentQueue<T>* mQueue;
};

#endif //__ConcurrentQueue_hpp__
