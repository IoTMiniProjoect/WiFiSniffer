#include <Arduino.h>

#include "WiFiSniffer.h"
#include "TimerModule.h"
#include "MACAddressUtilities.h"
#include "DebugPrint.h"

//Undefine to show all devices
//#define ONLY_CLOSE 
//#define MAX_RSSI -30    //The lower the number, the closer the device is

WiFiSniffer &Sniffer = WiFiSniffer::Instance();
Timer printTimer = Timer(5 SECONDS);

void setup()
{
    Serial.begin(115200);
    Sniffer.SetUp();

    printTimer.Start();

    DEBUG_PRINTLN("[+] Setup complete");
}

void loop()
{
    if (printTimer.Elapsed())
    {
        DEBUG_PRINTLN("MAC Addresses Found:");

        std::string detectedMacs = MACAddressUtilities::GetMacCollectionAsString(Sniffer.GetDetectedMacs());

        DEBUG_PRINTLN(detectedMacs.c_str());

        printTimer.Start();

        DEBUG_PRINTLN(std::string(30, '=').c_str());
    }
    Sniffer.SetRSSIRange(RSSIBoundry_NONE);
}

