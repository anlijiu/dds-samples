#ifndef __hvac_msgs_AcPublisher__
#define __hvac_msgs_AcPublisher__

#include "hvac_msgs/msg/Ac.hpp"
#include <dds/pub/DataWriterListener.hpp>

namespace hvac {
namespace msg {

class AcPublisher : public dds::pub::DataWriterListener<hvac_msgs::msg::Ac> {
private:
    std::unique_ptr<dds::pub::DataWriter<hvac_msgs::msg::Ac>> writer_;
    hvac_msgs::msg::Ac sample_;
public:
    AcPublisher(DDS_DomainId_t domain_id);
    void parseCanFrameForGlobal(const uint8_t * data);
    ~AcPublisher();

protected:
    void on_offered_deadline_missed(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>& writer
        ,const dds::core::status::OfferedDeadlineMissedStatus& status) override;

    void on_offered_incompatible_qos(dds::pub::DataWriter<hvac_msgs::msg::Ac>& writer
        ,const dds::core::status::OfferedIncompatibleQosStatus& status) override;

    void on_liveliness_lost(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>& writer
        ,const dds::core::status::LivelinessLostStatus& status) override;

    void on_publication_matched(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>& writer
        ,const dds::core::status::PublicationMatchedStatus& status) override;

    void on_reliable_writer_cache_changed(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>& writer,
        const rti::core::status::ReliableWriterCacheChangedStatus& status) override;

    void on_reliable_reader_activity_changed(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>& writer
        ,const rti::core::status::ReliableReaderActivityChangedStatus& status) override;

    void on_instance_replaced(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>& writer
        , const dds::core::InstanceHandle& handle) override;

    void on_application_acknowledgment(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>& writer,
        const rti::pub::AcknowledgmentInfo& acknowledgment_info) override;

    void on_service_request_accepted(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>& writer
        ,const rti::core::status::ServiceRequestAcceptedStatus& status) override;

    void on_destination_unreachable(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>&
        ,const dds::core::InstanceHandle&
        ,const rti::core::Locator&) override;

    void* on_data_request(dds::pub::DataWriter<hvac_msgs::msg::Ac>&
        ,const rti::core::Cookie&) override;

    void on_data_return(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>&
        ,void*
        ,const rti::core::Cookie&) override;

    void on_sample_removed(
        dds::pub::DataWriter<hvac_msgs::msg::Ac>& writer
        ,const rti::core::Cookie& cookie) override;
};



}// msg
}// hvac

#endif //__hvac_msgs_AcPublisher__
