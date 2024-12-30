#pragma once
#include <cstdint>

struct TunnelPacket {
    uint16_t srcPort;
    uint16_t destPort;
    uint32_t srcIP;
    uint32_t destIP;

    static constexpr size_t MAX_PAYLOAD_SIZE = 1024;
    char payload[MAX_PAYLOAD_SIZE];
};
