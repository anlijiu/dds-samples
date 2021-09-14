/*
(c) Copyright, Real-Time Innovations, 2016-2017.
All rights reserved.

No duplications, whole or partial, manual or electronic, may be made
without express written permission. Any such copies, or
revisions thereof, must display this notice unaltered.
This code contains trade secrets of Real-Time Innovations, Inc.

 */

#ifndef RTI_REQUEST_SIMPLEREPLIER_HPP_
#define RTI_REQUEST_SIMPLEREPLIER_HPP_

#include <rti/request/detail/ReplierImpl.hpp>

namespace rti { namespace request {

/**
 * @ingroup ReplierModule
 * @headerfile rti/request/SimpleReplier.hpp
 * @brief @st_ref_type A callback-based replier
 *
 * @note A SimpleReplier provides all the functions of a @st_ref_type except
 * close() and retain().
 * 
 * @details \dref_details_SimpleReplier
 *
 * In the following example, the SimpleReplier responds to requests consisting
 * of the built-in type dds::core::StringTopicType with another string that
 * prepends "Hello" to the request.
 *
 * \code
 * SimpleReplier<StringTopicType, StringTopicType> simple_replier(
 *     participant,
 *     "Test",
 *     [](const StringTopicType& request) {
 *         // If a Requester sends "World", this SimpleReplier will
 *         // reply with "Hello World"
 *         return StringTopicType("Hello " + request.data());
 *     }
 * );
 * \endcode
 *
 */
template <typename RequestType, typename ReplyType>
class SimpleReplier
    : public dds::core::Reference< detail::ReplierImpl<RequestType, ReplyType> > {
public:

    typedef detail::ReplierImpl<RequestType, ReplyType> Delegate;
    typedef dds::core::Reference<Delegate> Base;
    typedef ReplierListener<RequestType, ReplyType> Listener;
    typedef typename dds::core::smart_ptr_traits<Delegate >::ref_type ref_type;
    typedef typename dds::core::smart_ptr_traits<Delegate >::weak_ref_type weak_ref_type;


    OMG_DDS_REF_TYPE(SimpleReplier, dds::core::Reference, Delegate);
    /**
     * @brief Creates a SimpleReplier
     *
     * @tparam Functor A function or function object that can receive a single
     * parameter const RequestType& and returns a ReplyType instance by value.
     *
     * @param participant The DomainParticipant that the Replier uses to join a domain.
     * @param service_name The service name. See \idref_ReplierParams_service_name
     * @param request_handler A functor that receives a request and returns
     * a reply
     */
    template <typename Functor>
    SimpleReplier(
            dds::domain::DomainParticipant participant,
            const std::string& service_name,
            Functor request_handler)
        : Base(new Delegate(
                ReplierParams(participant).service_name(service_name),
                &dummy_replier_listener_))
    {
        this->delegate()->initialize(
                ReplierParams(participant).service_name(service_name),
                new ListenerAdapter<Functor>(this->delegate(), request_handler));
        this->delegate()->remember_reference(this->delegate());
    }

    /**
     * @brief Creates a SimpleReplier with additional parameters
     *
     * @tparam Functor A function or function object that can receive a single
     * parameter const RequestType& and returns a ReplyType instance by value.
     *
     * @param params The parameters used to configure the replier
     * @param request_handler A functor that receives a request and returns
     * a reply
     */
    template <typename Functor>
    SimpleReplier(
            const ReplierParams& params,
            Functor request_handler)
        : Base(new Delegate(params, &dummy_replier_listener_))
    {
        this->delegate()->initialize(
                params,
                new ListenerAdapter<Functor>(this->delegate(), request_handler));
        this->delegate()->remember_reference(this->delegate());
    }


    dds::sub::DataReader<RequestType> request_datareader() const
    {
        return this->delegate()->request_datareader();
    }

    dds::pub::DataWriter<ReplyType> reply_datawriter() const
    {
        return this->delegate()->reply_datawriter();
    }

private:

    // As a ReaderListener it receives the requests, calls the user
    // request handler to obtain a reply for each request, and sends it.
    template <typename Functor>
    class ListenerAdapter : public dds::sub::NoOpDataReaderListener<RequestType> {
    public:
        void on_data_available(dds::sub::DataReader<RequestType>& reader) // override
        {
            typedef dds::sub::LoanedSamples<RequestType> LoanedRequests;

            ref_type replier = replier_weak_ref_.lock();
            if (!replier) {
                return;
            }

            LoanedRequests requests = reader.take();
            for (typename LoanedRequests::const_iterator it = requests.begin();
                    it != requests.end();
                    ++it) {
                if (!it->info().valid()) {
                    continue;
                }

                try {
                    ReplyType reply = request_handler_(it->data());
                    replier->send_reply(
                            reply,
                            it->info()->original_publication_virtual_sample_identity());
                } catch (...) {
                    // an exception in the user handler--ignore this request
                }
            }
        }

        ListenerAdapter(ref_type replier, Functor request_handler)
            : replier_weak_ref_(replier), request_handler_(request_handler)
        {
        }

    private:
        weak_ref_type replier_weak_ref_;
        Functor request_handler_;
    };

    class DummyReplierListener : public ReplierListener<RequestType, ReplyType> {
        void on_request_available(Replier<RequestType, ReplyType>&)
        {
            // nothing to do
        }
    };

    DummyReplierListener dummy_replier_listener_;
};

} } // namespace rti::request

#endif // RTI_REQUEST_SIMPLEREPLIER_HPP_
