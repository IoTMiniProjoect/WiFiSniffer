#pragma once

#include <stdint.h>
#include <vector>
#include <string>

struct MacData
{
    std::vector<uint8_t> macAddress;
    uint32_t timestamp;
    uint8_t channel;
    int rssi;
    
    MacData() = default;
    MacData(std::vector<uint8_t> macAddress, uint32_t timestamp, uint8_t channel, int rssi);

    MacData& operator=(const MacData &rVal);
    
    std::string ToString();
    bool IsRandomizedMac();
};