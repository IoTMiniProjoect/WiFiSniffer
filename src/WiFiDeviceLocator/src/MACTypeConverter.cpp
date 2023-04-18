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

std::string MACTypeConverter::GetStringFromVector(const std::vector<uint8_t> &macAddress)
{
    std::stringstream ss;

    ss << std::hex << std::setfill('0');

    for (int i = 0; i < macAddress.size(); i++)
    {
        ss << std::uppercase << std::hex << std::setw(2) << static_cast<int>(macAddress[i]) << ':';
    }

    std::string result = ss.str();
    result.pop_back();
    return result;
}
