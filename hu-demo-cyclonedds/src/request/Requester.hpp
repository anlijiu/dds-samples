#ifndef __HEADSET_DDS_REQUEST_REQUESTER_HPP__
#define __HEADSET_DDS_REQUEST_REQUESTER_HPP__

#include <dds/dds.hpp>

#include "request/detail/RequesterImpl.hpp"

namespace headunit {
namespace dds {
namespace request {

template <typename RequestType, typename ReplyType>
class Requester
    : public dds::core::Reference< detail::RequesterImpl<RequestType, ReplyType> > {
public:

    typedef detail::RequesterImpl<RequestType, ReplyType> Delegate;
    typedef dds::core::Reference<Delegate> Base;


    OMG_DDS_REF_TYPE(Requester, dds::core::Reference, Delegate);

    /**
     * \dref_Requester_new_w_params
     */
    explicit Requester(const RequesterParams& params)
        : Base(new Delegate(params))
    {
        this->delegate()->remember_reference(this->delegate());
    }

    /**
     * \dref_Requester_new
     */
    Requester(
        dds::domain::DomainParticipant participant,
        const std::string& service_name)
        : Base(new Delegate(
            RequesterParams(participant).service_name(service_name)))
    {
        this->delegate()->remember_reference(this->delegate());
    }

    void close()
    {
        this->delegate()->close();
    }

    /**
     * @brief Sends a request
     *
     * @param request The request to be sent
     * @return The identity of the request, which can be used to correlate it
     * with a future reply (see \idref_Requester_wait_for_replies_w_related_request)
     *
     */
    rti::core::SampleIdentity send_request(const RequestType& request)
    {
        return this->delegate()->send_request(request);
    }


    /**
     * @brief Sends a request with advanced parameters
     *
     * @param request The request to be sent
     * @param params (in-out) The parameters to write the request (see \idref_FooDataWriter_write_w_params)
     *
     * This function allows setting the request identity in \p params.identity(),
     * among other advanced parameters. If the identity is not set, \ndds
     * automatically assigns it, and to obtain it \p params.replace_automatic_values()
     * needs to be set to true. Then, after this function ends, \p params.identity()
     * will contain the request identity. If no additional parameters need to be set, use the simpler
     * send_request(const RequestType&), which directly returns the automatically
     * assigned identity.
     */
    void send_request(const RequestType& request, rti::pub::WriteParams& params)
    {
        this->delegate()->send_request(request, params);
    }

    /**
     * \dref_Requester_receive_replies_simple
     */
    dds::sub::LoanedSamples<ReplyType> receive_replies(
            const dds::core::Duration& max_wait)
    {
        return this->delegate()->receive_replies(1, max_wait);
    }

    /**
     * \dref_Requester_receive_replies
     */
    dds::sub::LoanedSamples<ReplyType> receive_replies(
            int min_count,
            const dds::core::Duration& max_wait)
    {
        return this->delegate()->receive_replies(min_count, max_wait);
    }

    /**
     * @brief Takes all the replies
     *
     * @details \dref_details_Requester_take_replies
     */
    dds::sub::LoanedSamples<ReplyType> take_replies()
    {
        return this->delegate()->take_replies();
    }

    /**
     * @brief Reads all the replies
     *
     * @details \dref_details_Requester_read_replies
     */
    dds::sub::LoanedSamples<ReplyType> read_replies()
    {
        return this->delegate()->read_replies();
    }

    /**
     * \dref_Requester_wait_for_replies_simple
     */
    bool wait_for_replies(const dds::core::Duration& max_wait)
    {
        return this->delegate()->wait_for_replies(1, max_wait);
    }

    /**
     * \dref_Requester_wait_for_replies
     */
    bool wait_for_replies(int min_count, const dds::core::Duration& max_wait)
    {
        return this->delegate()->wait_for_replies(min_count, max_wait);
    }

    /**
     * \dref_Requester_get_request_datawriter
     */
    dds::pub::DataWriter<RequestType> request_datawriter() const
    {
        return this->delegate()->request_datawriter();
    }

    /**
     * \dref_Requester_get_reply_datareader
     */
    dds::sub::DataReader<ReplyType> reply_datareader() const
    {
        return this->delegate()->reply_datareader();
    }
};

} // namespace request
} // namespace dds
} // namespace headunit

#endif // __HEADSET_DDS_REQUEST_REQUESTER_HPP__
