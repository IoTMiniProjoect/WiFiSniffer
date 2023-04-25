#include "MACTypeConverter.h"
#include <sstream>
#include <iomanip>

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

std::vector<uint8_t> MACTypeConverter::GetVectorFromArray(const uint8_t (&macArray)[6])
{
    std::vector<uint8_t> macVector = std::vector<uint8_t>(std::begin(macArray), std::end(macArray));
    
    return macVector;
}