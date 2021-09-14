#include <random>
#include <iostream>
#include <unordered_map>
#include <thread>

#include "CanMessageDistributor.hpp"
#include "CanConfig.h"
#include "Can.h"


CanMessageDistributor::CanMessageDistributor()
    : mSendCanCache((std::bind(&CanMessageDistributor::onFlushCan,
                                  this, std::placeholders::_1)))
{
}

void CanMessageDistributor::start() {
    mReceiveDataThread = std::thread(
                &CanMessageDistributor::receive_loop, this);
}

using random_bytes_engine = std::independent_bits_engine<
    std::default_random_engine, CHAR_BIT, unsigned char>;

void CanMessageDistributor::write(uint32_t canid, uint8_t startbit, uint8_t length,uint64_t value) {
    mSendCanCache.write(canid, startbit, length, value);
}


void CanMessageDistributor::addParseMap(const phmap::flat_hash_map<uint32_t, std::function<void(const uint8_t * data)>>& parseMap) {
    for (auto const& x : parseMap) {
        auto got = mParseMap.find (x.first);

        if ( got == mParseMap.end() ) {
            mParseMap[x.first] = { x.second };
        } else {
            mParseMap[x.first].push_back(x.second);
        }
    }
}

void CanMessageDistributor::onFlushCan(const phmap::flat_hash_map<uint32_t, udp_can_packet_t *>& packetMap) {
    for (auto it = packetMap.cbegin(), next_it = it; it != packetMap.cend(); it = next_it ) {
        ++next_it;
        auto canid = it->first;
        auto packet = it->second;

        std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << canid << " : ";
        for(int i = 0; i < 8; ++i) {
            std::cout << std::setw(2) << std::setfill('0') << std::hex << packet->data[i] << std::endl;
        }
        std::cout << std::endl;
    }
}

void CanMessageDistributor::receive_loop() {
    random_bytes_engine engine;
    udp_can_packet_t *canPacket = (udp_can_packet_t *)malloc(sizeof(struct udp_can_packet_s));
    bzero(canPacket, sizeof(struct udp_can_packet_s));
    canPacket->can_id = 0x334;
    canPacket->data[0] = 0xFF;
    canPacket->data[1] = 0xFF;
    canPacket->data[2] = 0xFF;
    canPacket->data[3] = 0xFF;
    canPacket->data[4] = 0xFF;
    canPacket->data[5] = 0xFF;
    canPacket->data[6] = 0xFF;
    canPacket->data[7] = 0xFF;

    while(1) {
        // for(int i = 0; i < 8; ++i) {
        //     canPacket->data[i] = engine();
        // }

        auto iterator = mParseMap.find(canPacket->can_id);
        if(iterator != mParseMap.end()) { 

            // for(auto it = parsers.begin(); it != parsers.end(); it++) {
            //     (*it)(canPacket->data);
            // }
        
            for(size_t i = 0; i < iterator->second.size(); ++i) {
                auto& parse = iterator->second[i];
                parse(canPacket->data);
            }
            // for(const auto& parse : iterator->second.parsers) {   // Range-for!
            //     parse(canPacket->data);
            // }
        }

        usleep(1000000);
    }

}
