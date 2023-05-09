#pragma once

#include <vector>
#include <stdint.h>
#include <string>
#include <set>

namespace MACAddressUtilities
{
    bool IsRandomizedMAC(std::vector<uint8_t> &mac);
    bool IsRandomizedMAC(std::string mac);

    std::string GetMacCollectionAsString(const std::set<std::vector<uint8_t>> &macsColletion);
}