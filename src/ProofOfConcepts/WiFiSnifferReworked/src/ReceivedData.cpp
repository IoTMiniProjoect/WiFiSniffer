#include "ReceivedData.h"
#include <string>
#include "MACTypeConverter.h"
#include "MACAddressUtilities.h"

ReceivedData::ReceivedData(std::vector<uint8_t> senderMacAddress, uint32_t timestamp, uint8_t channel, int rssi)
{
    this->senderMacAddress = senderMacAddress;
    this->timestamp = timestamp;
    this->channel = channel;
    this->rssi = rssi;
}

std::string ReceivedData::ToString()
{
    std::string result =  "";
    result += "Timestamp: " + std::to_string(timestamp) + " || ";
    result += "Channel: " + std::to_string(channel) + " || ";
    result += "Mac: " + MACTypeConverter::ToString(std::begin(senderMacAddress), std::end(senderMacAddress)) + " || ";
    result += "RSSI: " + std::to_string(rssi);

    return result;
}

bool ReceivedData::IsRandomizedMac()
{
    return MACAddressUtilities::IsRandomizedMAC(senderMacAddress);
}
