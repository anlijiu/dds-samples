/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to CXX Translator
  File name: Cloud.idl
  Source: Cloud.hpp
  Cyclone DDS: v0.10.0

*****************************************************************/
#ifndef DDSCXX_CLOUD_HPP
#define DDSCXX_CLOUD_HPP

#include <cstdint>
#include <array>
#include <string>

namespace cloud_msgs
{
namespace msg
{
const std::string TOPIC_WEATHER_FORCAST_TEMPERATURE = "cloud_msgs/msg/WeatherForcastTemperature";

class WeatherForcastTemperature
{
private:
 std::array<int16_t, 5> min_ = { };
 std::array<int16_t, 5> max_ = { };

public:
  WeatherForcastTemperature() = default;

  explicit WeatherForcastTemperature(
    const std::array<int16_t, 5>& min,
    const std::array<int16_t, 5>& max) :
    min_(min),
    max_(max) { }

  const std::array<int16_t, 5>& min() const { return this->min_; }
  std::array<int16_t, 5>& min() { return this->min_; }
  void min(const std::array<int16_t, 5>& _val_) { this->min_ = _val_; }
  void min(std::array<int16_t, 5>&& _val_) { this->min_ = _val_; }
  const std::array<int16_t, 5>& max() const { return this->max_; }
  std::array<int16_t, 5>& max() { return this->max_; }
  void max(const std::array<int16_t, 5>& _val_) { this->max_ = _val_; }
  void max(std::array<int16_t, 5>&& _val_) { this->max_ = _val_; }

  bool operator==(const WeatherForcastTemperature& _other) const
  {
    (void) _other;
    return min_ == _other.min_ &&
      max_ == _other.max_;
  }

  bool operator!=(const WeatherForcastTemperature& _other) const
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

template <> constexpr const char* TopicTraits<::cloud_msgs::msg::WeatherForcastTemperature>::getTypeName()
{
  return "cloud_msgs::msg::WeatherForcastTemperature";
}

template <> constexpr bool TopicTraits<::cloud_msgs::msg::WeatherForcastTemperature>::isKeyless()
{
  return true;
}

#ifdef DDSCXX_HAS_TYPE_DISCOVERY
template<> constexpr unsigned int TopicTraits<::cloud_msgs::msg::WeatherForcastTemperature>::type_map_blob_sz() { return 310; }
template<> constexpr unsigned int TopicTraits<::cloud_msgs::msg::WeatherForcastTemperature>::type_info_blob_sz() { return 100; }
template<> inline const uint8_t * TopicTraits<::cloud_msgs::msg::WeatherForcastTemperature>::type_map_blob() {
  static const uint8_t blob[] = {
 0x62,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf1,  0x67,  0x72,  0x34,  0x83,  0x1c,  0xd5,  0x12, 
 0x34,  0xef,  0xbe,  0x3f,  0x41,  0x24,  0xe9,  0x00,  0x4a,  0x00,  0x00,  0x00,  0xf1,  0x51,  0x01,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x3a,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00, 
 0x16,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x01,  0x00,  0x90,  0xf3,  0x01,  0x00,  0x00,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x05,  0x03,  0xd8,  0xbd,  0x79,  0xcc,  0x00,  0x00,  0x16,  0x00,  0x00,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x90,  0xf3,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00, 
 0x05,  0x03,  0x2f,  0xfe,  0x4e,  0x77,  0x00,  0x00,  0xa2,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00, 
 0xf2,  0x27,  0xd0,  0x6b,  0x06,  0xbd,  0x16,  0x63,  0x80,  0x10,  0xb1,  0x47,  0xa5,  0xe9,  0x4a,  0x00, 
 0x8a,  0x00,  0x00,  0x00,  0xf2,  0x51,  0x01,  0x00,  0x33,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00, 
 0x2b,  0x00,  0x00,  0x00,  0x63,  0x6c,  0x6f,  0x75,  0x64,  0x5f,  0x6d,  0x73,  0x67,  0x73,  0x3a,  0x3a, 
 0x6d,  0x73,  0x67,  0x3a,  0x3a,  0x57,  0x65,  0x61,  0x74,  0x68,  0x65,  0x72,  0x46,  0x6f,  0x72,  0x63, 
 0x61,  0x73,  0x74,  0x54,  0x65,  0x6d,  0x70,  0x65,  0x72,  0x61,  0x74,  0x75,  0x72,  0x65,  0x00,  0x00, 
 0x4a,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00,  0x1e,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00, 
 0x01,  0x00,  0x90,  0xf3,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x05,  0x03,  0x00,  0x00, 
 0x04,  0x00,  0x00,  0x00,  0x6d,  0x69,  0x6e,  0x00,  0x00,  0x00,  0x00,  0x00,  0x1e,  0x00,  0x00,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x90,  0xf3,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00, 
 0x05,  0x03,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00,  0x6d,  0x61,  0x78,  0x00,  0x00,  0x00,  0x00,  0x00, 
 0x22,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf2,  0x27,  0xd0,  0x6b,  0x06,  0xbd,  0x16,  0x63, 
 0x80,  0x10,  0xb1,  0x47,  0xa5,  0xe9,  0x4a,  0xf1,  0x67,  0x72,  0x34,  0x83,  0x1c,  0xd5,  0x12,  0x34, 
 0xef,  0xbe,  0x3f,  0x41,  0x24,  0xe9, };
  return blob;
}
template<> inline const uint8_t * TopicTraits<::cloud_msgs::msg::WeatherForcastTemperature>::type_info_blob() {
  static const uint8_t blob[] = {
 0x60,  0x00,  0x00,  0x00,  0x01,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf1,  0x67,  0x72,  0x34,  0x83,  0x1c,  0xd5,  0x12,  0x34,  0xef,  0xbe,  0x3f, 
 0x41,  0x24,  0xe9,  0x00,  0x4e,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x02,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf2,  0x27,  0xd0,  0x6b,  0x06,  0xbd,  0x16,  0x63,  0x80,  0x10,  0xb1,  0x47, 
 0xa5,  0xe9,  0x4a,  0x00,  0x8e,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
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
struct topic_type_name<::cloud_msgs::msg::WeatherForcastTemperature>
{
    static std::string value()
    {
      return org::eclipse::cyclonedds::topic::TopicTraits<::cloud_msgs::msg::WeatherForcastTemperature>::getTypeName();
    }
};

}
}

REGISTER_TOPIC_TYPE(::cloud_msgs::msg::WeatherForcastTemperature)

namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

template<>
propvec &get_type_props<::cloud_msgs::msg::WeatherForcastTemperature>();

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool write(T& streamer, const ::cloud_msgs::msg::WeatherForcastTemperature& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!streamer.start_consecutive(true, true))
        return false;
      if (!write(streamer, instance.min()[0], instance.min().size()))
        return false;
      if (!streamer.finish_consecutive())
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!streamer.start_consecutive(true, true))
        return false;
      if (!write(streamer, instance.max()[0], instance.max().size()))
        return false;
      if (!streamer.finish_consecutive())
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
bool write(S& str, const ::cloud_msgs::msg::WeatherForcastTemperature& instance, bool as_key) {
  auto &props = get_type_props<::cloud_msgs::msg::WeatherForcastTemperature>();
  str.set_mode(cdr_stream::stream_mode::write, as_key);
  return write(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool read(T& streamer, ::cloud_msgs::msg::WeatherForcastTemperature& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!streamer.start_consecutive(true, true))
        return false;
      if (!read(streamer, instance.min()[0], instance.min().size()))
        return false;
      if (!streamer.finish_consecutive())
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!streamer.start_consecutive(true, true))
        return false;
      if (!read(streamer, instance.max()[0], instance.max().size()))
        return false;
      if (!streamer.finish_consecutive())
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
bool read(S& str, ::cloud_msgs::msg::WeatherForcastTemperature& instance, bool as_key) {
  auto &props = get_type_props<::cloud_msgs::msg::WeatherForcastTemperature>();
  str.set_mode(cdr_stream::stream_mode::read, as_key);
  return read(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool move(T& streamer, const ::cloud_msgs::msg::WeatherForcastTemperature& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!streamer.start_consecutive(true, true))
        return false;
      if (!move(streamer, instance.min()[0], instance.min().size()))
        return false;
      if (!streamer.finish_consecutive())
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!streamer.start_consecutive(true, true))
        return false;
      if (!move(streamer, instance.max()[0], instance.max().size()))
        return false;
      if (!streamer.finish_consecutive())
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
bool move(S& str, const ::cloud_msgs::msg::WeatherForcastTemperature& instance, bool as_key) {
  auto &props = get_type_props<::cloud_msgs::msg::WeatherForcastTemperature>();
  str.set_mode(cdr_stream::stream_mode::move, as_key);
  return move(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool max(T& streamer, const ::cloud_msgs::msg::WeatherForcastTemperature& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!streamer.start_consecutive(true, true))
        return false;
      if (!max(streamer, instance.min()[0], instance.min().size()))
        return false;
      if (!streamer.finish_consecutive())
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!streamer.start_consecutive(true, true))
        return false;
      if (!max(streamer, instance.max()[0], instance.max().size()))
        return false;
      if (!streamer.finish_consecutive())
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
bool max(S& str, const ::cloud_msgs::msg::WeatherForcastTemperature& instance, bool as_key) {
  auto &props = get_type_props<::cloud_msgs::msg::WeatherForcastTemperature>();
  str.set_mode(cdr_stream::stream_mode::max, as_key);
  return max(str, instance, props.data()); 
}

} //namespace cdr
} //namespace core
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

#endif // DDSCXX_CLOUD_HPP
