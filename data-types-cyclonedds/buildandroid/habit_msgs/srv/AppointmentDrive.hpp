/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to CXX Translator
  File name: AppointmentDrive.idl
  Source: /home/an/workspace/dds/dds-samples/data-types-cyclonedds/buildandroid/habit_msgs/srv/AppointmentDrive.hpp
  Cyclone DDS: v0.10.0

*****************************************************************/
#ifndef DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_HABIT_MSGS_SRV_APPOINTMENTDRIVE_HPP
#define DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_HABIT_MSGS_SRV_APPOINTMENTDRIVE_HPP

#include <cstdint>
#include <string>

namespace habit_msgs
{
namespace srv
{
const std::string TOPIC_APPOINTMENT_DRIVE = "habit_msgs/srv/AppointmentDrive";

class AppointmentDriveRequest
{
private:
 uint64_t timestamp_ = 0;

public:
  AppointmentDriveRequest() = default;

  explicit AppointmentDriveRequest(
    uint64_t timestamp) :
    timestamp_(timestamp) { }

  uint64_t timestamp() const { return this->timestamp_; }
  uint64_t& timestamp() { return this->timestamp_; }
  void timestamp(uint64_t _val_) { this->timestamp_ = _val_; }

  bool operator==(const AppointmentDriveRequest& _other) const
  {
    (void) _other;
    return timestamp_ == _other.timestamp_;
  }

  bool operator!=(const AppointmentDriveRequest& _other) const
  {
    return !(*this == _other);
  }

};

}

}

#include "dds/topic/TopicTraits.hpp"
#include "org/eclipse/cyclonedds/topic/datatopic.hpp"

namespace org {
namespace eclipse {
namespace cyclonedds {
namespace topic {

template <> constexpr const char* TopicTraits<::habit_msgs::srv::AppointmentDriveRequest>::getTypeName()
{
  return "habit_msgs::srv::AppointmentDriveRequest";
}

template <> constexpr bool TopicTraits<::habit_msgs::srv::AppointmentDriveRequest>::isKeyless()
{
  return true;
}

#ifdef DDSCXX_HAS_TYPE_DISCOVERY
template<> constexpr unsigned int TopicTraits<::habit_msgs::srv::AppointmentDriveRequest>::type_map_blob_sz() { return 226; }
template<> constexpr unsigned int TopicTraits<::habit_msgs::srv::AppointmentDriveRequest>::type_info_blob_sz() { return 100; }
template<> inline const uint8_t * TopicTraits<::habit_msgs::srv::AppointmentDriveRequest>::type_map_blob() {
  static const uint8_t blob[] = {
 0x3b,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf1,  0xe2,  0x1a,  0xeb,  0x25,  0x58,  0xf8,  0x59, 
 0xfb,  0xc5,  0x8e,  0x8c,  0xc7,  0x4f,  0xd3,  0x00,  0x23,  0x00,  0x00,  0x00,  0xf1,  0x51,  0x01,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x13,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00, 
 0x0b,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x01,  0x00,  0x08,  0xd7,  0xe6,  0xd5,  0x5b,  0x00, 
 0x78,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf2,  0x44,  0x41,  0x89,  0xfb,  0x1f,  0x55,  0xd7, 
 0x9e,  0xf4,  0x0c,  0xf2,  0x88,  0xe6,  0xa4,  0x00,  0x60,  0x00,  0x00,  0x00,  0xf2,  0x51,  0x01,  0x00, 
 0x31,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x29,  0x00,  0x00,  0x00,  0x68,  0x61,  0x62,  0x69, 
 0x74,  0x5f,  0x6d,  0x73,  0x67,  0x73,  0x3a,  0x3a,  0x73,  0x72,  0x76,  0x3a,  0x3a,  0x41,  0x70,  0x70, 
 0x6f,  0x69,  0x6e,  0x74,  0x6d,  0x65,  0x6e,  0x74,  0x44,  0x72,  0x69,  0x76,  0x65,  0x52,  0x65,  0x71, 
 0x75,  0x65,  0x73,  0x74,  0x00,  0x00,  0x00,  0x00,  0x20,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00, 
 0x18,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x01,  0x00,  0x08,  0x00,  0x0a,  0x00,  0x00,  0x00, 
 0x74,  0x69,  0x6d,  0x65,  0x73,  0x74,  0x61,  0x6d,  0x70,  0x00,  0x00,  0x00,  0x22,  0x00,  0x00,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0xf2,  0x44,  0x41,  0x89,  0xfb,  0x1f,  0x55,  0xd7,  0x9e,  0xf4,  0x0c,  0xf2, 
 0x88,  0xe6,  0xa4,  0xf1,  0xe2,  0x1a,  0xeb,  0x25,  0x58,  0xf8,  0x59,  0xfb,  0xc5,  0x8e,  0x8c,  0xc7, 
 0x4f,  0xd3, };
  return blob;
}
template<> inline const uint8_t * TopicTraits<::habit_msgs::srv::AppointmentDriveRequest>::type_info_blob() {
  static const uint8_t blob[] = {
 0x60,  0x00,  0x00,  0x00,  0x01,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf1,  0xe2,  0x1a,  0xeb,  0x25,  0x58,  0xf8,  0x59,  0xfb,  0xc5,  0x8e,  0x8c, 
 0xc7,  0x4f,  0xd3,  0x00,  0x27,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x02,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf2,  0x44,  0x41,  0x89,  0xfb,  0x1f,  0x55,  0xd7,  0x9e,  0xf4,  0x0c,  0xf2, 
 0x88,  0xe6,  0xa4,  0x00,  0x64,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0x00,  0x00, };
  return blob;
}
#endif //DDSCXX_HAS_TYPE_DISCOVERY

} //namespace topic
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

namespace dds {
namespace topic {

template <>
struct topic_type_name<::habit_msgs::srv::AppointmentDriveRequest>
{
    static std::string value()
    {
      return org::eclipse::cyclonedds::topic::TopicTraits<::habit_msgs::srv::AppointmentDriveRequest>::getTypeName();
    }
};

}
}

REGISTER_TOPIC_TYPE(::habit_msgs::srv::AppointmentDriveRequest)

namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

template<>
propvec &get_type_props<::habit_msgs::srv::AppointmentDriveRequest>();

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool write(T& streamer, const ::habit_msgs::srv::AppointmentDriveRequest& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!write(streamer, instance.timestamp()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
    }
    prop = streamer.next_entity(prop);
  }
  return streamer.finish_struct(*props);
}

template<typename S, std::enable_if_t<std::is_base_of<cdr_stream, S>::value, bool> = true >
bool write(S& str, const ::habit_msgs::srv::AppointmentDriveRequest& instance, bool as_key) {
  auto &props = get_type_props<::habit_msgs::srv::AppointmentDriveRequest>();
  str.set_mode(cdr_stream::stream_mode::write, as_key);
  return write(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool read(T& streamer, ::habit_msgs::srv::AppointmentDriveRequest& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!read(streamer, instance.timestamp()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
    }
    prop = streamer.next_entity(prop);
  }
  return streamer.finish_struct(*props);
}

template<typename S, std::enable_if_t<std::is_base_of<cdr_stream, S>::value, bool> = true >
bool read(S& str, ::habit_msgs::srv::AppointmentDriveRequest& instance, bool as_key) {
  auto &props = get_type_props<::habit_msgs::srv::AppointmentDriveRequest>();
  str.set_mode(cdr_stream::stream_mode::read, as_key);
  return read(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool move(T& streamer, const ::habit_msgs::srv::AppointmentDriveRequest& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!move(streamer, instance.timestamp()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
    }
    prop = streamer.next_entity(prop);
  }
  return streamer.finish_struct(*props);
}

template<typename S, std::enable_if_t<std::is_base_of<cdr_stream, S>::value, bool> = true >
bool move(S& str, const ::habit_msgs::srv::AppointmentDriveRequest& instance, bool as_key) {
  auto &props = get_type_props<::habit_msgs::srv::AppointmentDriveRequest>();
  str.set_mode(cdr_stream::stream_mode::move, as_key);
  return move(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool max(T& streamer, const ::habit_msgs::srv::AppointmentDriveRequest& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!max(streamer, instance.timestamp()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
    }
    prop = streamer.next_entity(prop);
  }
  return streamer.finish_struct(*props);
}

template<typename S, std::enable_if_t<std::is_base_of<cdr_stream, S>::value, bool> = true >
bool max(S& str, const ::habit_msgs::srv::AppointmentDriveRequest& instance, bool as_key) {
  auto &props = get_type_props<::habit_msgs::srv::AppointmentDriveRequest>();
  str.set_mode(cdr_stream::stream_mode::max, as_key);
  return max(str, instance, props.data()); 
}

} //namespace cdr
} //namespace core
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

#endif // DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_HABIT_MSGS_SRV_APPOINTMENTDRIVE_HPP
