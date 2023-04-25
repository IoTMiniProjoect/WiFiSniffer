#include "MACAddressUtilities.h"
#include "DebugPrint.h"

bool MACAddressUtilities::IsRandomizedMAC(std::vector<uint8_t> mac)
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

bool MACAddressUtilities::IsRandomizedMAC(std::string mac)
{
    //TODO...
    #warning "Not implemented yet"
    DEBUG_PRINTLN("[!] Not implemented");

    return false;
}