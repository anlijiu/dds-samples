/*
(c) Copyright, Real-Time Innovations, 2016-2017.
All rights reserved.

No duplications, whole or partial, manual or electronic, may be made
without express written permission. Any such copies, or
revisions thereof, must display this notice unaltered.
This code contains trade secrets of Real-Time Innovations, Inc.

 */

#ifndef RTI_REQUEST_REPLIERPARAMS_HPP_
#define RTI_REQUEST_REPLIERPARAMS_HPP_

// IMPORTANT: macros.hpp must be the first RTI header included in every header
// file so that symbols are exported correctly on Windows
#include <dds/core/macros.hpp>

#include <rti/request/detail/EntityParams.hpp>

namespace rti { namespace request {

/**
 * \dref_ReplierParams
 */
class ReplierParams
    : public detail::EntityParamsWithSetters<ReplierParams> {
public:

    typedef detail::EntityParamsWithSetters<ReplierParams> Base;

    /**
     * \dref_ReplierParams_new
     */
    explicit ReplierParams(dds::domain::DomainParticipant participant)
        : Base(participant)
    {
    }

    ~ReplierParams()
    {
    }

    virtual void validate() const
    {
        Base::validate();

        if (service_name_.empty()) {
            if (request_topic_name_.empty() || reply_topic_name_.empty()) {
                throw dds::core::InvalidArgumentError(
                        "either service name or topic names are required");
            }
        }
    }

#ifdef DOXYGEN_DOCUMENTATION_ONLY
    /**
     * \dref_ReplierParams_service_name
     */
    ReplierParams& service_name(const std::string& name);

    /**
     * \dref_ReplierParams_request_topic_name
     */
    ReplierParams& request_topic_name(const std::string& name);

    /**
     * \dref_ReplierParams_reply_topic_name
     */
    ReplierParams& reply_topic_name(const std::string& name);

    /**
     * \dref_ReplierParams_datawriter_qos
     */
    ReplierParams& datawriter_qos(const dds::core::optional<dds::pub::qos::DataWriterQos>& qos);

    /**
     * \dref_ReplierParams_datareader_qos
     */
    ReplierParams& datareader_qos(const dds::core::optional<dds::sub::qos::DataReaderQos>& qos);

    /**
     * \dref_ReplierParams_publisher
     */
    ReplierParams& publisher(dds::pub::Publisher publisher);

    /**
     * \dref_ReplierParams_subscriber
     */
    ReplierParams& subscriber(dds::sub::Subscriber subscriber);

    /**
     * @brief The request type, when DynamicData is used
     *
     * This is required when the Replier RequestType is dds::core::xtypes::DynamicData.
     * Otherwise this is ignored.
     */
    ReplierParams& request_type(const dds::core::optional<dds::core::xtypes::DynamicType>& type);

    /**
     * @brief The reply type, when DynamicData is used
     *
     * This is required when the Replier ReplyType is dds::core::xtypes::DynamicData.
     * Otherwise this is ignored.
     */
    ReplierParams& reply_type(const dds::core::optional<dds::core::xtypes::DynamicType>& type);
#endif
};

} }

#endif // RTI_REQUEST_REPLIERPARAMS_HPP_
