#pragma once

#include <stdint.h>
#include <vector>
#include <string>

struct ReceivedData
{
    std::vector<uint8_t> senderMacAddress;
    uint32_t timestamp;
    uint8_t channel;
    int rssi;

    ReceivedData() = default;
    ReceivedData(std::vector<uint8_t> senderMacAddress, uint32_t timestamp, uint8_t channel, int rssi);
    
    std::string ToString();
    bool IsRandomizedMac();
};