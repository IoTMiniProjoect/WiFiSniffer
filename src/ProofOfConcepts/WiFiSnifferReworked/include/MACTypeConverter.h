#pragma once

#include <string>
#include <vector>
#include <stdint.h>

namespace MACTypeConverter
{
    std::vector<uint8_t> GetVectorFromString(const std::string &macAddress);
    std::vector<uint8_t> GetVectorFromArray(const uint8_t (&macArray)[6]);   //Assumed size of 6

    /// @brief Converts the Provided MAC address container to a string
    /// @tparam Iterator The iterator of the container
    /// @param start The begining of the container
    /// @param end The end of the container
    /// @return The generated string address
    template <typename Iterator>
    std::string MACTypeConverter::ToString(Iterator start, Iterator end)
    {
        //Template has to be in the header
        std::stringstream ss;

        ss << std::hex << std::setfill('0');

        for (Iterator i = start; i < end; i++)
        {
            ss << std::uppercase << std::hex << std::setw(2) << static_cast<int>(*i) << ':';
        }

        std::string result = ss.str();
        result.pop_back();
        return result;
    }
}