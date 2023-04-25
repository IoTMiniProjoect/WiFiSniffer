#pragma once

#include <vector>
#include <stdint.h>
#include <string>

namespace MACAddressUtilities
{
    bool IsRandomizedMAC(std::vector<uint8_t> mac);
    bool IsRandomizedMAC(std::string mac);
}