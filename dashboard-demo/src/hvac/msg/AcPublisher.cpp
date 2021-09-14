
#include <dds/pub/ddspub.hpp>
#include <rti/util/util.hpp>      // for sleep()

#include <std_msgs/msg/VehicleArea.hpp>


#include "AcPublisher.hpp"
#include "libwecan.h"
#include "Can.h"

namespace hvac {
namespace msg {

AcPublisher::AcPublisher(DDS_DomainId_t domain_id) {
    dds::domain::DomainParticipant participant(domain_id);
    dds::topic::Topic<hvac_msgs::msg::Ac> topic(participant, "hvac_msg_ac");

    dds::topic::qos::TopicQos topic_qos = topic.qos();
    std::string str;
    rti::core::QosPrintFormat format;
    std::cout << to_string(topic_qos, format) << std::endl;

    // Create a Publisher
    dds::pub::Publisher publisher(participant);

    dds::pub::qos::DataWriterQos writer_qos;
    writer_qos << dds::core::policy::Reliability::Reliable() << dds::core::policy::History::KeepLast(1);


    writer_ = std::make_unique<dds::pub::DataWriter<hvac_msgs::msg::Ac>>(publisher
        , topic
        , writer_qos
        , this
        , dds::core::status::StatusMask::none());
}

AcPublisher::~AcPublisher() {
}

void AcPublisher::parseCanFrameForGlobal(const uint8_t * data) {
    uint32_t startBit = 0;
    uint32_t length = 1;
    double scaling = 1;
    double offset = 0;

    uint64_t origvalue = ::extract(data, startBit, length, UNSIGNED, MOTOROLA);
    double value = origvalue * scaling + offset;
    bool on = static_cast<bool>(value);

    std::cout << __func__ << " value:" << value << " on:" << on << " orig on:" << sample_.on();
    sample_.zone(0);
    if(on != sample_.on()) {
        sample_.on(on);
    }

    writer_->write(sample_);
}

void AcPublisher::on_offered_deadline_missed(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>& /*writer*/,
    const dds::core::status::OfferedDeadlineMissedStatus& /*status*/)
{
    //std::cout << "           on_offered_deadline_missed" << std::endl;
}
void AcPublisher::on_offered_incompatible_qos(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>& /*writer*/,
    const dds::core::status::OfferedIncompatibleQosStatus& /*status*/)
{
    //std::cout << "           on_offered_incompatible_qos" << std::endl;
}
void AcPublisher::on_liveliness_lost(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>& /*writer*/,
    const dds::core::status::LivelinessLostStatus& /*status*/)
{
    //std::cout << "           on_liveliness_lost" << std::endl;
}
void AcPublisher::on_publication_matched(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>& /*writer*/,
    const dds::core::status::PublicationMatchedStatus& /*status*/)
{
    //std::cout << "           on_publication_matched" << std::endl;
}
void AcPublisher::on_reliable_writer_cache_changed(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>& /*writer*/,
    const rti::core::status::ReliableWriterCacheChangedStatus& /*status*/)
{
    //std::cout << "           on_reliable_writer_cache_changed" << std::endl;
}
void AcPublisher::on_reliable_reader_activity_changed(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>& /*writer*/,
    const rti::core::status::ReliableReaderActivityChangedStatus& /*status*/)
{
    //std::cout << "           on_reliable_reader_activity_changed" << std::endl;
}
void AcPublisher::on_instance_replaced(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>& /*writer*/,
    const dds::core::InstanceHandle& /*handle*/)
{
    //std::cout << "           on_instance_replaced" << std::endl;
}
void AcPublisher::on_application_acknowledgment(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>& /*writer*/,
    const rti::pub::AcknowledgmentInfo& /*acknowledgment_info*/)
{
    //std::cout << "on_application_acknowledgment" << std::endl;
}
void AcPublisher::on_service_request_accepted(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>& /*writer*/,
    const rti::core::status::ServiceRequestAcceptedStatus& /*status*/)
{
    //std::cout << "           on_service_request_accepted" << std::endl;
}
void AcPublisher::on_destination_unreachable(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>&,
    const dds::core::InstanceHandle&,
    const rti::core::Locator&)
{
    //std::cout << "           on_destination_unreachable" << std::endl;
}
void* AcPublisher::on_data_request(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>&,
    const rti::core::Cookie&)
{
    //std::cout << "           on_data_request" << std::endl;
    return nullptr;
}
void AcPublisher::on_data_return(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>&,
    void*,
    const rti::core::Cookie&)
{
    //std::cout << "           on_data_return" << std::endl;
}
void AcPublisher::on_sample_removed(
    dds::pub::DataWriter<hvac_msgs::msg::Ac>& /*writer*/,
    const rti::core::Cookie& /*cookie*/)
{
    //std::cout << "           on_sample_removed" << std::endl;
}

}//namespace msg
}//namespace hvac
