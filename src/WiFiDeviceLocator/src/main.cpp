#include <Arduino.h>

#include "WiFiSniffer.h"
#include "TimerModule.h"
#include "MACAddressUtilities.h"
#include "DebugPrint.h"
#include "PromiscuousPacketHandlers.h"
#include "GlobalVariables.h"

//MQTT
#include "MQTTHandler.h"
#include "RealTime.h"

//Extern definitions
std::mutex macManipulationgMutex;
std::vector<MacData> macData;

//Has to be in the file scope cus of onConnectionEstablished()
EspMQTTClient mqttClient = EspMQTTClient(WIFI_SSID, WIFI_PASSWORD, MQTT_BROKER, "", "", "", MQTT_PORT);


TaskHandle_t snifferTaskHandle;

void SnifferThread(void *parameter)
{   
    WiFiSniffer &Sniffer = WiFiSniffer::Instance();
    Timer snifferMacUpdateTimer = Timer(5 SECONDS);

    Sniffer.SetUp();
    //Sniffer.SetRSSIRange(RSSIBoundry_NONE);
    Sniffer.SetRSSIRange(-40);
    Sniffer.SetPromiscuousPacketHandlerCallbackFunction(&SilentPromiscuousPacketHandler);
    Sniffer.SetWiFiChannel(WIFI_CHANNEL);
    //Note: 1 minute for testing, realistically would be 10 minutes
    Sniffer.SetMacTimeout(1 MINUTES);

    snifferMacUpdateTimer.Start();

    DEBUG_PRINTLN("[+] Sniffer setup complete");

    //Loop
    while (true)    //Don't care about stopping it
    {
        Sniffer.Handle();

        if (snifferMacUpdateTimer.Elapsed())
        {
            //Restart timer before acquiring the mutex so we keep to an accurate interval
            snifferMacUpdateTimer.Start();

            if (Sniffer.GetCurrentMacsCount() > 0)
            {
                //Update mac data in a thread-safe way
                {
                    std::lock_guard<std::mutex> lock(macManipulationgMutex);

                    macData = Sniffer.GetMacDataAsVector();
                }

                std::string macAddressesInfoMessage = "Current devices: " + std::to_string(Sniffer.GetCurrentMacsCount());
                DEBUG_PRINTLN(macAddressesInfoMessage.c_str());

                std::string detectedMacStrings = Sniffer.GetDetectedMacDataAsPrettyString();
                DEBUG_PRINTLN(detectedMacStrings.c_str());
                DEBUG_PRINTLN(std::string(30, '=').c_str());
            }
        }
        
        vTaskDelay(1);
    }
}

TaskHandle_t mqttTaskHandle;

void MqttThread(void *parameter)
{
    RealTime realTime;
    Timer mqttDataSendTimer = Timer(10 SECONDS);    //Change if needed

    MQTTHandler mqttHandler = MQTTHandler(WIFI_SSID, WIFI_PASSWORD, realTime, mqttClient, mqttDataSendTimer);
    
    //Note: Are prints thread-safe?
    DEBUG_PRINTLN("[+] MQTT setup complete");

    while (true)    //Don't care about exiting
    {
        mqttHandler.Loop();
        vTaskDelay(1);
    }
    
}

void setup()
{
    Serial.begin(115200);

    xTaskCreatePinnedToCore(&SnifferThread, "Sniffer", 4000 /*Too much/little?*/, NULL, 1, &snifferTaskHandle, 0);
    xTaskCreatePinnedToCore(&MqttThread, "Sniffer", 4000 /*Too much/little?*/, NULL, 1, &mqttTaskHandle, 1);
}

void loop()
{
    //Empty, both cores busy
    //Code here will not be executed
}

//Why does this have to exist...
void onConnectionEstablished()
{
    mqttClient.subscribe("Wifisniffer/data", [](const String & payload) {
    Serial.println(payload);
  });
}