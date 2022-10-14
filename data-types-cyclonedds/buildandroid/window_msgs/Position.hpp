/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to CXX Translator
  File name: Position.idl
  Source: Position.hpp
  Cyclone DDS: v0.10.0

*****************************************************************/
#ifndef DDSCXX_POSITION_HPP
#define DDSCXX_POSITION_HPP

#include <cstdint>
#include <string>

namespace window_msgs
{
namespace msg
{
const std::string TOPIC_WINDOW_POSITION = "window_msgs/msg/Position";

class Position
{
private:
 int32_t zone_ = 0;
 int32_t persent_ = 0;

public:
  Position() = default;

  explicit Position(
    int32_t zone,
    int32_t persent) :
    zone_(zone),
    persent_(persent) { }

  int32_t zone() const { return this->zone_; }
  int32_t& zone() { return this->zone_; }
  void zone(int32_t _val_) { this->zone_ = _val_; }
  int32_t persent() const { return this->persent_; }
  int32_t& persent() { return this->persent_; }
  void persent(int32_t _val_) { this->persent_ = _val_; }

  bool operator==(const Position& _other) const
  {
    (void) _other;
    return zone_ == _other.zone_ &&
      persent_ == _other.persent_;
  }

  bool operator!=(const Position& _other) const
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

template <> constexpr const char* TopicTraits<::window_msgs::msg::Position>::getTypeName()
{
  return "window_msgs::msg::Position";
}

#ifdef DDSCXX_HAS_TYPE_DISCOVERY
template<> constexpr unsigned int TopicTraits<::window_msgs::msg::Position>::type_map_blob_sz() { return 250; }
template<> constexpr unsigned int TopicTraits<::window_msgs::msg::Position>::type_info_blob_sz() { return 100; }
template<> inline const uint8_t * TopicTraits<::window_msgs::msg::Position>::type_map_blob() {
  static const uint8_t blob[] = {
 0x4b,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf1,  0xe2,  0xd5,  0x2f,  0xfc,  0xa1,  0x68,  0xdf, 
 0x18,  0x20,  0x23,  0x5b,  0x25,  0x05,  0x50,  0x00,  0x33,  0x00,  0x00,  0x00,  0xf1,  0x51,  0x01,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x23,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00, 
 0x0b,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x31,  0x00,  0x04,  0xa8,  0x4e,  0x5f,  0x25,  0x00, 
 0x0b,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x04,  0xfa,  0xb5,  0x32,  0x01,  0x00, 
 0x7e,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf2,  0x5a,  0xec,  0xb7,  0x27,  0x9a,  0xbe,  0xcd, 
 0x69,  0x14,  0x80,  0xfa,  0xd8,  0x10,  0x7f,  0x00,  0x66,  0x00,  0x00,  0x00,  0xf2,  0x51,  0x01,  0x00, 
 0x23,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x1b,  0x00,  0x00,  0x00,  0x77,  0x69,  0x6e,  0x64, 
 0x6f,  0x77,  0x5f,  0x6d,  0x73,  0x67,  0x73,  0x3a,  0x3a,  0x6d,  0x73,  0x67,  0x3a,  0x3a,  0x50,  0x6f, 
 0x73,  0x69,  0x74,  0x69,  0x6f,  0x6e,  0x00,  0x00,  0x36,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00, 
 0x13,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x31,  0x00,  0x04,  0x00,  0x05,  0x00,  0x00,  0x00, 
 0x7a,  0x6f,  0x6e,  0x65,  0x00,  0x00,  0x00,  0x00,  0x16,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00, 
 0x01,  0x00,  0x04,  0x00,  0x08,  0x00,  0x00,  0x00,  0x70,  0x65,  0x72,  0x73,  0x65,  0x6e,  0x74,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x22,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf2,  0x5a,  0xec,  0xb7, 
 0x27,  0x9a,  0xbe,  0xcd,  0x69,  0x14,  0x80,  0xfa,  0xd8,  0x10,  0x7f,  0xf1,  0xe2,  0xd5,  0x2f,  0xfc, 
 0xa1,  0x68,  0xdf,  0x18,  0x20,  0x23,  0x5b,  0x25,  0x05,  0x50, };
  return blob;
}
template<> inline const uint8_t * TopicTraits<::window_msgs::msg::Position>::type_info_blob() {
  static const uint8_t blob[] = {
 0x60,  0x00,  0x00,  0x00,  0x01,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf1,  0xe2,  0xd5,  0x2f,  0xfc,  0xa1,  0x68,  0xdf,  0x18,  0x20,  0x23,  0x5b, 
 0x25,  0x05,  0x50,  0x00,  0x37,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x02,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf2,  0x5a,  0xec,  0xb7,  0x27,  0x9a,  0xbe,  0xcd,  0x69,  0x14,  0x80,  0xfa, 
 0xd8,  0x10,  0x7f,  0x00,  0x6a,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
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
struct topic_type_name<::window_msgs::msg::Position>
{
    static std::string value()
    {
      return org::eclipse::cyclonedds::topic::TopicTraits<::window_msgs::msg::Position>::getTypeName();
    }
};

}
}

REGISTER_TOPIC_TYPE(::window_msgs::msg::Position)

namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

template<>
propvec &get_type_props<::window_msgs::msg::Position>();

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool write(T& streamer, const ::window_msgs::msg::Position& instance, entity_properties_t *props) {
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
      if (!write(streamer, instance.persent()))
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
bool write(S& str, const ::window_msgs::msg::Position& instance, bool as_key) {
  auto &props = get_type_props<::window_msgs::msg::Position>();
  str.set_mode(cdr_stream::stream_mode::write, as_key);
  return write(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool read(T& streamer, ::window_msgs::msg::Position& instance, entity_properties_t *props) {
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
      if (!read(streamer, instance.persent()))
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
bool read(S& str, ::window_msgs::msg::Position& instance, bool as_key) {
  auto &props = get_type_props<::window_msgs::msg::Position>();
  str.set_mode(cdr_stream::stream_mode::read, as_key);
  return read(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool move(T& streamer, const ::window_msgs::msg::Position& instance, entity_properties_t *props) {
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
      if (!move(streamer, instance.persent()))
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
bool move(S& str, const ::window_msgs::msg::Position& instance, bool as_key) {
  auto &props = get_type_props<::window_msgs::msg::Position>();
  str.set_mode(cdr_stream::stream_mode::move, as_key);
  return move(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool max(T& streamer, const ::window_msgs::msg::Position& instance, entity_properties_t *props) {
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
      if (!max(streamer, instance.persent()))
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
bool max(S& str, const ::window_msgs::msg::Position& instance, bool as_key) {
  auto &props = get_type_props<::window_msgs::msg::Position>();
  str.set_mode(cdr_stream::stream_mode::max, as_key);
  return max(str, instance, props.data()); 
}

} //namespace cdr
} //namespace core
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

#endif // DDSCXX_POSITION_HPP
