#include "AsyncWaitSet.hpp"

namespace headunit {
namespace dds {

AsyncWaitSet::AsyncWaitSet(int threads_num)
    : mThreadPool(std::make_unique<ThreadPool>(threads_num))
    , mWaitGuardCond(std::bind(&AsyncWaitSet::handleGuard, this ))
    , mWaitSet()
{
}

AsyncWaitSet::~AsyncWaitSet() {
}

void AsyncWaitSet::start() {
    mRunningStatus = true;
    mWaitSet.attach_condition(mWaitGuardCond);
    mThread = std::thread(
                &AsyncWaitSet::run, this);
}

void AsyncWaitSet::stop() {
    mRunningStatus = false;
    mWaitGuardCond.trigger_value(true);
    mThread.join();
    mWaitSet.detach_condition(mWaitGuardCond);
}

void AsyncWaitSet::handleGuard() {
    std::cout << "AsyncWaitSet::"<< __func__ << mWaitGuardCond.trigger_value() << std::endl;
}

void AsyncWaitSet::run() {
    while(mRunningStatus) {
        std::cout << "AsyncWaitSet::"<< __func__ <<  ", mRunningStatus:" << mRunningStatus << std::endl;
        mWaitSet.wait();
    }
}

::dds::core::cond::WaitSet& AsyncWaitSet::attach_condition(const dds::core::cond::Condition& cond) {
    return mWaitSet.attach_condition(cond);
}

bool AsyncWaitSet::detach_condition(const dds::core::cond::Condition& cond) {
    return mWaitSet.detach_condition(cond);
}
    
} //dds
} // headunit
