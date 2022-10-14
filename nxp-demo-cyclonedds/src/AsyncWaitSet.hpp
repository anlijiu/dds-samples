#ifndef __ASYNC_WAITSET_HPP__
#define __ASYNC_WAITSET_HPP__

#include <dds/core/cond/StatusCondition.hpp>
#include <memory>
#include <thread>
#include "dds/dds.hpp"
#include "ThreadPool.hpp"

namespace headunit {
namespace dds {

class AsyncWaitSet
{
private:
    std::unique_ptr<ThreadPool> mThreadPool;
    ::dds::core::cond::WaitSet mWaitSet;
    std::thread mThread;
    ::dds::core::cond::GuardCondition mWaitGuardCond;
    bool mRunningStatus;


    void handleGuard();
    void run();
public:
    AsyncWaitSet(int threads_num = 4);
    virtual ~AsyncWaitSet();

    void start();
    void stop();

    void attach_condition(const ::dds::core::cond::Condition& cond);

    bool detach_condition(const ::dds::core::cond::Condition& cond);
};

} // dds
} // headunit
#endif /* ifndef __ASYNC_WAITSET_HPP__ */
