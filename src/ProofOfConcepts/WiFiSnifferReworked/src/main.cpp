#include <Arduino.h>

#include "WiFiSniffer.h"

//Undefine to show all devices
//#define ONLY_CLOSE 
//#define MAX_RSSI -30    //The lower the number, the closer the device is

WiFiSniffer &Sniffer = WiFiSniffer::Instance();

void setup()
{
    Serial.begin(115200);
    Sniffer.SetUp();
}

void loop()
{
    //Nothing runs here, event driven code
}

