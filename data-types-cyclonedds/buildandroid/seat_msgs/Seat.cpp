/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to CXX Translator
  File name: Seat.idl
  Source: Seat.cpp
  Cyclone DDS: v0.10.0

*****************************************************************/
#include "Seat.hpp"

namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

template<>
propvec &get_type_props<::seat_msgs::msg::SeatMotorIdent_T>() {
  static thread_local std::mutex mtx;
  static thread_local propvec props;
  static thread_local entity_properties_t *props_end = nullptr;
  static thread_local std::atomic_bool initialized {false};
  key_endpoint keylist;
  if (initialized.load(std::memory_order_relaxed)) {
    auto ptr = props.data();
    while (ptr < props_end)
      (ptr++)->is_present = false;
    return props;
  }
  std::lock_guard<std::mutex> lock(mtx);
  if (initialized.load(std::memory_order_relaxed)) {
    auto ptr = props.data();
    while (ptr < props_end)
      (ptr++)->is_present = false;
    return props;
  }
  props.clear();

  props.push_back(entity_properties_t(0, 0, false, bb_unset, extensibility::ext_final));  //root
  props.push_back(entity_properties_t(1, 0, false, get_bit_bound<int32_t>(), extensibility::ext_final, false));  //::zone
  props.push_back(entity_properties_t(1, 1, false, get_bit_bound<int32_t>(), extensibility::ext_final, false));  //::axle

  entity_properties_t::finish(props, keylist);
  props_end = props.data() + props.size();
  initialized.store(true, std::memory_order_release);
  return props;
}

template<>
propvec &get_type_props<::seat_msgs::msg::Position>() {
  static thread_local std::mutex mtx;
  static thread_local propvec props;
  static thread_local entity_properties_t *props_end = nullptr;
  static thread_local std::atomic_bool initialized {false};
  key_endpoint keylist;
  if (initialized.load(std::memory_order_relaxed)) {
    auto ptr = props.data();
    while (ptr < props_end)
      (ptr++)->is_present = false;
    return props;
  }
  std::lock_guard<std::mutex> lock(mtx);
  if (initialized.load(std::memory_order_relaxed)) {
    auto ptr = props.data();
    while (ptr < props_end)
      (ptr++)->is_present = false;
    return props;
  }
  props.clear();

  props.push_back(entity_properties_t(0, 0, false, bb_unset, extensibility::ext_final));  //root
  props.push_back(entity_properties_t(1, 0, false, get_bit_bound<::seat_msgs::msg::SeatMotorIdent_T>(), extensibility::ext_final, false));  //::id
  entity_properties_t::append_struct_contents(props, get_type_props<::seat_msgs::msg::SeatMotorIdent_T>());  //internal contents of ::id
  props.push_back(entity_properties_t(1, 1, false, get_bit_bound<int32_t>(), extensibility::ext_final, false));  //::angle
    keylist.add_key_endpoint(std::list<uint32_t>{0, 0});
    keylist.add_key_endpoint(std::list<uint32_t>{0, 1});

  entity_properties_t::finish(props, keylist);
  props_end = props.data() + props.size();
  initialized.store(true, std::memory_order_release);
  return props;
}

template<>
propvec &get_type_props<::seat_msgs::msg::Heat>() {
  static thread_local std::mutex mtx;
  static thread_local propvec props;
  static thread_local entity_properties_t *props_end = nullptr;
  static thread_local std::atomic_bool initialized {false};
  key_endpoint keylist;
  if (initialized.load(std::memory_order_relaxed)) {
    auto ptr = props.data();
    while (ptr < props_end)
      (ptr++)->is_present = false;
    return props;
  }
  std::lock_guard<std::mutex> lock(mtx);
  if (initialized.load(std::memory_order_relaxed)) {
    auto ptr = props.data();
    while (ptr < props_end)
      (ptr++)->is_present = false;
    return props;
  }
  props.clear();

  props.push_back(entity_properties_t(0, 0, false, bb_unset, extensibility::ext_final));  //root
  props.push_back(entity_properties_t(1, 0, false, get_bit_bound<int16_t>(), extensibility::ext_final, false));  //::level
  props.push_back(entity_properties_t(1, 1, false, get_bit_bound<int32_t>(), extensibility::ext_final, false));  //::zone
    keylist.add_key_endpoint(std::list<uint32_t>{1});

  entity_properties_t::finish(props, keylist);
  props_end = props.data() + props.size();
  initialized.store(true, std::memory_order_release);
  return props;
}

template<>
propvec &get_type_props<::seat_msgs::msg::Ventilate>() {
  static thread_local std::mutex mtx;
  static thread_local propvec props;
  static thread_local entity_properties_t *props_end = nullptr;
  static thread_local std::atomic_bool initialized {false};
  key_endpoint keylist;
  if (initialized.load(std::memory_order_relaxed)) {
    auto ptr = props.data();
    while (ptr < props_end)
      (ptr++)->is_present = false;
    return props;
  }
  std::lock_guard<std::mutex> lock(mtx);
  if (initialized.load(std::memory_order_relaxed)) {
    auto ptr = props.data();
    while (ptr < props_end)
      (ptr++)->is_present = false;
    return props;
  }
  props.clear();

  props.push_back(entity_properties_t(0, 0, false, bb_unset, extensibility::ext_final));  //root
  props.push_back(entity_properties_t(1, 0, false, get_bit_bound<int16_t>(), extensibility::ext_final, false));  //::level
  props.push_back(entity_properties_t(1, 1, false, get_bit_bound<int32_t>(), extensibility::ext_final, false));  //::zone

  entity_properties_t::finish(props, keylist);
  props_end = props.data() + props.size();
  initialized.store(true, std::memory_order_release);
  return props;
}

template<>
propvec &get_type_props<::seat_msgs::msg::Occupancy>() {
  static thread_local std::mutex mtx;
  static thread_local propvec props;
  static thread_local entity_properties_t *props_end = nullptr;
  static thread_local std::atomic_bool initialized {false};
  key_endpoint keylist;
  if (initialized.load(std::memory_order_relaxed)) {
    auto ptr = props.data();
    while (ptr < props_end)
      (ptr++)->is_present = false;
    return props;
  }
  std::lock_guard<std::mutex> lock(mtx);
  if (initialized.load(std::memory_order_relaxed)) {
    auto ptr = props.data();
    while (ptr < props_end)
      (ptr++)->is_present = false;
    return props;
  }
  props.clear();

  props.push_back(entity_properties_t(0, 0, false, bb_unset, extensibility::ext_final));  //root
  props.push_back(entity_properties_t(1, 0, false, get_bit_bound<int16_t>(), extensibility::ext_final, false));  //::level
  props.push_back(entity_properties_t(1, 1, false, get_bit_bound<int32_t>(), extensibility::ext_final, false));  //::zone

  entity_properties_t::finish(props, keylist);
  props_end = props.data() + props.size();
  initialized.store(true, std::memory_order_release);
  return props;
}

} //namespace cdr
} //namespace core
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

