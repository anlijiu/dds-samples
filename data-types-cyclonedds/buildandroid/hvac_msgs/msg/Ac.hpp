/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to CXX Translator
  File name: Ac.idl
  Source: /home/an/workspace/dds/dds-samples/data-types-cyclonedds/buildandroid/hvac_msgs/msg/Ac.hpp
  Cyclone DDS: v0.10.0

*****************************************************************/
#ifndef DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_HVAC_MSGS_MSG_AC_HPP
#define DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_HVAC_MSGS_MSG_AC_HPP

#include <cstdint>
#include <string>

namespace hvac_msgs
{
namespace msg
{
const std::string TOPIC_AC = "hvac_msgs/msg/Ac";

class Ac
{
private:
 int32_t zone_ = 0;
 bool on_ = false;

public:
  Ac() = default;

  explicit Ac(
    int32_t zone,
    bool on) :
    zone_(zone),
    on_(on) { }

  int32_t zone() const { return this->zone_; }
  int32_t& zone() { return this->zone_; }
  void zone(int32_t _val_) { this->zone_ = _val_; }
  bool on() const { return this->on_; }
  bool& on() { return this->on_; }
  void on(bool _val_) { this->on_ = _val_; }

  bool operator==(const Ac& _other) const
  {
    (void) _other;
    return zone_ == _other.zone_ &&
      on_ == _other.on_;
  }

  bool operator!=(const Ac& _other) const
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

template <> constexpr const char* TopicTraits<::hvac_msgs::msg::Ac>::getTypeName()
{
  return "hvac_msgs::msg::Ac";
}

#ifdef DDSCXX_HAS_TYPE_DISCOVERY
template<> constexpr unsigned int TopicTraits<::hvac_msgs::msg::Ac>::type_map_blob_sz() { return 238; }
template<> constexpr unsigned int TopicTraits<::hvac_msgs::msg::Ac>::type_info_blob_sz() { return 100; }
template<> inline const uint8_t * TopicTraits<::hvac_msgs::msg::Ac>::type_map_blob() {
  static const uint8_t blob[] = {
 0x4b,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf1,  0xf1,  0x22,  0x8a,  0x3e,  0xc4,  0x7c,  0x87, 
 0xff,  0x7b,  0x4a,  0x93,  0xc7,  0x49,  0xc3,  0x00,  0x33,  0x00,  0x00,  0x00,  0xf1,  0x51,  0x01,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x23,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00, 
 0x0b,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x31,  0x00,  0x04,  0xa8,  0x4e,  0x5f,  0x25,  0x00, 
 0x0b,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x01,  0xed,  0x2b,  0x5c,  0x01,  0x00, 
 0x71,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf2,  0x1c,  0x95,  0xd8,  0x23,  0xf9,  0x25,  0x86, 
 0x8a,  0xfa,  0x85,  0x46,  0xf4,  0x17,  0x2a,  0x00,  0x59,  0x00,  0x00,  0x00,  0xf2,  0x51,  0x01,  0x00, 
 0x1b,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x13,  0x00,  0x00,  0x00,  0x68,  0x76,  0x61,  0x63, 
 0x5f,  0x6d,  0x73,  0x67,  0x73,  0x3a,  0x3a,  0x6d,  0x73,  0x67,  0x3a,  0x3a,  0x41,  0x63,  0x00,  0x00, 
 0x31,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00,  0x13,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00, 
 0x31,  0x00,  0x04,  0x00,  0x05,  0x00,  0x00,  0x00,  0x7a,  0x6f,  0x6e,  0x65,  0x00,  0x00,  0x00,  0x00, 
 0x11,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x01,  0x00,  0x03,  0x00,  0x00,  0x00, 
 0x6f,  0x6e,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x22,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00, 
 0xf2,  0x1c,  0x95,  0xd8,  0x23,  0xf9,  0x25,  0x86,  0x8a,  0xfa,  0x85,  0x46,  0xf4,  0x17,  0x2a,  0xf1, 
 0xf1,  0x22,  0x8a,  0x3e,  0xc4,  0x7c,  0x87,  0xff,  0x7b,  0x4a,  0x93,  0xc7,  0x49,  0xc3, };
  return blob;
}
template<> inline const uint8_t * TopicTraits<::hvac_msgs::msg::Ac>::type_info_blob() {
  static const uint8_t blob[] = {
 0x60,  0x00,  0x00,  0x00,  0x01,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf1,  0xf1,  0x22,  0x8a,  0x3e,  0xc4,  0x7c,  0x87,  0xff,  0x7b,  0x4a,  0x93, 
 0xc7,  0x49,  0xc3,  0x00,  0x37,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x02,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf2,  0x1c,  0x95,  0xd8,  0x23,  0xf9,  0x25,  0x86,  0x8a,  0xfa,  0x85,  0x46, 
 0xf4,  0x17,  0x2a,  0x00,  0x5d,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
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
struct topic_type_name<::hvac_msgs::msg::Ac>
{
    static std::string value()
    {
      return org::eclipse::cyclonedds::topic::TopicTraits<::hvac_msgs::msg::Ac>::getTypeName();
    }
};

}
}

REGISTER_TOPIC_TYPE(::hvac_msgs::msg::Ac)

namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

template<>
propvec &get_type_props<::hvac_msgs::msg::Ac>();

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool write(T& streamer, const ::hvac_msgs::msg::Ac& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!write(streamer, instance.zone()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!write(streamer, instance.on()))
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
bool write(S& str, const ::hvac_msgs::msg::Ac& instance, bool as_key) {
  auto &props = get_type_props<::hvac_msgs::msg::Ac>();
  str.set_mode(cdr_stream::stream_mode::write, as_key);
  return write(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool read(T& streamer, ::hvac_msgs::msg::Ac& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!read(streamer, instance.zone()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!read(streamer, instance.on()))
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
bool read(S& str, ::hvac_msgs::msg::Ac& instance, bool as_key) {
  auto &props = get_type_props<::hvac_msgs::msg::Ac>();
  str.set_mode(cdr_stream::stream_mode::read, as_key);
  return read(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool move(T& streamer, const ::hvac_msgs::msg::Ac& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!move(streamer, instance.zone()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!move(streamer, instance.on()))
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
bool move(S& str, const ::hvac_msgs::msg::Ac& instance, bool as_key) {
  auto &props = get_type_props<::hvac_msgs::msg::Ac>();
  str.set_mode(cdr_stream::stream_mode::move, as_key);
  return move(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool max(T& streamer, const ::hvac_msgs::msg::Ac& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!max(streamer, instance.zone()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!max(streamer, instance.on()))
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
bool max(S& str, const ::hvac_msgs::msg::Ac& instance, bool as_key) {
  auto &props = get_type_props<::hvac_msgs::msg::Ac>();
  str.set_mode(cdr_stream::stream_mode::max, as_key);
  return max(str, instance, props.data()); 
}

} //namespace cdr
} //namespace core
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

#endif // DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_HVAC_MSGS_MSG_AC_HPP