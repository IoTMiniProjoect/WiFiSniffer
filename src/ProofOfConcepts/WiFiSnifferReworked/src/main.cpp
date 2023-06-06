#include <Arduino.h>

#include "WiFiSniffer.h"
#include "TimerModule.h"
#include "MACAddressUtilities.h"
#include "DebugPrint.h"
#include "PromiscuousPacketHandlers.h"

#define WIFI_CHANNEL 1

WiFiSniffer &Sniffer = WiFiSniffer::Instance();
Timer printTimer = Timer(5 SECONDS);

void setup()
{
    Serial.begin(115200);

    Sniffer.SetUp();
    //Sniffer.SetRSSIRange(RSSIBoundry_NONE);
    Sniffer.SetRSSIRange(-40);
    Sniffer.SetPromiscuousPacketHandlerCallbackFunction(&SilentPromiscuousPacketHandler);
    Sniffer.SetWiFiChannel(WIFI_CHANNEL);
    
    //Values for testing
    Sniffer.SetMacTimeout(1 MINUTES);

    printTimer.Start();

    DEBUG_PRINTLN("[+] Setup complete");
}

void loop()
{
    Sniffer.Handle();

    if (printTimer.Elapsed())
    {
        if (Sniffer.GetCurrentMacsCount() > 0)
        {
            std::string macAddressesInfoMessage = "Current devices: " + std::to_string(Sniffer.GetCurrentMacsCount());
            DEBUG_PRINTLN(macAddressesInfoMessage.c_str());

            std::string detectedMacStrings = Sniffer.GetDetectedMacDataAsPrettyString();
            DEBUG_PRINTLN(detectedMacStrings.c_str());
            DEBUG_PRINTLN(std::string(30, '=').c_str());
        }
        else
        {
            DEBUG_PRINT(".");
        }

        printTimer.Start();
    }
}

