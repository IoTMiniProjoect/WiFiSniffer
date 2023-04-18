#pragma once

#include <vector>
#include <stdint.h>

struct DeviceData final
{
    uint32_t Timestamp;
    int8_t RSSI;
    std::vector<uint8_t> MACaddress;

    DeviceData() = default;
    DeviceData(uint32_t timestamp, int8_t RSSI, std::vector<uint8_t> MACaddress)
    {
        this->Timestamp = timestamp;
        this->RSSI = RSSI;
        this->MACaddress = MACaddress;
    }
};
