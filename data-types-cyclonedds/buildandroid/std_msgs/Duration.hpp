/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to CXX Translator
  File name: Duration.idl
  Source: Duration.hpp
  Cyclone DDS: v0.10.0

*****************************************************************/
#ifndef DDSCXX_DURATION_HPP
#define DDSCXX_DURATION_HPP

#include <cstdint>

namespace std_msgs
{
namespace msg
{
class Duration
{
private:
 int32_t sec_ = 0;
 uint32_t nanosec_ = 0;

public:
  Duration() = default;

  explicit Duration(
    int32_t sec,
    uint32_t nanosec) :
    sec_(sec),
    nanosec_(nanosec) { }

  int32_t sec() const { return this->sec_; }
  int32_t& sec() { return this->sec_; }
  void sec(int32_t _val_) { this->sec_ = _val_; }
  uint32_t nanosec() const { return this->nanosec_; }
  uint32_t& nanosec() { return this->nanosec_; }
  void nanosec(uint32_t _val_) { this->nanosec_ = _val_; }

  bool operator==(const Duration& _other) const
  {
    (void) _other;
    return sec_ == _other.sec_ &&
      nanosec_ == _other.nanosec_;
  }

  bool operator!=(const Duration& _other) const
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

template <> constexpr const char* TopicTraits<::std_msgs::msg::Duration>::getTypeName()
{
  return "std_msgs::msg::Duration";
}

template <> constexpr bool TopicTraits<::std_msgs::msg::Duration>::isKeyless()
{
  return true;
}

#ifdef DDSCXX_HAS_TYPE_DISCOVERY
template<> constexpr unsigned int TopicTraits<::std_msgs::msg::Duration>::type_map_blob_sz() { return 246; }
template<> constexpr unsigned int TopicTraits<::std_msgs::msg::Duration>::type_info_blob_sz() { return 100; }
template<> inline const uint8_t * TopicTraits<::std_msgs::msg::Duration>::type_map_blob() {
  static const uint8_t blob[] = {
 0x4b,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf1,  0x56,  0x7c,  0x5a,  0x93,  0x54,  0x1c,  0x3b, 
 0x10,  0x86,  0xa4,  0xba,  0x46,  0xf9,  0x8d,  0x00,  0x33,  0x00,  0x00,  0x00,  0xf1,  0x51,  0x01,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x23,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00, 
 0x0b,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x01,  0x00,  0x04,  0x74,  0x45,  0x9c,  0xa3,  0x00, 
 0x0b,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x07,  0xe2,  0x04,  0x64,  0xd5,  0x00, 
 0x7a,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf2,  0x04,  0x40,  0xd3,  0x1d,  0x23,  0xa5,  0xf8, 
 0xa9,  0xfb,  0xa8,  0x16,  0x63,  0x7d,  0xbe,  0x00,  0x62,  0x00,  0x00,  0x00,  0xf2,  0x51,  0x01,  0x00, 
 0x20,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x18,  0x00,  0x00,  0x00,  0x73,  0x74,  0x64,  0x5f, 
 0x6d,  0x73,  0x67,  0x73,  0x3a,  0x3a,  0x6d,  0x73,  0x67,  0x3a,  0x3a,  0x44,  0x75,  0x72,  0x61,  0x74, 
 0x69,  0x6f,  0x6e,  0x00,  0x36,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00,  0x12,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x01,  0x00,  0x04,  0x00,  0x04,  0x00,  0x00,  0x00,  0x73,  0x65,  0x63,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x16,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x07,  0x00, 
 0x08,  0x00,  0x00,  0x00,  0x6e,  0x61,  0x6e,  0x6f,  0x73,  0x65,  0x63,  0x00,  0x00,  0x00,  0x00,  0x00, 
 0x22,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf2,  0x04,  0x40,  0xd3,  0x1d,  0x23,  0xa5,  0xf8, 
 0xa9,  0xfb,  0xa8,  0x16,  0x63,  0x7d,  0xbe,  0xf1,  0x56,  0x7c,  0x5a,  0x93,  0x54,  0x1c,  0x3b,  0x10, 
 0x86,  0xa4,  0xba,  0x46,  0xf9,  0x8d, };
  return blob;
}
template<> inline const uint8_t * TopicTraits<::std_msgs::msg::Duration>::type_info_blob() {
  static const uint8_t blob[] = {
 0x60,  0x00,  0x00,  0x00,  0x01,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf1,  0x56,  0x7c,  0x5a,  0x93,  0x54,  0x1c,  0x3b,  0x10,  0x86,  0xa4,  0xba, 
 0x46,  0xf9,  0x8d,  0x00,  0x37,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x02,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf2,  0x04,  0x40,  0xd3,  0x1d,  0x23,  0xa5,  0xf8,  0xa9,  0xfb,  0xa8,  0x16, 
 0x63,  0x7d,  0xbe,  0x00,  0x66,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
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
struct topic_type_name<::std_msgs::msg::Duration>
{
    static std::string value()
    {
      return org::eclipse::cyclonedds::topic::TopicTraits<::std_msgs::msg::Duration>::getTypeName();
    }
};

}
}

REGISTER_TOPIC_TYPE(::std_msgs::msg::Duration)

namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

template<>
propvec &get_type_props<::std_msgs::msg::Duration>();

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool write(T& streamer, const ::std_msgs::msg::Duration& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!write(streamer, instance.sec()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!write(streamer, instance.nanosec()))
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
bool write(S& str, const ::std_msgs::msg::Duration& instance, bool as_key) {
  auto &props = get_type_props<::std_msgs::msg::Duration>();
  str.set_mode(cdr_stream::stream_mode::write, as_key);
  return write(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool read(T& streamer, ::std_msgs::msg::Duration& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!read(streamer, instance.sec()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!read(streamer, instance.nanosec()))
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
bool read(S& str, ::std_msgs::msg::Duration& instance, bool as_key) {
  auto &props = get_type_props<::std_msgs::msg::Duration>();
  str.set_mode(cdr_stream::stream_mode::read, as_key);
  return read(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool move(T& streamer, const ::std_msgs::msg::Duration& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!move(streamer, instance.sec()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!move(streamer, instance.nanosec()))
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
bool move(S& str, const ::std_msgs::msg::Duration& instance, bool as_key) {
  auto &props = get_type_props<::std_msgs::msg::Duration>();
  str.set_mode(cdr_stream::stream_mode::move, as_key);
  return move(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool max(T& streamer, const ::std_msgs::msg::Duration& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!max(streamer, instance.sec()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!max(streamer, instance.nanosec()))
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
bool max(S& str, const ::std_msgs::msg::Duration& instance, bool as_key) {
  auto &props = get_type_props<::std_msgs::msg::Duration>();
  str.set_mode(cdr_stream::stream_mode::max, as_key);
  return max(str, instance, props.data()); 
}

} //namespace cdr
} //namespace core
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

#endif // DDSCXX_DURATION_HPP