#include <dds/core/ddscore.hpp>
#include <std_msgs/srv/Response.hpp>

#include "HvacModule.hpp"
#include "DdsUtils.hpp"

namespace dashboard {
namespace dds {
namespace hvac {

    using ParseFrame = std::function<void(const uint8_t * data)>;
    using ParseMapFromCan = phmap::flat_hash_map<uint32_t, ParseFrame>;
HvacModule::HvacModule(
        TimeoutTrigger* timer,
        int domain_id,
        std::string qos_profile_file,
        std::string qos_profile
) : mTimer(timer),
    mParticipant(::dds::core::null),
    mSetAcReplier(::dds::core::null),
    mTopicAc(::dds::core::null),
    mAcWriter(::dds::core::null),
    mParseMapFromCan({
        { 0x334, std::bind(&dashboard::dds::hvac::HvacModule::parseCan0x334, this,  std::placeholders::_1) }
    }),
    mSetAcListener(std::bind(
        &HvacModule::setAcFromDdsRequest,
        this,
        std::placeholders::_1))
{
    ::dds::core::QosProvider defaultQosProvider (::dds::core::null);
    defaultQosProvider = ::dds::core::QosProvider::Default();

    utils::find_or_create_domain_participant(
              domain_id, mParticipant, defaultQosProvider, "NvWa::State");

    // Create services
    // utils::create_replier<
    //         hvac_msgs::srv::SetAcRequest,
    //         std_msgs::srv::Response>(
    //         mSetAcReplier, mParticipant, "set_ac", defaultQosProvider);

    rti::request::ReplierParams replier_params(mParticipant);

    replier_params.service_name("Hvac::Ac");
    replier_params.datawriter_qos(defaultQosProvider.datawriter_qos(
            "RequestReply::Replier"));
    replier_params.datareader_qos(defaultQosProvider.datareader_qos(
            "RequestReply::Replier"));

    mSetAcReplier = rti::request::Replier<hvac_msgs::srv::SetAcRequest, std_msgs::srv::Response>(
                replier_params);

    mSetAcReplier.listener(&mSetAcListener);

    utils::find_or_create_topic<hvac_msgs::msg::Ac>(
            mParticipant, mTopicAc, hvac_msgs::msg::TOPIC_AC);

    utils::create_datawriter<hvac_msgs::msg::Ac>(
            mAcWriter,
            mParticipant,
            mTopicAc,
            defaultQosProvider);
}


HvacModule::~HvacModule() {
}

void HvacModule::parseCan0x334(const uint8_t * data) {
    parseAcFromCAN(data);
}

const phmap::flat_hash_map<uint32_t, ParseFrame>& HvacModule::getParseMapFromCan() {
    return mParseMapFromCan;
}

void HvacModule::parseAcFromCAN(const uint8_t * data) {
    // module -> ??????CANID ??? ???module??????????????? -> canid->signals 
    //??????canid?????????????????????????????????????????? ?????? mGlobalAc ????????? startbit,length,scale,offset
    uint32_t startBit = 0;
    uint32_t len= 1;
    double scaling = 1;
    double offset = 0;

    uint64_t origvalue = ::extract(data, startBit, len, UNSIGNED, MOTOROLA);
    double value = origvalue * scaling + offset;
    bool on = static_cast<bool>(value);

    // std::cout << __func__ << " value:" << value << " on:" << on << " orig on:" << mGlobalAc.on();
    mGlobalAc.zone(0);
    if(on != mGlobalAc.on()) {
        // if(need) {
        //     replier.send_reply(error_reply, request.info());
        //     timer.unregisterEvent(ac);
        // }
        mGlobalAc.on(on);

        auto propzone = (static_cast<uint64_t>(0) << 32) | static_cast<uint64_t>(mPropAc);//getPropZone(mPropAc, request.zone());
        if(mReplyAcFunc) {
            mReplyAcFunc(on);
            mReplyAcFunc = nullptr;
        }
    }

    mAcWriter->write(mGlobalAc);
}

std_msgs::srv::Response HvacModule::setAcFromDdsRequest(hvac_msgs::srv::SetAcRequest request)
{
    std::cout << " HvacModule::setAcFromDdsRequest thread id: " << std::this_thread::get_id() << std::endl;

    bool on = request.on();
    std::cout << __func__ << " in, ac: " << on << std::endl;

    //??????CAN ?????????timeout map ??? ????????????CAN ??????????????? ??????????????????reply
    //??????????????? ??????????????? ??????????????????ID?????????1??????????????????
    //??????????????????ID????????????
    //????????????setac ??????   set ac&zone   <->  canid & start bit & length & offset

    // canMessageAdapter->insert(/*uint8_t *frame, */uint32_t canid, uint8_t startbit, uint8_t length, uint64_t value, 
    //             uint8_t endianness);

    std::atomic<int64_t> counter { 0L };
    auto counterRef = std::ref(counter);
    auto propzone = (static_cast<uint64_t>(request.zone()) << 32) | static_cast<uint64_t>(mPropAc);//getPropZone(mPropAc, request.zone());
    mTimer->registerEvent(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(3)), propzone, std::bind(&HvacModule::setAcTimeout, this));
    mIsSetAcTimeout = true;
    while(mIsSetAcTimeout) {
        if(mGlobalAc.on() == request.on()) {
            mTimer->unregisterEvent(propzone);
            mDefaultReply.status(std_msgs::srv::ResponseStatus::REPLY_COMPLETED);
            mDefaultReply.error("");
            mTimer->unregisterEvent(propzone);
            return mDefaultReply;
        }
    }

    mDefaultReply.status() = std_msgs::srv::ResponseStatus::REPLY_ERROR;
    mDefaultReply.error() = "timeout";
    return mDefaultReply;
    // mReplyAcFunc = [&mTimer, request,  this, propzone, &info](bool on) {
    // mReplyAcFunc = [&](bool _on) {
    //     std::cout << " mReplyAcFunc in , on:" << on << ", request.on():" << on << std::endl;
    //     
    //     if(_on == on) {
    //         std::cout << " mReplyAcFunc in ,  send_reply!!!!!!!" << std::endl;
    //         mTimer->unregisterEvent(propzone);
    //         mDefaultReply.status(std_msgs::srv::ResponseStatus::REPLY_COMPLETED);
    //         mDefaultReply.error("");
    //         mSetAcReplier.send_reply(mDefaultReply, id);
    //     }
    // };
}

void HvacModule::setAcTimeout() {
    mIsSetAcTimeout = false;
}



}//namespace hvac
}//namespace dds
}//namespace dashboard
