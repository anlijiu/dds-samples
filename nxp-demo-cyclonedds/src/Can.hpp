#ifndef __CAN_H__
#define __CAN_H__

#include <string>
#include <vector>

#define CAN_FRAME_MAX_DATA_LEN 8

typedef struct can_frame_s {
	uint16_t can_id;
	uint8_t can_dlc;
	uint8_t data[CAN_FRAME_MAX_DATA_LEN];
} __attribute__((packed)) can_frame_t;


struct CanMessageMeta {
  std::string name;
  uint8_t dlc;
  std::vector<uint64_t> propZoneIds;

  bool operator==(const CanMessageMeta& obj2) const
  {
      return name == obj2.name;
  }
};


struct CanSignalMeta {
    std::string name;
    uint32_t startBit;
    uint32_t length;
    double scaling;
    double offset;
    double minimum;
    double maximum;
    bool isSigned;
    std::string units;
    int defaultValue;
    CanSignalMeta(std::string name, uint32_t startBit, uint32_t length,
               double scaling, double offset, double minimum, double maximum,
               bool isSigned, std::string units, int defaultValue) :
        name(name), startBit(startBit), length(length),scaling(scaling), 
        offset(offset), minimum(minimum), maximum(maximum),
        isSigned(isSigned), units(units), defaultValue(defaultValue)
    {}
};

#endif// __CAN_H__
