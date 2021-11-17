#include <dds/core/ddscore.hpp>
#include <std_msgs/srv/Response.hpp>

#include "HvacModule.hpp"
#include "DdsUtils.hpp"

namespace dashboard {
namespace dds {
namespace hvac {

    using ParseFrame = std::function<void(const uint8_t * data)>;
    using ParseMapFromCan = std::unordered_map<uint32_t, ParseFrame>;
HvacModule::HvacModule(
        TimeoutTrigger* timer,
        int domain_id,
        std::string qos_profile_file,
        std::string qos_profile
) : mTimer(timer),
    mParticipant(::dds::core::null),
    mTopicAc(::dds::core::null),
    mAcWriter(::dds::core::null),
    mParseMapFromCan({
        { 0x334, std::bind(&dashboard::dds::hvac::HvacModule::parseCan0x334, this,  std::placeholders::_1) }
    })
{
    utils::find_or_create_domain_participant(domain_id, mParticipant);

    utils::find_or_create_topic<hvac_msgs::msg::Ac>(
            mParticipant, mTopicAc, hvac_msgs::msg::TOPIC_AC);

    ::dds::pub::qos::PublisherQos defaultPublisherQos = mParticipant.default_publisher_qos();
    ::dds::pub::qos::DataWriterQos acDataWriterQos(mTopicAc.qos());

    utils::create_datawriter<hvac_msgs::msg::Ac>(
            mAcWriter,
            mParticipant,
            mTopicAc,
            acDataWriterQos,
            defaultPublisherQos);
}


HvacModule::~HvacModule() {
}

void HvacModule::parseCan0x334(const uint8_t * data) {
    parseAcFromCAN(data);
}

const std::unordered_map<uint32_t, ParseFrame>& HvacModule::getParseMapFromCan() {
    return mParseMapFromCan;
}

void HvacModule::parseAcFromCAN(const uint8_t * data) {
    std::cout << "HvacModule::parseAcFromCAN in " << data << std::endl;
    // module -> 多个CANID ， 本module内所有功能 -> canid->signals 
    //这个canid之内的信号与功能的对应关系， 例如 mGlobalAc 对应的 startbit,length,scale,offset
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

    std::cout << "HvacModule::parseAcFromCAN before write" << std::endl;
    mAcWriter->write(mGlobalAc);
    std::cout << "HvacModule::parseAcFromCAN after write" << std::endl;
}

std_msgs::srv::Response HvacModule::setAcFromDdsRequest(hvac_msgs::srv::SetAcRequest request)
{
    std::cout << " HvacModule::setAcFromDdsRequest thread id: " << std::this_thread::get_id() << std::endl;

    bool on = request.on();
    std::cout << __func__ << " in, ac: " << on << std::endl;

    //下发CAN ，加入timeout map ， 加入检测CAN 对应数据， 然后看哪个先reply
    //功能对应到 信号集合， 大概率为同一ID下面的1个或多个信号
    //小概率为多个ID多个信号
    //对于这个setac 就是   set ac&zone   <->  canid & start bit & length & offset
    //
    //void canMessageAdapter->write(uint32_t canid, uint8_t startbit, uint8_t len, uint64_t value);

}




}//namespace hvac
}//namespace dds
}//namespace dashboard
