#pragma once

#include <cstdint>
#include <cstring>

struct TunnelPacket {
    uint16_t srcPort;
    uint16_t destPort;
    uint32_t srcIP;
    uint32_t destIP;

    static constexpr size_t MAX_PAYLOAD_SIZE = 1024;
    char payload[MAX_PAYLOAD_SIZE];

    TunnelPacket() : srcPort(0), destPort(0), srcIP(0), destIP(0) {
        std::memset(payload, 0, MAX_PAYLOAD_SIZE);
    }
};
