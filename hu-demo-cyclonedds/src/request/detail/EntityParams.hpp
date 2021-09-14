/*
(c) Copyright, Real-Time Innovations, 2016-2017.
All rights reserved.

No duplications, whole or partial, manual or electronic, may be made
without express written permission. Any such copies, or
revisions thereof, must display this notice unaltered.
This code contains trade secrets of Real-Time Innovations, Inc.

 */

#ifndef RTI_REQUEST_DETAIL_ENTITYPARAMS_HPP_
#define RTI_REQUEST_DETAIL_ENTITYPARAMS_HPP_

// IMPORTANT: macros.hpp must be the first RTI header included in every header
// file so that symbols are exported correctly on Windows
#include <dds/core/macros.hpp>
#include <dds/core/Optional.hpp>
#include <dds/core/xtypes/DynamicType.hpp>
#include <dds/domain/DomainParticipant.hpp>
#include <dds/sub/Subscriber.hpp>
#include <dds/sub/qos/DataReaderQos.hpp>
#include <dds/pub/Publisher.hpp>
#include <dds/pub/qos/DataWriterQos.hpp>

namespace headunit {
namespace dds {
namespace request {

class EntityParams {
public:
    explicit EntityParams(dds::domain::DomainParticipant participant)
        : participant_(participant),
          publisher_(dds::core::null),
          subscriber_(dds::core::null)
    {
    }

    virtual ~EntityParams()
    {
    }

    // --- Getters: -----------------------------------------------------------

    dds::domain::DomainParticipant participant() const
    {
        return participant_;
    }

    const std::string& service_name() const
    {
        return service_name_;
    }

    const std::string& request_topic_name() const
    {
        return request_topic_name_;
    }

    const std::string& reply_topic_name() const
    {
        return reply_topic_name_;
    }

    const dds::core::optional<dds::core::xtypes::DynamicType>& request_type() const
    {
        return request_type_;
    }

    const dds::core::optional<dds::core::xtypes::DynamicType>& reply_type() const
    {
        return reply_type_;
    }

    const dds::core::optional<dds::pub::qos::DataWriterQos>& datawriter_qos() const
    {
        return datawriter_qos_;
    }

    const dds::core::optional<dds::sub::qos::DataReaderQos>& datareader_qos() const
    {
        return datareader_qos_;
    }

    dds::pub::Publisher publisher() const
    {
        return publisher_;
    }

    dds::sub::Subscriber subscriber() const
    {
        return subscriber_;
    }
    
    virtual void validate() const 
    {
        if (participant_ == dds::core::null) {
            throw dds::core::InvalidArgumentError("null participant");
        }

        if (subscriber_ != dds::core::null) {
            if (subscriber_.participant() != participant_) {
                throw dds::core::InvalidArgumentError(
                        "The subscriber belongs to a different participant");
            }
        }
        if (publisher_ != dds::core::null) {
            if (publisher_.participant() != participant_) {
                throw dds::core::InvalidArgumentError(
                        "The publisher belongs to a different participant");
            }
        }
        
        if (!service_name_.empty()) {
            if(!request_topic_name_.empty() || !reply_topic_name_.empty())
                throw dds::core::InvalidArgumentError(
                        "service name and topic names cannot be set at the same time");
        } // see RequesterParams and ReplierParams for more
    }

    bool operator==(const EntityParams& other) const
    {
        if (participant_ != other.participant_) {
            return false;
        }
        if (service_name_ != other.service_name_) {
            return false;
        }
        if (request_topic_name_ != other.request_topic_name_) {
            return false;
        }
        if (reply_topic_name_ != other.reply_topic_name_) {
            return false;
        }
        if (request_type_ != other.request_type_) {
            return false;
        }
        if (reply_type_ != other.reply_type_) {
            return false;
        }
        if (datawriter_qos_ != other.datawriter_qos_) {
            return false;
        }
        if (datareader_qos_ != other.datareader_qos_) {
            return false;
        }
        if (publisher_ != other.publisher_) {
            return false;
        }
        if (subscriber_ != other.subscriber_) {
            return false;
        }

        return true;
    }

    bool operator!=(const EntityParams& other) const
    {
        return !(*this == other);
    }

    friend void swap(EntityParams& a, EntityParams& b) OMG_NOEXCEPT
    {
        using std::swap;

        swap(a.participant_, b.participant_);
        swap(a.service_name_, b.service_name_);
        swap(a.request_topic_name_, b.request_topic_name_);
        swap(a.reply_topic_name_, b.reply_topic_name_);
        swap(a.request_type_, b.request_type_);
        swap(a.reply_type_, b.reply_type_);
        swap(a.datawriter_qos_, b.datawriter_qos_);
        swap(a.datareader_qos_, b.datareader_qos_);
        swap(a.publisher_, b.publisher_);
        swap(a.subscriber_, b.subscriber_);
    }

protected:
    dds::domain::DomainParticipant participant_;

    std::string service_name_;
    std::string request_topic_name_;
    std::string reply_topic_name_;

    dds::core::optional<dds::core::xtypes::DynamicType> request_type_;
    dds::core::optional<dds::core::xtypes::DynamicType> reply_type_;

    dds::core::optional<dds::pub::qos::DataWriterQos> datawriter_qos_;
    dds::core::optional<dds::sub::qos::DataReaderQos> datareader_qos_;
    
    dds::pub::Publisher publisher_;
    dds::sub::Subscriber subscriber_;
};

template <typename ActualEntity>
class EntityParamsWithSetters : public EntityParams {
public:

    explicit EntityParamsWithSetters(dds::domain::DomainParticipant participant)
        : EntityParams(participant)
    {
    }

    using EntityParams::validate;

    // --- Getters: -----------------------------------------------------------

    using EntityParams::participant;
    using EntityParams::service_name;
    using EntityParams::request_topic_name;
    using EntityParams::reply_topic_name;
    using EntityParams::request_type;
    using EntityParams::reply_type;
    using EntityParams::datawriter_qos;
    using EntityParams::datareader_qos;
    using EntityParams::publisher;
    using EntityParams::subscriber;

    // --- Setters: -----------------------------------------------------------

    ActualEntity& participant(dds::domain::DomainParticipant participant)
    {
        participant_ = participant;
        return static_cast<ActualEntity&>(*this);
    }

    /**
     * \dref_DerivedClass_service_name
     */
    ActualEntity& service_name(const std::string& name)
    {
        service_name_ = name;
        return static_cast<ActualEntity&>(*this);
    }

    /**
     * \dref_DerivedClass_request_topic_name
     */
    ActualEntity& request_topic_name(const std::string& name)
    {
        request_topic_name_ = name;
        return static_cast<ActualEntity&>(*this);
    }

    /**
     * \dref_DerivedClass_reply_topic_name
     */
    ActualEntity& reply_topic_name(const std::string& name)
    {
        reply_topic_name_ = name;
        return static_cast<ActualEntity&>(*this);
    }

    /**
     * \dref_DerivedClass_datawriter_qos
     */
    ActualEntity& datawriter_qos(const dds::core::optional<dds::pub::qos::DataWriterQos>& qos)
    {
        datawriter_qos_ = qos;
        return static_cast<ActualEntity&>(*this);
    }

    /**
     * \dref_DerivedClass_datareader_qos
     */
    ActualEntity& datareader_qos(const dds::core::optional<dds::sub::qos::DataReaderQos>& qos)
    {
        datareader_qos_ = qos;
        return static_cast<ActualEntity&>(*this);
    }

    /**
     * \dref_DerivedClass_requester_type
     */
    ActualEntity& request_type(const dds::core::optional<dds::core::xtypes::DynamicType>& type)
    {
        request_type_ = type;
        return static_cast<ActualEntity&>(*this);
    }

    /**
     * \dref_DerivedClass_reply_type
     */
    ActualEntity& reply_type(const dds::core::optional<dds::core::xtypes::DynamicType>& type)
    {
        reply_type_ = type;
        return static_cast<ActualEntity&>(*this);
    }

    /**
     * \dref_DerivedClass_publisher
     */
    ActualEntity& publisher(dds::pub::Publisher publisher)
    {
        publisher_ = publisher;
        return static_cast<ActualEntity&>(*this);
    }

    /**
     * \dref_DerivedClass_subscriber
     */
    ActualEntity& subscriber(dds::sub::Subscriber subscriber)
    {
        subscriber_ = subscriber;
        return static_cast<ActualEntity&>(*this);
    }
};

template <typename TargetEntity, typename SourceEntity>
void convert_params(
    EntityParamsWithSetters<TargetEntity>& target,
    const EntityParamsWithSetters<SourceEntity>& source)
{
    target.datareader_qos(source.datareader_qos());
    target.datawriter_qos(source.datawriter_qos());
    target.participant(source.participant());
    target.publisher(source.publisher());
    target.reply_topic_name(source.reply_topic_name());
    target.reply_type(source.reply_type());
    target.request_topic_name(source.request_topic_name());
    target.request_type(source.request_type());
    target.service_name(source.service_name());
    target.subscriber(source.subscriber());
}

// Helper class that can be used as a private base of another class C so that
// C's constructors can validate the EntityParams before creating any other
// member
class ParamsValidator {
public:
    ParamsValidator(const EntityParams& params)
    {
        params.validate();
    }

    ParamsValidator(const EntityParams& params, void *non_null_listener)
    {
        params.validate();
        if (non_null_listener == NULL) {
            throw dds::core::InvalidArgumentError(
                    "listener argument cannot be NULL");
        }
    }
};

} // namespace request
} // namespace dds
} // namespace headunit

#endif // RTI_REQUEST_DETAIL_ENTITYPARAMS_HPP_
