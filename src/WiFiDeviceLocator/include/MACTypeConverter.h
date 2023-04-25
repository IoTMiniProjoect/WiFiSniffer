#pragma once

#include <string>
#include <vector>
#include <stdint.h>

// class MACTypeConverter final
// {
//     private:
//         //Creation not allowed
//         MACTypeConverter();
//     public:
//         ~MACTypeConverter() = default;
//         static std::vector<uint8_t> GetVectorFromString(const std::string &macAddress);
//         static std::string GetStringFromVector(const std::vector<uint8_t> &macAddress);
// };

namespace MACTypeConverter
{
    std::vector<uint8_t> GetVectorFromString(const std::string &macAddress);
    std::string GetStringFromVector(const std::vector<uint8_t> &macAddress);
}