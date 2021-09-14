#ifndef __HEADSET_DDS_REQUEST_REPLIER_HPP__
#define __HEADSET_DDS_REQUEST_REPLIER_HPP__

#include <detail/ReplierImpl.hpp>

namespace headunit {
namespace dds {
namespace request {

template <typename RequestType, typename ReplyType>
class Replier : public ::dds::core::Reference< detail::ReplierImpl<RequestType, ReplyType> > {
public:

    typedef detail::ReplierImpl<RequestType, ReplyType> Delegate;
    typedef ::dds::core::Reference<Delegate> Base;
    typedef ReplierListener<RequestType, ReplyType> Listener;
    typedef typename ::dds::core::smart_ptr_traits<Delegate >::ref_type ref_type;
    typedef typename ::dds::core::smart_ptr_traits<Delegate >::weak_ref_type weak_ref_type;


    OMG_DDS_REF_TYPE(Replier, ::dds::core::Reference, Delegate);

    /**
     * \dref_Replier_new
     */
    Replier(
        dds::domain::DomainParticipant participant,
        const std::string& service_name)
        : Base(new Delegate(
            ReplierParams(participant).service_name(service_name)))
    {
        this->delegate()->remember_reference(this->delegate());
    }

    /**
     * \dref_Replier_new_w_params
     */
    explicit Replier(const ReplierParams& params)
        : Base(new Delegate(params))
    {
        this->delegate()->remember_reference(this->delegate());
    }

    Replier(const ReplierParams& params, Listener *the_listener)
        : Base(new Delegate(params, the_listener))
    {
        // Two-phase initialization. The constructor (Base) partially initializes
        // the delegate, but to complete it we need to pass a ListenerAdapter
        // that requires a reference to delegate itself, which has just been
        // constructed.
        this->delegate()->initialize(
                params,
                new ListenerAdapter(this->delegate()));
        this->delegate()->remember_reference(this->delegate());
        this->delegate()->retain();
    }

#ifndef RTI_NO_CXX11_SMART_PTR

    /**
     * @brief Creates a replier with additional parameters and a listener
     *
     * @param params All the parameters that configure this Replier
     * @param the_listener A ReplierListener that notifies when new requests
     * are available. Cannot be nullptr, but the parameter can be omitted.
     */
    Replier(const ReplierParams& params, std::shared_ptr<Listener> the_listener)
            : Base(new Delegate(params, the_listener))
    {
        // Two-phase initialization. The constructor (Base) partially
        // initializes the delegate, but to complete it we need to pass a
        // ListenerAdapter that requires a reference to delegate itself, which
        // has just been constructed.
        this->delegate()->initialize(
                params,
                new ListenerAdapter(this->delegate()));
        this->delegate()->remember_reference(this->delegate());
    }

#endif

    // For internal use only
    Replier(ref_type delegate_ref)
        : Base(delegate_ref)
    {
        this->delegate()->remember_reference(this->delegate());
    }

    void close()
    {
        this->delegate()->close();
    }

    /**
     * @brief Sends a reply
     *
     * This operation obtains the related_request_id from a request SampleInfo,
     * that is, it's equivalent to send_reply(reply, related_request_info->original_publication_virtual_sample_identity())
     */
    void send_reply(
        const ReplyType& reply,
        const dds::sub::SampleInfo& related_request_info)
    {
        this->delegate()->send_reply(
                reply,
                related_request_info->original_publication_virtual_sample_identity());
    }

    /**
     * \dref_Replier_wait_for_requests_simple
     */
    bool wait_for_requests(const ::dds::core::Duration& max_wait)
    {
        return this->delegate()->wait_for_requests(1, max_wait);
    }

    /**
     * \dref_Replier_wait_for_requests
     */
    bool wait_for_requests(int min_count, const ::dds::core::Duration& max_wait)
    {
        return this->delegate()->wait_for_requests(min_count, max_wait);
    }

    /**
     * \dref_Replier_receive_requests_simple
     */
    dds::sub::LoanedSamples<RequestType> receive_requests(
        const ::dds::core::Duration& max_wait)
    {
        return this->delegate()->receive_requests(1, max_wait);
    }

    /**
     * \dref_Replier_receive_requests
     */
    dds::sub::LoanedSamples<RequestType> receive_requests(
        int min_count,
        const ::dds::core::Duration& max_wait)
    {
        return this->delegate()->receive_requests(min_count, max_wait);
    }

    /**
     * @brief Takes all the requests
     *
     * @details \dref_details_Replier_take_requests
     */
    dds::sub::LoanedSamples<RequestType> take_requests()
    {
        return this->delegate()->take_requests();
    }

    /**
     * @brief Reads all the requests
     *
     * @details \dref_details_Replier_read_requests
     */
    dds::sub::LoanedSamples<RequestType> read_requests()
    {
        return this->delegate()->read_requests();
    }

    /**
     * @brief Gets the replier listener
     *
     * @deprecated Prefer `get_listener()` instead of this function.
     */
    Listener* listener() const
    {
        return this->delegate()->listener_impl();
    }

    /**
     * @brief Sets a listener to be notified when requests are available
     *
     * @deprecated The use of `set_listener()` is recommended. Unlike this
     * function, `set_listener` receives a `shared_ptr` which simplifies the
     * management of listener's lifecycle.
     *
     * The listener will be called when new requests are available.
     *
     * @note rti::core::bind_listener() is recommended instead of this listener
     * setter. bind_listener() and bind_and_manage_listener() automatically
     * remove the listener, which allows the Replier to be automatically
     * destroyed.
     *
     * (The second parameter is ignored)
     */
    void listener(
        Listener* the_listener,
        const ::dds::core::status::StatusMask& = ::dds::core::status::StatusMask::none())
    {
        if (the_listener != NULL && !this->delegate()->has_listener_adapter()) {
            /* Before the ReplierImpl can hold a listener, it needs a listener
             * adapter. The listener base class definition needs to be here
             * because its callback takes a Replier as an argument.
             */
            this->delegate()->listener_adapter(new ListenerAdapter(this->delegate()));
        }
        this->delegate()->listener_impl(the_listener);
    }

#ifndef RTI_NO_CXX11_SMART_PTR
    /**
     * @brief Gets the replier listener
     *
     * The listener will be called when new requests are available
     */
    std::shared_ptr<Listener> get_listener() const
    {
        return this->delegate()->get_listener_impl();
    }

    /**
     * @brief Sets a listener to be notified when requests are available
     *
     * @warning It's recommended that the listener implementation doesn't hold
     * a permanent reference to the replier. If it does, the application needs
     * to manually reset the listener (`set_listener(nullptr)`) to ensure
     * that there is no cycle that prevents the destruction of these two
     * objects.
     *
     * @param the_listener A shared pointer to the listener to receive
     * updates or `nullptr` to reset the current listener and stop receiving
     * updates.
     */
    void set_listener(std::shared_ptr<Listener> the_listener)
    {
        if (the_listener && !this->delegate()->has_listener_adapter()) {
            /* Before the ReplierImpl can hold a listener, it needs a listener
             * adapter. The listener base class definition needs to be here
             * because its callback takes a Replier as an argument.
             */
            this->delegate()->listener_adapter(
                    new ListenerAdapter(this->delegate()));
        }
        this->delegate()->set_listener_impl(the_listener);
    }
#endif

    /**
     * \dref_Replier_get_request_datareader
     */
    ::dds::sub::DataReader<RequestType> request_datareader() const
    {
        return this->delegate()->request_datareader();
    }

    /**
     * \dref_Replier_get_reply_datawriter
     */
    ::dds::pub::DataWriter<ReplyType> reply_datawriter() const
    {
        return this->delegate()->reply_datawriter();
    }

private:
    class ListenerAdapter : public ::dds::sub::NoOpDataReaderListener<RequestType> {
    public:
        void on_data_available(::dds::sub::DataReader<RequestType>&) // override
        {
            ref_type delegate_ref = requester_weak_ref_.lock();
            if (delegate_ref) {
                Replier<RequestType, ReplyType> replier(delegate_ref);
                Listener* listener = replier.delegate()->listener_impl();
                if (listener != NULL) {
                    listener->on_request_available(replier);
                }
            }
        }

        ListenerAdapter(ref_type requester)
            : requester_weak_ref_(requester)
        {
        }

    private:
        weak_ref_type requester_weak_ref_;
    };
};

/**
 * @brief Obtains the number of Requesters that match with a Replier
 *
 * @note This operation requires that the requesters' data writer and data reader
 * role name (in rti::core::policy::EntityName) has not been changed.
 */
template <typename RequestType, typename ReplyType>
size_t matched_requester_count(Replier<RequestType, ReplyType> replier)
{
    return detail::matched_count(
            *replier.request_datareader().delegate(),
            *replier.reply_datawriter().delegate(),
            "Requester");
}

} // namespace request
} // namespace dds
} // namespace headunit

#endif // __HEADSET_DDS_REQUEST_REPLIER_HPP__
