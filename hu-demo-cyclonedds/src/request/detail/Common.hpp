#ifndef __REQUEST_COMMON_HPP__
#define __REQUEST_COMMON_HPP__

#include <dds/dds.hpp>

#include <dds/core/BuiltinTopicTypes.hpp>
#include <dds/topic/find.hpp>
#include <dds/topic/Topic.hpp>
#include <dds/topic/ContentFilteredTopic.hpp>
#include <dds/core/xtypes/DynamicData.hpp>
#include <dds/sub/DataReader.hpp>
#include <dds/pub/DataWriter.hpp>

#include <detail/EntityParams.hpp>

namespace headunit {
namespace dds {
namespace request {
namespace detail {

template <typename T>
bool is_content_filtered_topic(
    const dds::topic::TopicDescription<T>& topic_desc)
{
    using dds::topic::ContentFilteredTopic;

    try {
        dds::core::polymorphic_cast<ContentFilteredTopic<T> >(topic_desc);
        return true;
    } catch (const dds::core::InvalidDowncastError&) {
        return false;
    }
}

template <typename T>
dds::topic::TopicDescription<T> get_or_create_topic(
    dds::domain::DomainParticipant participant,
    const std::string& name,
    // DynamicType parameter not used in general template
    // (see DynamicData specialization)
    const dds::core::optional<dds::core::xtypes::DynamicType>&,
    bool allow_cft)
{
    using namespace dds::topic;

    TopicDescription<T> topic_desc = find<TopicDescription<T> >(
        participant, name);
    if (topic_desc != dds::core::null) {
        // Topic exists: check if we can use it
        if (!allow_cft && is_content_filtered_topic(topic_desc)) {
            throw dds::core::PreconditionNotMetError(
                "Another Topic with the same name (" + name + ") but different "
                "class (ContentFilteredTopic) exists in the same participant ");
        }

        return topic_desc;
    }

    // Topic doesn't exist: create a new one
    return Topic<T>(participant, name);
}

// Specialization for DynamicData needs parameter 'type'
template <>
XMQCPP2DllExport
dds::topic::TopicDescription<dds::core::xtypes::DynamicData> get_or_create_topic(
    dds::domain::DomainParticipant participant,
    const std::string& name,
    const dds::core::optional<dds::core::xtypes::DynamicType>& type,
    bool allow_cft);

XMQCPP2DllExport
std::string get_request_topic_name(const EntityParams& params);

XMQCPP2DllExport
std::string get_reply_topic_name(const EntityParams& params);

XMQCPP2DllExport
void configure_qos(dds::pub::qos::DataWriterQos& qos, const std::string& role_name);

XMQCPP2DllExport
void configure_qos(dds::sub::qos::DataReaderQos& qos, const std::string& role_name);

XMQCPP2DllExport
size_t matched_count(
        rti::sub::UntypedDataReader& reader,
        rti::pub::UntypedDataWriter& writer,
        const std::string& role_name);

} //detail
} //request
} //dds
} //headunit

#endif // __REQUEST_COMMON_HPP__
