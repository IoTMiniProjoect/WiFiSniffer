#include <Arduino.h>

#include "WiFiSniffer.h"
#include "TimerModule.h"
#include "MACAddressUtilities.h"
#include "DebugPrint.h"
#include "PromiscuousPacketHandlers.h"

//Undefine to show all devices
//#define ONLY_CLOSE 
//#define MAX_RSSI -30    //The lower the number, the closer the device is

WiFiSniffer &Sniffer = WiFiSniffer::Instance();
Timer printTimer = Timer(5 SECONDS);

void setup()
{
    Serial.begin(115200);

    Sniffer.SetUp();
    Sniffer.SetRSSIRange(RSSIBoundry_NONE);
    Sniffer.SetPromiscuousPacketHandlerCallbackFunction(&SilentPromiscuousPacketHandler);
    
    printTimer.Start();

    DEBUG_PRINTLN("[+] Setup complete");
}

void loop()
{
    Sniffer.Handle();

    if (printTimer.Elapsed())
    {
        std::string macAddressesInfoMessage = "MAC Addresses Found: " + std::to_string(Sniffer.GetCurrentMacsCount());
        DEBUG_PRINTLN(macAddressesInfoMessage.c_str());

        std::string detectedMacStrings = Sniffer.GetDetectedMacDataAsPrettyString();
        DEBUG_PRINTLN(detectedMacStrings.c_str());
        DEBUG_PRINTLN(std::string(30, '=').c_str());

        printTimer.Start();
    }
}

