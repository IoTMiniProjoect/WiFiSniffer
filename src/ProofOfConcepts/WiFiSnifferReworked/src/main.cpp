#include <Arduino.h>
// //#include "freertos/FreeRTOS.h"
// #include "esp_wifi.h"
// #include "esp_wifi_types.h"
// // #include "esp_system.h"
// //#include "nvs_flash.h"

// #include "WiFi.h"
// #include "DebugPrint.h"
// #include "string"
// #include "sstream"

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

