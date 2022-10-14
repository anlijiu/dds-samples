#include "DdsUtils.hpp"

static const char *simple_config_uri =
    "file:///etc/cyclonedds/config.xml";

namespace headunit {
namespace dds {
namespace utils {

bool exit_application = false;

void find_or_create_domain_participant(
        int domain_id,
        ::dds::domain::DomainParticipant & participant)
{
    participant = ::dds::domain::find(domain_id);
    if (participant == ::dds::core::null) {
        participant = ::dds::domain::DomainParticipant(
            domain_id,
            ::dds::domain::DomainParticipant::default_participant_qos(),
            0,
            ::dds::core::status::StatusMask::none(),
            simple_config_uri);
    }
}


}  // namespace utils
}  // namespace dds
}  // namespace headunit 
