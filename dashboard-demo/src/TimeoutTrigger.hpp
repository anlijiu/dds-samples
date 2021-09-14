#ifndef __TIMEOUT_TRIGGER__
#define __TIMEOUT_TRIGGER__

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

class TimeoutTrigger {
private:
    using Nanos = std::chrono::nanoseconds;
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock, Nanos>;
public:
    using Action = std::function<void()>;

    TimeoutTrigger() {
        mTimerThread = std::thread(&TimeoutTrigger::loop, this);
    }

    virtual ~TimeoutTrigger() {
        stop();
    }

    /**
     *
     */
    void registerEvent(std::chrono::nanoseconds timeout, uint64_t cookie, const Action& action) {
        TimePoint now = Clock::now();
        TimePoint absoluteTime = now + timeout;

        {
            std::lock_guard<std::mutex> g(mLock);
            mCookieToEventsMap[cookie] = { timeout, cookie, absoluteTime, action };
        }
        mCond.notify_one();
    }

    void unregisterEvent(uint64_t cookie) {
        {
            std::lock_guard<std::mutex> g(mLock);
            mCookieToEventsMap.erase(cookie);
        }
        mCond.notify_one();
    }

    bool hasEvent(uint64_t propzone) {
        auto got = mCookieToEventsMap.find (propzone);

        if ( got == mCookieToEventsMap.end() ) {
            return false;
        } else {
            return true;
        }
    }


private:

    struct TimoutEvent {
        Nanos interval;
        uint64_t cookie;
        TimePoint absoluteTime;  // Absolute time of timeout.
        Action action;
    };

    void loop() {
        static constexpr auto kInvalidTime = TimePoint(Nanos::max());

        while (!mStopRequested) {
            auto now = Clock::now();
            auto nextEventTime = kInvalidTime;


            {
                std::unique_lock<std::mutex> g(mLock);

                for (auto it = mCookieToEventsMap.cbegin(), next_it = it; it != mCookieToEventsMap.cend(); it = next_it )
                {
                    ++next_it;
                    TimoutEvent event = it->second;

                    if (event.absoluteTime <= now) {
                        mCookieToEventsMap.erase(it);
                        event.action();
                    }

                    if (nextEventTime > event.absoluteTime) {
                        nextEventTime = event.absoluteTime;
                    }
                }

            }

            std::unique_lock<std::mutex> g(mLock);
            mCond.wait_until(g, nextEventTime);  // nextEventTime can be nanoseconds::max()
        }
    }

    void stop() {
        mStopRequested = true;
        {
            std::lock_guard<std::mutex> g(mLock);
            mCookieToEventsMap.clear();
        }
        mCond.notify_one();
        if (mTimerThread.joinable()) {
            mTimerThread.join();
        }
    }
private:
    mutable std::mutex mLock;
    std::thread mTimerThread;
    std::condition_variable mCond;
    std::atomic_bool mStopRequested { false };
    std::unordered_map<uint64_t, TimoutEvent> mCookieToEventsMap;
};


#endif //__TIMEOUT_TRIGGER__
