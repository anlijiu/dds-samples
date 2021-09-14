#ifndef __HEADSET_DDS_REQUEST_REQUESTER__
#define __HEADSET_DDS_REQUEST_REQUESTER__

#include "DdsUtils.hpp"
#include "request/RequesterParams.hpp"

namespace headunit {
namespace dds {
namespace request {
namespace detail {


template <typename T>
::dds::topic::Topic<T> get_requester_request_topic(
    const RequesterParams& params);

template <typename T>
::dds::topic::Topic<T> get_requester_reply_topic(
    const RequesterParams& params)
{

    ::dds::topic::Topic<T> topic = ::dds::core::null;

    utils::find_or_create_topic<T>(
        params.participant(),
        topic,
        params.reply_topic_nama());


    return topic;
}

/**
 */
template <typename RequestType, typename ReplyType>
class RequesterImpl : private ParamsValidator {
public:

    // Creates the sender and receiver with the appropriate topics that make
    // a Requester.
    explicit RequesterImpl(const RequesterParams& params)
        : ParamsValidator(params),
          sender_(
              params,
              get_requester_request_topic<RequestType>(params),
              "Requester"),
          receiver_(
              params,
              // The receiver's topic is a CFT that filter replies based on the
              // correlation with requests
              get_requester_reply_topic<ReplyType>(
                  params),
              "Requester")
    {
    }

    void close()
    {
        sender_.close();
        receiver_.close();
    }

    void send_request(const RequestType& data)
    {
        sender_.write(data);
    }

    ::dds::sub::LoanedSamples<ReplyType> receive_replies(
        int min_count,
        const dds::core::Duration& max_wait)
    {
        return receiver_.receive_samples(min_count, max_wait);
    }

    ::dds::sub::LoanedSamples<ReplyType> take_replies()
    {
        return receiver_.take_samples();
    }

    ::dds::sub::LoanedSamples<ReplyType> read_replies()
    {
        return receiver_.read_samples();
    }

    ::dds::pub::DataWriter<RequestType> request_datawriter() const
    {
        return sender_.writer();
    }

    ::dds::sub::DataReader<ReplyType> reply_datareader() const
    {
        return receiver_.reader();
    }

    bool wait_for_replies(int min_count, const dds::core::Duration& max_wait)
    {
        return receiver_.wait_for_any_sample(min_count, max_wait);
    }

    bool closed() const
    {
        return receiver_.closed();
    }

private:
    detail::GenericSender<RequestType> sender_;
    detail::GenericReceiver<ReplyType> receiver_;
};


template <typename T>
::dds::topic::Topic<T> get_requester_request_topic(
    const RequesterParams& params)
{

    ::dds::topic::Topic<T> topic = ::dds::core::null;

    utils::find_or_create_topic<T>(
        params.participant(),
        topic,
        params.request_topic_name());

    return topic;
}


} // namespace detail
} // namespace request
} // namespace dds
} // namespace headunit

#endif // __HEADSET_DDS_REQUEST_REQUESTER__
