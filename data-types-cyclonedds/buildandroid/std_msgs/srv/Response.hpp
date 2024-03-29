/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to CXX Translator
  File name: Response.idl
  Source: /home/an/workspace/dds/dds-samples/data-types-cyclonedds/buildandroid/std_msgs/srv/Response.hpp
  Cyclone DDS: v0.10.0

*****************************************************************/
#ifndef DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_STD_MSGS_SRV_RESPONSE_HPP
#define DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_STD_MSGS_SRV_RESPONSE_HPP

#include <string>

namespace std_msgs
{
namespace srv
{
enum class ResponseStatus
{
  REPLY_IN_PROGRESS,
  REPLY_COMPLETED,
  REPLY_ERROR};

class Response
{
private:
 std::string error_;
 ::std_msgs::srv::ResponseStatus status_ = ::std_msgs::srv::ResponseStatus::REPLY_IN_PROGRESS;

public:
  Response() = default;

  explicit Response(
    const std::string& error,
    ::std_msgs::srv::ResponseStatus status) :
    error_(error),
    status_(status) { }

  const std::string& error() const { return this->error_; }
  std::string& error() { return this->error_; }
  void error(const std::string& _val_) { this->error_ = _val_; }
  void error(std::string&& _val_) { this->error_ = _val_; }
  ::std_msgs::srv::ResponseStatus status() const { return this->status_; }
  ::std_msgs::srv::ResponseStatus& status() { return this->status_; }
  void status(::std_msgs::srv::ResponseStatus _val_) { this->status_ = _val_; }

  bool operator==(const Response& _other) const
  {
    (void) _other;
    return error_ == _other.error_ &&
      status_ == _other.status_;
  }

  bool operator!=(const Response& _other) const
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

template <> constexpr const char* TopicTraits<::std_msgs::srv::Response>::getTypeName()
{
  return "std_msgs::srv::Response";
}

template <> constexpr bool TopicTraits<::std_msgs::srv::Response>::isSelfContained()
{
  return false;
}

template <> constexpr bool TopicTraits<::std_msgs::srv::Response>::isKeyless()
{
  return true;
}

#ifdef DDSCXX_HAS_TYPE_DISCOVERY
template<> constexpr unsigned int TopicTraits<::std_msgs::srv::Response>::type_map_blob_sz() { return 592; }
template<> constexpr unsigned int TopicTraits<::std_msgs::srv::Response>::type_info_blob_sz() { return 148; }
template<> inline const uint8_t * TopicTraits<::std_msgs::srv::Response>::type_map_blob() {
  static const uint8_t blob[] = {
 0xba,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00,  0xf1,  0x7d,  0xfc,  0x3c,  0x44,  0x47,  0xf7,  0xf7, 
 0xa0,  0x3d,  0xd1,  0x46,  0x71,  0xd2,  0xf9,  0x00,  0x41,  0x00,  0x00,  0x00,  0xf1,  0x51,  0x01,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x31,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00, 
 0x0c,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x01,  0x00,  0x70,  0x00,  0xcb,  0x5e,  0x10,  0x0e, 
 0x19,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0xf1,  0xd0,  0x6c,  0x9b,  0xcd,  0x48, 
 0xe7,  0xbb,  0xd2,  0xb7,  0x07,  0x1e,  0xc3,  0x21,  0x34,  0x9a,  0xcb,  0x44,  0x54,  0xf1,  0xd0,  0x6c, 
 0x9b,  0xcd,  0x48,  0xe7,  0xbb,  0xd2,  0xb7,  0x07,  0x1e,  0xc3,  0x21,  0x34,  0x4e,  0x00,  0x00,  0x00, 
 0xf1,  0x40,  0x01,  0x00,  0x02,  0x00,  0x00,  0x00,  0x20,  0x00,  0x00,  0x00,  0x3e,  0x00,  0x00,  0x00, 
 0x03,  0x00,  0x00,  0x00,  0x0e,  0x00,  0x00,  0x00,  0x06,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0xd0,  0x1d,  0x36,  0x12,  0x00,  0x00,  0x0e,  0x00,  0x00,  0x00,  0x06,  0x00,  0x00,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x00,  0x00,  0x9a,  0xbd,  0x42,  0xd4,  0x00,  0x00,  0x0e,  0x00,  0x00,  0x00, 
 0x06,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00,  0x00,  0x00,  0x9f,  0xf2,  0x25,  0xee,  0x00,  0x00, 
 0x46,  0x01,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00,  0xf2,  0x5f,  0x23,  0x0b,  0xe7,  0xe0,  0x80,  0x3a, 
 0x8e,  0xcc,  0x43,  0x21,  0x59,  0xbf,  0x5a,  0x00,  0x71,  0x00,  0x00,  0x00,  0xf2,  0x51,  0x01,  0x00, 
 0x20,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x18,  0x00,  0x00,  0x00,  0x73,  0x74,  0x64,  0x5f, 
 0x6d,  0x73,  0x67,  0x73,  0x3a,  0x3a,  0x73,  0x72,  0x76,  0x3a,  0x3a,  0x52,  0x65,  0x73,  0x70,  0x6f, 
 0x6e,  0x73,  0x65,  0x00,  0x45,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00,  0x14,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x01,  0x00,  0x70,  0x00,  0x06,  0x00,  0x00,  0x00,  0x65,  0x72,  0x72,  0x6f, 
 0x72,  0x00,  0x00,  0x00,  0x25,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0xf2,  0x23, 
 0x7f,  0x30,  0x93,  0xdd,  0x5f,  0x17,  0x79,  0xdc,  0x30,  0x61,  0x29,  0xcc,  0x7e,  0x00,  0x00,  0x00, 
 0x07,  0x00,  0x00,  0x00,  0x73,  0x74,  0x61,  0x74,  0x75,  0x73,  0x00,  0x00,  0x00,  0xf2,  0x23,  0x7f, 
 0x30,  0x93,  0xdd,  0x5f,  0x17,  0x79,  0xdc,  0x30,  0x61,  0x29,  0xcc,  0x7e,  0xaa,  0x00,  0x00,  0x00, 
 0xf2,  0x40,  0x01,  0x00,  0x26,  0x00,  0x00,  0x00,  0x20,  0x00,  0x00,  0x00,  0x1e,  0x00,  0x00,  0x00, 
 0x73,  0x74,  0x64,  0x5f,  0x6d,  0x73,  0x67,  0x73,  0x3a,  0x3a,  0x73,  0x72,  0x76,  0x3a,  0x3a,  0x52, 
 0x65,  0x73,  0x70,  0x6f,  0x6e,  0x73,  0x65,  0x53,  0x74,  0x61,  0x74,  0x75,  0x73,  0x00,  0x00,  0x00, 
 0x76,  0x00,  0x00,  0x00,  0x03,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00,  0x06,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x12,  0x00,  0x00,  0x00,  0x52,  0x45,  0x50,  0x4c, 
 0x59,  0x5f,  0x49,  0x4e,  0x5f,  0x50,  0x52,  0x4f,  0x47,  0x52,  0x45,  0x53,  0x53,  0x00,  0x00,  0x00, 
 0x22,  0x00,  0x00,  0x00,  0x06,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00, 
 0x10,  0x00,  0x00,  0x00,  0x52,  0x45,  0x50,  0x4c,  0x59,  0x5f,  0x43,  0x4f,  0x4d,  0x50,  0x4c,  0x45, 
 0x54,  0x45,  0x44,  0x00,  0x00,  0x00,  0x00,  0x00,  0x1e,  0x00,  0x00,  0x00,  0x06,  0x00,  0x00,  0x00, 
 0x02,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x0c,  0x00,  0x00,  0x00,  0x52,  0x45,  0x50,  0x4c, 
 0x59,  0x5f,  0x45,  0x52,  0x52,  0x4f,  0x52,  0x00,  0x00,  0x00,  0x00,  0x00,  0x40,  0x00,  0x00,  0x00, 
 0x02,  0x00,  0x00,  0x00,  0xf2,  0x5f,  0x23,  0x0b,  0xe7,  0xe0,  0x80,  0x3a,  0x8e,  0xcc,  0x43,  0x21, 
 0x59,  0xbf,  0x5a,  0xf1,  0x7d,  0xfc,  0x3c,  0x44,  0x47,  0xf7,  0xf7,  0xa0,  0x3d,  0xd1,  0x46,  0x71, 
 0xd2,  0xf9,  0xf2,  0x23,  0x7f,  0x30,  0x93,  0xdd,  0x5f,  0x17,  0x79,  0xdc,  0x30,  0x61,  0x29,  0xcc, 
 0x7e,  0xf1,  0xd0,  0x6c,  0x9b,  0xcd,  0x48,  0xe7,  0xbb,  0xd2,  0xb7,  0x07,  0x1e,  0xc3,  0x21,  0x34, 
};
  return blob;
}
template<> inline const uint8_t * TopicTraits<::std_msgs::srv::Response>::type_info_blob() {
  static const uint8_t blob[] = {
 0x90,  0x00,  0x00,  0x00,  0x01,  0x10,  0x00,  0x40,  0x40,  0x00,  0x00,  0x00,  0x3c,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf1,  0x7d,  0xfc,  0x3c,  0x44,  0x47,  0xf7,  0xf7,  0xa0,  0x3d,  0xd1,  0x46, 
 0x71,  0xd2,  0xf9,  0x00,  0x45,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x1c,  0x00,  0x00,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x14,  0x00,  0x00,  0x00,  0xf1,  0xd0,  0x6c,  0x9b,  0xcd,  0x48,  0xe7,  0xbb, 
 0xd2,  0xb7,  0x07,  0x1e,  0xc3,  0x21,  0x34,  0x00,  0x52,  0x00,  0x00,  0x00,  0x02,  0x10,  0x00,  0x40, 
 0x40,  0x00,  0x00,  0x00,  0x3c,  0x00,  0x00,  0x00,  0x14,  0x00,  0x00,  0x00,  0xf2,  0x5f,  0x23,  0x0b, 
 0xe7,  0xe0,  0x80,  0x3a,  0x8e,  0xcc,  0x43,  0x21,  0x59,  0xbf,  0x5a,  0x00,  0x75,  0x00,  0x00,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x1c,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x14,  0x00,  0x00,  0x00, 
 0xf2,  0x23,  0x7f,  0x30,  0x93,  0xdd,  0x5f,  0x17,  0x79,  0xdc,  0x30,  0x61,  0x29,  0xcc,  0x7e,  0x00, 
 0xae,  0x00,  0x00,  0x00, };
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
struct topic_type_name<::std_msgs::srv::Response>
{
    static std::string value()
    {
      return org::eclipse::cyclonedds::topic::TopicTraits<::std_msgs::srv::Response>::getTypeName();
    }
};

}
}

REGISTER_TOPIC_TYPE(::std_msgs::srv::Response)

namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

template<>
::std_msgs::srv::ResponseStatus enum_conversion<::std_msgs::srv::ResponseStatus>(uint32_t in);

template<>
propvec &get_type_props<::std_msgs::srv::Response>();

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool write(T& streamer, const ::std_msgs::srv::Response& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!write_string(streamer, instance.error(), 0))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!write(streamer, instance.status()))
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
bool write(S& str, const ::std_msgs::srv::Response& instance, bool as_key) {
  auto &props = get_type_props<::std_msgs::srv::Response>();
  str.set_mode(cdr_stream::stream_mode::write, as_key);
  return write(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool read(T& streamer, ::std_msgs::srv::Response& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!read_string(streamer, instance.error(), 0))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!read(streamer, instance.status()))
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
bool read(S& str, ::std_msgs::srv::Response& instance, bool as_key) {
  auto &props = get_type_props<::std_msgs::srv::Response>();
  str.set_mode(cdr_stream::stream_mode::read, as_key);
  return read(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool move(T& streamer, const ::std_msgs::srv::Response& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!move_string(streamer, instance.error(), 0))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!move(streamer, instance.status()))
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
bool move(S& str, const ::std_msgs::srv::Response& instance, bool as_key) {
  auto &props = get_type_props<::std_msgs::srv::Response>();
  str.set_mode(cdr_stream::stream_mode::move, as_key);
  return move(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool max(T& streamer, const ::std_msgs::srv::Response& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!max_string(streamer, instance.error(), 0))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!max(streamer, instance.status()))
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
bool max(S& str, const ::std_msgs::srv::Response& instance, bool as_key) {
  auto &props = get_type_props<::std_msgs::srv::Response>();
  str.set_mode(cdr_stream::stream_mode::max, as_key);
  return max(str, instance, props.data()); 
}

} //namespace cdr
} //namespace core
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

#endif // DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_STD_MSGS_SRV_RESPONSE_HPP
