#ifndef __LOG_HPP__
#define __LOG_HPP__

#include "vhal_compatible.hpp"
#include "Can.hpp"

class Log {
public:
    static void printVehicleProperty(const VehiclePropValue& propValue);
    static void printCanFrame(const can_frame_t * frame);
};


#endif //__LOG_HPP__
