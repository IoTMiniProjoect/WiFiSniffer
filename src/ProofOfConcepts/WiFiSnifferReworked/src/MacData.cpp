#include "MacData.h"
#include <string>
#include "MACTypeConverter.h"
#include "MACAddressUtilities.h"

MacData::MacData(std::vector<uint8_t> macAddress, uint32_t timestamp, uint8_t channel, int rssi)
{
    this->macAddress = macAddress;
    this->timestamp = timestamp;
    this->channel = channel;
    this->rssi = rssi;
}

MacData& MacData::operator=(const MacData &rVal)
{
    macAddress = rVal.macAddress;
    timestamp = rVal.timestamp;
    channel = rVal.channel;
    rssi = rVal.rssi;

    return *this;
}

/// @brief Converts the mac data to string, every item is separated by " || "
/// @return The output string
std::string MacData::ToString()
{
    std::string result =  "";
    result += "Timestamp: " + std::to_string(timestamp) + " || ";
    result += "Channel: " + std::to_string(channel) + " || ";
    result += "Mac: " + MACTypeConverter::ToString(std::begin(macAddress), std::end(macAddress)) + " || ";
    result += "RSSI: " + std::to_string(rssi);

    return result;
}

/// @brief Determines if the mac inside the struct is randomized
/// @return True if its randomized, false otherwise
bool MacData::IsRandomizedMac()
{
    return MACAddressUtilities::IsRandomizedMAC(macAddress);
}
