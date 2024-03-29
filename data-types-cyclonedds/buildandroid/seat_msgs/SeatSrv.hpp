/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to CXX Translator
  File name: SeatSrv.idl
  Source: SeatSrv.hpp
  Cyclone DDS: v0.10.0

*****************************************************************/
#ifndef DDSCXX_SEATSRV_HPP
#define DDSCXX_SEATSRV_HPP

#include <string>

namespace seat_msgs
{
namespace srv
{
const std::string TOPIC_SET_SEAT_POSITION = "seat_msgs/srv/SetPosition";

const std::string TOPIC_SET_SEAT_HEAT = "seat_msgs/srv/SetHeat";

const std::string TOPIC_SET_SEAT_VENTILATE = "seat_msgs/msg/SetVentilate";

}

}

#include "dds/topic/TopicTraits.hpp"
#include "org/eclipse/cyclonedds/topic/datatopic.hpp"

namespace org {
namespace eclipse {
namespace cyclonedds {
namespace topic {

} //namespace topic
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

namespace dds {
namespace topic {

}
}


namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

} //namespace cdr
} //namespace core
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

#endif // DDSCXX_SEATSRV_HPP
