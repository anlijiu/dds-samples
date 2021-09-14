#include "HvacModule.hpp"

#include <iostream>
#include <thread>
#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>
#include <rti/config/Logger.hpp>
#include <rti/core/cond/AsyncWaitSet.hpp>
#include <rti/util/util.hpp>  // for sleep()

#include "vhal_compatible.hpp"
#include "DdsUtils.hpp"

namespace headunit {
namespace dds {
namespace hvac {

HvacModule::HvacModule(
    rti::core::cond::AsyncWaitSet async_waitset,
    std::function<void(VehiclePropValuePtr)> func,
    VehiclePropValuePool* pool,
    DDS_DomainId_t domain_id
) : mReaderAc(::dds::core::null),
    mParticipant(::dds::core::null),
    mTopicAc(::dds::core::null),
    mSetAcRequester(::dds::core::null),
    mAsyncWaitSet(async_waitset),
    mValueObjectPoolPtr(pool),
    mWritePropValueMap({
        { toInt(VehicleProperty::HVAC_AC_ON), std::bind(&HvacModule::set_ac, this, std::placeholders::_1) }
    })
{

    std::cout << __func__ << std::endl;
    ::dds::core::QosProvider defaultQosProvider (::dds::core::null);
    defaultQosProvider = ::dds::core::QosProvider::Default();
    auto stateParticipantQos = defaultQosProvider.participant_qos("NvWa::State");
    auto acDataReaderQos = defaultQosProvider.datareader_qos("NvWa::State");
    auto acSubscriberQos = defaultQosProvider.subscriber_qos("NvWa::State");
    utils::find_or_create_domain_participant(
              domain_id, mParticipant, defaultQosProvider, "NvWa::State");

    utils::find_or_create_topic<hvac_msgs::msg::Ac>(
            mParticipant, mTopicAc, hvac_msgs::msg::TOPIC_AC);

    utils::create_datareader<hvac_msgs::msg::Ac>(
            mReaderAc,
            mParticipant,
            mTopicAc,
            acDataReaderQos,
            acSubscriberQos);

    // DataReader status condition: to process the reception of samples
    ::dds::core::cond::StatusCondition acReadStatusCondition(mReaderAc);
    acReadStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    acReadStatusCondition->handler(std::bind(&HvacModule::process_samples_ac, this, func));
    mAsyncWaitSet.attach_condition(acReadStatusCondition);


    rti::request::RequesterParams set_ac_requester_params(mParticipant);
    set_ac_requester_params.service_name("Hvac::Ac");
    set_ac_requester_params.datawriter_qos(defaultQosProvider.datawriter_qos(
            "RequestReply::Requester"));
    set_ac_requester_params.datareader_qos(defaultQosProvider.datareader_qos(
            "RequestReply::Requester"));

    mSetAcRequester = rti::request::Requester<hvac_msgs::srv::SetAcRequest, std_msgs::srv::Response>(set_ac_requester_params);

    while (rti::request::matched_replier_count(mSetAcRequester) == 0) {
        rti::util::sleep(::dds::core::Duration::from_millisecs(100));
    }

}

HvacModule::~HvacModule()
{
    std::cout << __func__ << std::endl;
    mAsyncWaitSet.detach_condition(
            ::dds::core::cond::StatusCondition(mReaderAc));
}

void HvacModule::process_samples_ac(std::function<void(VehiclePropValuePtr)> func)
{

    std::cout << " HvacModule::process_samples_ac thread id: " << std::this_thread::get_id() << std::endl;
    std::cout << __func__ << std::endl;
    // Take all samples This will reset the StatusCondition
    ::dds::sub::LoanedSamples<hvac_msgs::msg::Ac> samples = mReaderAc.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    // Release status condition in case other threads can process outstanding
    // samples
    mAsyncWaitSet.unlock_condition(
            ::dds::core::cond::StatusCondition(mReaderAc));

    // Process sample
    for (::dds::sub::LoanedSamples<hvac_msgs::msg::Ac>::iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            // std::cout << "Received sample:\n\t" << sample_it->data()
            //           << std::endl;
            func(properties_from_ac(sample_it->data()));
        }
    }
}

VehiclePropValuePtr HvacModule::properties_from_ac(const hvac_msgs::msg::Ac& ac) {
    auto v = mValueObjectPoolPtr->obtain(VehiclePropertyType::BOOLEAN, 2);
    v->prop = static_cast<int32_t>(VehicleProperty::HVAC_AC_ON);
    v->value.int32Values.resize(1);
    v->value.int32Values[0] = static_cast<int32_t>(ac.on());
    v->areaId = ac.zone();
    v->timestamp = elapsedRealtimeNano();
    return v;
}

const phmap::flat_hash_map<int32_t, WritePropValue>& HvacModule::getWritePropValueMap() {
    return mWritePropValueMap;
}

StatusCode HvacModule::set_ac(const VehiclePropValue& propValue) {
    std::cout << " HvacModule::set_ac thread id: " << std::this_thread::get_id() << std::endl;

    std::cout << "set_ac in" << propValue.value.int32Values[0] <<  std::endl;
    auto request_id =
            mSetAcRequester.send_request(hvac_msgs::srv::SetAcRequest(propValue.value.int32Values[0], propValue.areaId));

    // Receive replies
    const auto MAX_WAIT = ::dds::core::Duration::from_secs(20);

    bool in_progress = true;
    while(in_progress) {
        auto replies = mSetAcRequester.receive_replies(MAX_WAIT);
        std::cout << "set_ac receive " << replies.length() << " samples" << std::endl;

        // When receive_replies times out,
        // it returns an empty reply collection
        if (replies.length() == 0) {
            std::cout<< "ResponseStatus::TIMEOUT" << std::endl;
            return StatusCode::TIMEOUT;
        }

        // Print the prime numbers we receive
        for (const auto& reply : replies) {
            if (!reply.info().valid()) {
                continue;
            }

            for (auto error: reply.data().error()) {
                std::cout << " reply.data().error(): " << error << " ";
            }

            if (reply.data().status() != std_msgs::srv::ResponseStatus::REPLY_IN_PROGRESS) {
                in_progress = false;
                if (reply.data().status() == std_msgs::srv::ResponseStatus::REPLY_ERROR) {

                    std::cout<< "ResponseStatus::REPLY_ERROR" << std::endl;
                    return StatusCode::INTERNAL_ERROR;
                    // throw std::runtime_error("Error in replier");
                } else { // reply->status == COMPLETED
                    std::cout << "DONE";
                    return StatusCode::OK;
                }
            }

            std::cout << std::endl;
        }
    }
}

} // hvac
} // dds
} // headunit
