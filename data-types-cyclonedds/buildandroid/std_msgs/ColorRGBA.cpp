/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to CXX Translator
  File name: ColorRGBA.idl
  Source: ColorRGBA.cpp
  Cyclone DDS: v0.10.0

*****************************************************************/
#include "ColorRGBA.hpp"

namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

template<>
propvec &get_type_props<::std_msgs::msg::ColorRGBA>() {
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
  props.push_back(entity_properties_t(1, 0, false, get_bit_bound<float>(), extensibility::ext_final, false));  //::r
  props.push_back(entity_properties_t(1, 1, false, get_bit_bound<float>(), extensibility::ext_final, false));  //::g
  props.push_back(entity_properties_t(1, 2, false, get_bit_bound<float>(), extensibility::ext_final, false));  //::b
  props.push_back(entity_properties_t(1, 3, false, get_bit_bound<float>(), extensibility::ext_final, false));  //::a

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

