namespace dds {
namespace request {

template <typename RequestType, typename ReplyType>
class Requester {
public:
    class ReplyListener : public virtual ::dds::sub::NoOpDataReaderListener<ReplyType> {
    public:
        ReplyListener(
                Requester<RequestType, ReplyType> & requester)
            : requester_(requester)
        {
        }

        ~ReplyListener()
        {
        }

        void on_data_available(
            ::dds::sub::DataReader<ReplyType>& reader) override;

        void on_subscription_matched(
            ::dds::sub::DataReader<ReplyType>& reader,
            const ::dds::core::status::SubscriptionMatchedStatus& status) override {
            if (0 < info.current_count_change)
            {
                requester_.matched();
            }
        }

    private:

        ReplyListener& operator =(
                const ReplyListener&) = delete;

        Requester<RequestType, ReplyType> & requester_;
    } reply_listener_;

    class RequestListener : public ::dds::pub::NoOpDataWriterListener<RequestType>
    {
    public:

        RequestListener(
                Requester<RequestType, ReplyType>& requester)
            : requester_(requester)
        {
        }

        ~RequestListener()
        {
        }

        void on_publication_matched(
            ::dds::pub::DataWriter<T>& writer,
            const ::dds::core::status::PublicationMatchedStatus& status) override {
            if (0 < status.current_count_change())
            {
                requester_.matched();
            }
        }

    private:

        RequestListener& operator =(
                const RequestListener&) = delete;

        Requester<RequestType, ReplyType>& requester_;

    } request_listener_;

    void send_request(const RequestType& data);

    dds::sub::LoanedSamples<ReplyType> receive_replies(
            const dds::core::Duration& max_wait)

protected:
    ::dds::sub::qos::DataReaderQos datareader_qos;
    ::dds::pub::qos::DataWriterQos datawriter_qos;

    std::string datareader_topicname_;
    std::string datawriter_topicname_;

private:

    Requester& operator =(
            const Requester&) = delete;

    uint16_t current_number_;
    uint16_t number_received_;

    dds::domain::DomainParticipant participant;
    dds::topic::Topic<ReplyType> reply_topic;
    dds::topic::Topic<RequestType> request_topic;
    dds::sub::Subscriber reply_subscriber;
    dds::pub::Publisher request_publisher;
    dds::sub::DataReader<ReplyType> reply_reader;
    dds::pub::DataWriter<RequestType> request_writer;

    bool initialized_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::mutex mutexDiscovery_;
    std::condition_variable cvDiscovery_;
    unsigned int matched_;
}
    
} /* request  */     
} /* dds  */ 
