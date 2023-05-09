#include "MACAddressUtilities.h"
#include "MACTypeConverter.h"
#include "DebugPrint.h"

bool MACAddressUtilities::IsRandomizedMAC(std::vector<uint8_t> &mac)
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
    //Needed? User can just convert themselves
    //TODO...
    #warning "Not implemented"
    DEBUG_PRINTLN("[!] Not implemented");

    return false;
}

std::string MACAddressUtilities::GetMacCollectionAsString(const std::set<std::vector<uint8_t>> &macsColletion)
{
    std::string result = "";

    if (macsColletion.size() == 0)
    {
        return result;
    }

    for (std::vector<uint8_t> currentMacVector : macsColletion)
    {
        result += MACTypeConverter::ToString(std::begin(currentMacVector), std::end(currentMacVector)) + "\n";
    }

    result.pop_back();  //Remove trailing newline
    
    return result;
}