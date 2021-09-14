#ifndef __hvac_ac_FunctionPoint__
#define __hvac_ac_FunctionPoint__


namespace hvac {
namespace ac {
class FunctionPoint: public AbsFunctionPoint<udp_can_packet_t> {
    virtual void parse(udp_can_packet_t* pack);
};


};// namespace ac
};// namespace hvac


#endif //__hvac_ac_FunctionPoint__
