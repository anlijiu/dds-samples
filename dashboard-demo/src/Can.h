#ifndef __CAN_H__
#define __CAN_H__

typedef struct udp_can_packet_s {
	uint16_t can_id;
	uint8_t can_dlc;
	uint8_t data[8];
} __attribute__((packed)) udp_can_packet_t;


struct MessageMeta {
  std::string name;
  uint8_t dlc;
  std::vector<uint64_t> propZoneIds;

  bool operator==(const MessageMeta& obj2) const
  {
      return name == obj2.name;
  }
};


struct SignalMeta {
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
    SignalMeta(std::string name, uint32_t startBit, uint32_t length,
               double scaling, double offset, double minimum, double maximum,
               bool isSigned, std::string units, int defaultValue) :
        name(name), startBit(startBit), length(length),scaling(scaling), 
        offset(offset), minimum(minimum), maximum(maximum),
        isSigned(isSigned), units(units), defaultValue(defaultValue)
    {}
};

#endif// __CAN_H__
