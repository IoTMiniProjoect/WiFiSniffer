#include "MACAddressUtilities.h"
#include "MACTypeConverter.h"
#include "DebugPrint.h"

/// @brief Determines if a MAC address is randomized or not
/// @param mac The MAC address to check
/// @return True if the MAC is randomized, false otherwise
bool MACAddressUtilities::IsRandomizedMAC(const std::vector<uint8_t> &mac)
{
    uint8_t macMSB = mac[0];
    //x_2, x_6, x_A, x_E => randomized mac
    const uint8_t mask = 0b10; 

    if ((macMSB & mask) == 0)
    {
        //Not a random mac address, does not match the mask
        return false;
    }

    //Random mac address, matches the mask
    return true;
}