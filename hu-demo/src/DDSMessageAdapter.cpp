#include <rti/core/cond/AsyncWaitSet.hpp>

#include <vector>
#include <string>

#include "DDSMessageAdapter.hpp"


namespace headunit {
namespace dds {

DDSMessageAdapter::DDSMessageAdapter()
    : mAsyncWaitset(::dds::core::null)
    , mHvacModulePtr(nullptr)
{
    std::cout << __func__ << std::endl;
    int domain_id = 0;
    int thread_pool_size = 4;
    mAsyncWaitset = rti::core::cond::AsyncWaitSet(
                rti::core::cond::AsyncWaitSetProperty().thread_pool_size(
                        thread_pool_size));

    mHvacModulePtr = std::make_unique<hvac::HvacModule>(mAsyncWaitset, std::bind(&DDSMessageAdapter::onNewProperty, this, std::placeholders::_1), getValuePool(), domain_id);
    addWriteMap(mHvacModulePtr->getWritePropValueMap());
}

DDSMessageAdapter::~DDSMessageAdapter() {
    std::cout << __func__ << std::endl;
}

void DDSMessageAdapter::receive_thread() {
    std::cout <<"DDSMessageAdapter::"<< __func__ <<" start"<< std::endl;

    try {
        mAsyncWaitset.start();

        std::cout << "Wait for samples..." << std::endl;

        // while (1) {
            // usleep(1000000);
            // rti::util::sleep(dds::core::Duration(1));
        // }


    } catch (const std::exception &ex) {
        // This will catch DDS exceptions
        std::cerr << "Exception in DDSMessageAdapter::receive_thread(): " << ex.what() << std::endl;
    }


    std::cout <<"DDSMessageAdapter::"<< __func__ <<" end"<< std::endl;
}

void DDSMessageAdapter::start() {
    mReceiveThread = std::thread(
                &DDSMessageAdapter::receive_thread, this);
}

void DDSMessageAdapter::stop() {}

StatusCode DDSMessageAdapter::writeValue(const VehiclePropValue& propValue) {
    //根据propid & area 转换成 对应的module  & struct

    auto got = mWriteMap.find (propValue.prop);

    if ( got != mWriteMap.end() ) {
        return got->second(propValue);
    }

    return StatusCode::NOT_AVAILABLE;
}

void DDSMessageAdapter::onStart() {
}

void DDSMessageAdapter::addWriteMap(const phmap::flat_hash_map<int32_t, std::function<StatusCode(const VehiclePropValue &)>>& writeMap) {
    for (auto const& x : writeMap) {
        mWriteMap[x.first] = { x.second };
    }
}

} // dds
} // headunit
