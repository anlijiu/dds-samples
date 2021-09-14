#ifndef __HEADUNIT_DDS_REQUEST_REQUESTERPARAMS_HPP__
#define __HEADUNIT_DDS_REQUEST_REQUESTERPARAMS_HPP__

// IMPORTANT: macros.hpp must be the first RTI header included in every header
// file so that symbols are exported correctly on Windows
#include <dds/core/macros.hpp>

#include "request/detail/EntityParams.hpp"

namespace headunit {
namespace dds {
namespace request {

/**
 * \dref_RequesterParams
 */
class RequesterParams
    : public detail::EntityParamsWithSetters<RequesterParams> {
public:
    typedef detail::EntityParamsWithSetters<RequesterParams> Base;

    /**
     * \dref_RequesterParams_new
     */
    explicit RequesterParams(dds::domain::DomainParticipant participant)
        : Base(participant)
    {
    }

    ~RequesterParams()
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
     * \dref_RequesterParams_service_name
     */
    RequesterParams& service_name(const std::string& name);

    /**
     * \dref_RequesterParams_request_topic_name
     */
    RequesterParams& request_topic_name(const std::string& name);

    /**
     * \dref_RequesterParams_reply_topic_name
     */
    RequesterParams& reply_topic_name(const std::string& name);

    /**
     * \dref_RequesterParams_datawriter_qos
     */
    RequesterParams& datawriter_qos(const dds::core::optional<dds::pub::qos::DataWriterQos>& qos);

    /**
     * \dref_RequesterParams_datareader_qos
     */
    RequesterParams& datareader_qos(const dds::core::optional<dds::sub::qos::DataReaderQos>& qos);

    /**
     * \dref_RequesterParams_publisher
     */
    RequesterParams& publisher(dds::pub::Publisher publisher);

    /**
     * \dref_RequesterParams_subscriber
     */
    RequesterParams& subscriber(dds::sub::Subscriber subscriber);

    /**
     * @brief The request type, when DynamicData is used
     *
     * This is required when the Requester RequestType is dds::core::xtypes::DynamicData.
     * Otherwise this is ignored.
     */
    RequesterParams& request_type(const dds::core::optional<dds::core::xtypes::DynamicType>& type);

    /**
     * @brief The reply type, when DynamicData is used
     *
     * This is required when the Requester ReplyType is dds::core::xtypes::DynamicData.
     * Otherwise this is ignored.
     */
    RequesterParams& reply_type(const dds::core::optional<dds::core::xtypes::DynamicType>& type);
#endif
};

}
}
}

#endif // __HEADUNIT_DDS_REQUEST_REQUESTERPARAMS_HPP__
