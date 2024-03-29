/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to CXX Translator
  File name: SetFanSpeed.idl
  Source: /home/an/workspace/dds/dds-samples/data-types-cyclonedds/buildandroid/hvac_msgs/srv/SetFanSpeed.hpp
  Cyclone DDS: v0.10.0

*****************************************************************/
#ifndef DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_HVAC_MSGS_SRV_SETFANSPEED_HPP
#define DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_HVAC_MSGS_SRV_SETFANSPEED_HPP

#include <cstdint>

namespace hvac_msgs
{
namespace srv
{
class SetFanSpeedRequest
{
private:
 uint8_t value_ = 0;
 int32_t zone_ = 0;

public:
  SetFanSpeedRequest() = default;

  explicit SetFanSpeedRequest(
    uint8_t value,
    int32_t zone) :
    value_(value),
    zone_(zone) { }

  uint8_t value() const { return this->value_; }
  uint8_t& value() { return this->value_; }
  void value(uint8_t _val_) { this->value_ = _val_; }
  int32_t zone() const { return this->zone_; }
  int32_t& zone() { return this->zone_; }
  void zone(int32_t _val_) { this->zone_ = _val_; }

  bool operator==(const SetFanSpeedRequest& _other) const
  {
    (void) _other;
    return value_ == _other.value_ &&
      zone_ == _other.zone_;
  }

  bool operator!=(const SetFanSpeedRequest& _other) const
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

template <> constexpr const char* TopicTraits<::hvac_msgs::srv::SetFanSpeedRequest>::getTypeName()
{
  return "hvac_msgs::srv::SetFanSpeedRequest";
}

template <> constexpr bool TopicTraits<::hvac_msgs::srv::SetFanSpeedRequest>::isKeyless()
{
  return true;
}

#ifdef DDSCXX_HAS_TYPE_DISCOVERY
template<> constexpr unsigned int TopicTraits<::hvac_msgs::srv::SetFanSpeedRequest>::type_map_blob_sz() { return 254; }
template<> constexpr unsigned int TopicTraits<::hvac_msgs::srv::SetFanSpeedRequest>::type_info_blob_sz() { return 100; }
template<> inline const uint8_t * TopicTraits<::hvac_msgs::srv::SetFanSpeedRequest>::type_map_blob() {
  static const uint8_t blob[] = {
 0x4b,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf1,  0x02,  0xa5,  0x81,  0xa2,  0x9c,  0x24,  0x54, 
 0x1e,  0x15,  0xcf,  0xb9,  0x4b,  0x13,  0x1d,  0x00,  0x33,  0x00,  0x00,  0x00,  0xf1,  0x51,  0x01,  0x00, 
 0x01,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x23,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00, 
 0x0b,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x01,  0x00,  0x02,  0x20,  0x63,  0xc1,  0x60,  0x00, 
 0x0b,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x04,  0xa8,  0x4e,  0x5f,  0x25,  0x00, 
 0x83,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0xf2,  0xb8,  0x73,  0xc1,  0xca,  0x3d,  0x4b,  0x23, 
 0xe4,  0xb2,  0x10,  0xfa,  0x6a,  0x05,  0x82,  0x00,  0x6b,  0x00,  0x00,  0x00,  0xf2,  0x51,  0x01,  0x00, 
 0x2b,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x23,  0x00,  0x00,  0x00,  0x68,  0x76,  0x61,  0x63, 
 0x5f,  0x6d,  0x73,  0x67,  0x73,  0x3a,  0x3a,  0x73,  0x72,  0x76,  0x3a,  0x3a,  0x53,  0x65,  0x74,  0x46, 
 0x61,  0x6e,  0x53,  0x70,  0x65,  0x65,  0x64,  0x52,  0x65,  0x71,  0x75,  0x65,  0x73,  0x74,  0x00,  0x00, 
 0x33,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  0x00,  0x14,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00, 
 0x01,  0x00,  0x02,  0x00,  0x06,  0x00,  0x00,  0x00,  0x76,  0x61,  0x6c,  0x75,  0x65,  0x00,  0x00,  0x00, 
 0x13,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00,  0x01,  0x00,  0x04,  0x00,  0x05,  0x00,  0x00,  0x00, 
 0x7a,  0x6f,  0x6e,  0x65,  0x00,  0x00,  0x00,  0x00,  0x22,  0x00,  0x00,  0x00,  0x01,  0x00,  0x00,  0x00, 
 0xf2,  0xb8,  0x73,  0xc1,  0xca,  0x3d,  0x4b,  0x23,  0xe4,  0xb2,  0x10,  0xfa,  0x6a,  0x05,  0x82,  0xf1, 
 0x02,  0xa5,  0x81,  0xa2,  0x9c,  0x24,  0x54,  0x1e,  0x15,  0xcf,  0xb9,  0x4b,  0x13,  0x1d, };
  return blob;
}
template<> inline const uint8_t * TopicTraits<::hvac_msgs::srv::SetFanSpeedRequest>::type_info_blob() {
  static const uint8_t blob[] = {
 0x60,  0x00,  0x00,  0x00,  0x01,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf1,  0x02,  0xa5,  0x81,  0xa2,  0x9c,  0x24,  0x54,  0x1e,  0x15,  0xcf,  0xb9, 
 0x4b,  0x13,  0x1d,  0x00,  0x37,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
 0x00,  0x00,  0x00,  0x00,  0x02,  0x10,  0x00,  0x40,  0x28,  0x00,  0x00,  0x00,  0x24,  0x00,  0x00,  0x00, 
 0x14,  0x00,  0x00,  0x00,  0xf2,  0xb8,  0x73,  0xc1,  0xca,  0x3d,  0x4b,  0x23,  0xe4,  0xb2,  0x10,  0xfa, 
 0x6a,  0x05,  0x82,  0x00,  0x6f,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x00,  0x00,  0x00, 
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
struct topic_type_name<::hvac_msgs::srv::SetFanSpeedRequest>
{
    static std::string value()
    {
      return org::eclipse::cyclonedds::topic::TopicTraits<::hvac_msgs::srv::SetFanSpeedRequest>::getTypeName();
    }
};

}
}

REGISTER_TOPIC_TYPE(::hvac_msgs::srv::SetFanSpeedRequest)

namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

template<>
propvec &get_type_props<::hvac_msgs::srv::SetFanSpeedRequest>();

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool write(T& streamer, const ::hvac_msgs::srv::SetFanSpeedRequest& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!write(streamer, instance.value()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!write(streamer, instance.zone()))
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
bool write(S& str, const ::hvac_msgs::srv::SetFanSpeedRequest& instance, bool as_key) {
  auto &props = get_type_props<::hvac_msgs::srv::SetFanSpeedRequest>();
  str.set_mode(cdr_stream::stream_mode::write, as_key);
  return write(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool read(T& streamer, ::hvac_msgs::srv::SetFanSpeedRequest& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!read(streamer, instance.value()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!read(streamer, instance.zone()))
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
bool read(S& str, ::hvac_msgs::srv::SetFanSpeedRequest& instance, bool as_key) {
  auto &props = get_type_props<::hvac_msgs::srv::SetFanSpeedRequest>();
  str.set_mode(cdr_stream::stream_mode::read, as_key);
  return read(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool move(T& streamer, const ::hvac_msgs::srv::SetFanSpeedRequest& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!move(streamer, instance.value()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!move(streamer, instance.zone()))
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
bool move(S& str, const ::hvac_msgs::srv::SetFanSpeedRequest& instance, bool as_key) {
  auto &props = get_type_props<::hvac_msgs::srv::SetFanSpeedRequest>();
  str.set_mode(cdr_stream::stream_mode::move, as_key);
  return move(str, instance, props.data()); 
}

template<typename T, std::enable_if_t<std::is_base_of<cdr_stream, T>::value, bool> = true >
bool max(T& streamer, const ::hvac_msgs::srv::SetFanSpeedRequest& instance, entity_properties_t *props) {
  (void)instance;
  if (!streamer.start_struct(*props))
    return false;
  auto prop = streamer.first_entity(props);
  while (prop) {
    switch (prop->m_id) {
      case 0:
      if (!streamer.start_member(*prop))
        return false;
      if (!max(streamer, instance.value()))
        return false;
      if (!streamer.finish_member(*prop))
        return false;
      break;
      case 1:
      if (!streamer.start_member(*prop))
        return false;
      if (!max(streamer, instance.zone()))
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
bool max(S& str, const ::hvac_msgs::srv::SetFanSpeedRequest& instance, bool as_key) {
  auto &props = get_type_props<::hvac_msgs::srv::SetFanSpeedRequest>();
  str.set_mode(cdr_stream::stream_mode::max, as_key);
  return max(str, instance, props.data()); 
}

} //namespace cdr
} //namespace core
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

#endif // DDSCXX__HOME_AN_WORKSPACE_DDS_DDS_SAMPLES_DATA_TYPES_CYCLONEDDS_BUILDANDROID_HVAC_MSGS_SRV_SETFANSPEED_HPP
