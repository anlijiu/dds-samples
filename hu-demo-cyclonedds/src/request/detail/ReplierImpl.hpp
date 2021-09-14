#ifndef __HEADSET_DDS_REQUEST_DETAIL_REPLIERIMPL_HPP__
#define __HEADSET_DDS_REQUEST_DETAIL_REPLIERIMPL_HPP__

#include "request/detail/GenericSender.hpp"
#include "request/detail/GenericReceiver.hpp"
#include "request/ReplierParams.hpp"
#include "request/ReplierListener.hpp"

namespace headunit {
namespace dds {
namespace request {
namespace detail {

template <typename T>
dds::topic::TopicDescription<T> get_replier_request_topic(
    const ReplierParams& params);

template <typename T>
dds::topic::Topic<T> get_replier_reply_topic(
    const ReplierParams& params);


template <typename RequestType>
class ReplierDataReaderListenerAdapter
        : public dds::sub::NoOpDataReaderListener<RequestType> {

    void on_data_available(dds::sub::DataReader<RequestType>& reader)
    {

    }
};

/**
 */
template <typename RequestType, typename ReplyType>
class ReplierImpl : private ParamsValidator {
public:
    typedef ReplierListener<RequestType, ReplyType> Listener;

    explicit ReplierImpl(const ReplierParams& params)
        : ParamsValidator(params),
          listener_adapter_(NULL),
          listener_(nullptr),
          sender_(params, get_replier_reply_topic<ReplyType>(params), "Replier"),
          receiver_(params, get_replier_request_topic<RequestType>(params), "Replier")
    {
    }

    ReplierImpl(
            const ReplierParams& params,
            std::shared_ptr<ReplierListener<RequestType, ReplyType>> the_listener)
        : ParamsValidator(params, the_listener.get()),
          listener_adapter_(NULL),
          listener_(the_listener),
          sender_(params, get_replier_reply_topic<ReplyType>(params), "Replier")
    {
    }

    ReplierImpl(
            const ReplierParams& params,
            std::shared_ptr<Listener> the_listener)
            : ParamsValidator(params, the_listener.get()),
              listener_adapter_(NULL),
              listener_(the_listener),
              sender_(params,
                      get_replier_reply_topic<ReplyType>(params),
                      "Replier")
    {
    }

    // To be called after the constructor with a listener. See rationale in
    // Replier.hpp
    void initialize(
            const ReplierParams& params,
            dds::sub::DataReaderListener<RequestType> *the_listener_adapter)
    {
        listener_adapter(the_listener_adapter);
        receiver_.initialize(
                params,
                get_replier_request_topic<RequestType>(params),
                "Replier",
                the_listener_adapter,
                dds::core::status::StatusMask::data_available());
    }

    ~ReplierImpl()
    {
        if (closed()) {
            return;
        }

        // Listener can only be non-null here if this object was not retained
        // when the listener was set; otherwise the object can't be destroyed
        // while it holds a listener
        listener_impl(NULL, false);
        if (listener_adapter_ != NULL) {
            delete listener_adapter_;
        }
    }

    void close()
    {
        if (closed()) {
            return;
        }

        listener_impl(NULL);
        if (listener_adapter_ != NULL) {
            delete listener_adapter_;
            listener_adapter_ = NULL;
        }

        sender_.close();
        receiver_.close();
    }

    void send_reply(const ReplyType& data)
    {
        sender_.write(data);
    }

    bool wait_for_requests(int min_count, const dds::core::Duration& max_wait)
    {
        return receiver_.wait_for_any_sample(min_count, max_wait);
    }

    dds::sub::LoanedSamples<RequestType> receive_requests(
        int min_count, const dds::core::Duration& max_wait)
    {
        return receiver_.receive_samples(min_count, max_wait);
    }

    dds::sub::LoanedSamples<RequestType> take_requests()
    {
        return receiver_.take_samples();
    }

    dds::sub::LoanedSamples<RequestType> read_requests()
    {
        return receiver_.read_samples();
    }

    dds::pub::DataWriter<ReplyType> reply_datawriter() const
    {
        return sender_.writer();
    }

    dds::sub::DataReader<RequestType> request_datareader() const
    {
        return receiver_.reader();
    }

    void listener_impl(
            std::shared_ptr<ReplierListener<RequestType, ReplyType>> the_listener)
    {
        if (the_listener == NULL) {
            receiver_.listener(NULL);
            listener_ = NULL;
        } else {
            listener_ = the_listener;
            receiver_.listener(listener_adapter_);
        }
    }

    std::shared_ptr<Listener> listener_impl()
    {
        return listener_;
    }

    void set_listener_impl(std::shared_ptr<Listener> the_listener)
    {
        listener_ = the_lisener;
    }

    std::shared_ptr<Listener> get_listener_impl() const
    {
        return listener_;
    }

    // for internal use only:
    void listener_adapter(
            dds::sub::DataReaderListener<RequestType> *the_listener_adapter)
    {
        listener_adapter_ = the_listener_adapter;
    }

    bool has_listener_adapter() const
    {
        return listener_adapter_ != NULL;
    }

    bool closed() const
    {
        return receiver_.closed();
    }

private:
    dds::sub::DataReaderListener<RequestType>* listener_adapter_;
    std::shared_ptr<ReplierListener<RequestType, ReplyType>> listener_;
    detail::GenericSender<ReplyType> sender_;
    detail::GenericReceiver<RequestType> receiver_;
};


template <typename T>
dds::topic::TopicDescription<T> get_replier_request_topic(
    const ReplierParams& params)
{
    return get_or_create_topic<T>(
        params.participant(),
        get_request_topic_name(params),
        params.request_type(),
        true);
}

template <typename T>
dds::topic::Topic<T> get_replier_reply_topic(
    const ReplierParams& params)
{
    dds::topic::TopicDescription<T> topic_desc = get_or_create_topic<T>(
        params.participant(),
        get_reply_topic_name(params),
        params.reply_type(),
        false);

    return dds::core::polymorphic_cast<dds::topic::Topic<T> >(topic_desc);
}

} // namespace detail
} // namespace request
} // namespace dds
} // namespace headunit

#endif // __HEADSET_DDS_REQUEST_DETAIL_REPLIERIMPL_HPP__
