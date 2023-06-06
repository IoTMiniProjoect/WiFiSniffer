#include "MACTypeConverter.h"

/// @brief Gets a MAC vector from a mac in a string. The mac octets are separted by a ':'.
/// @param macAddress The string to convert to a mac vector 
/// @return The mac vector
std::vector<uint8_t> MACTypeConverter::GetVectorFromString(const std::string &macAddress)
{
    std::stringstream stringStream(macAddress);
    std::vector<uint8_t> macVector;
    std::string segment;

    while (std::getline(stringStream, segment, ':'))
    {
        macVector.push_back(std::stoi(segment, 0, 16));
    }

    return macVector;
}

/// @brief Gets a Mac vector from a mac array
/// @param macArray The mac array to convert to a vector
/// @return The resulting vector
std::vector<uint8_t> MACTypeConverter::GetVectorFromArray(const uint8_t (&macArray)[6])
{
    std::vector<uint8_t> macVector = std::vector<uint8_t>(std::begin(macArray), std::end(macArray));
    
    return macVector;
}