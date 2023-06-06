#include "MQTTHandler.h"
#include "RealTime.h"
#include "WiFi.h"
#include "GlobalVariables.h"
#include <thread>
#include <mutex>

MQTTHandler::MQTTHandler(const char *ssid, const char *password, RealTime &realTimeHandler, EspMQTTClient &client, Timer &sendTimer) : m_realTimeHandler(realTimeHandler), m_client(client), m_sendTimer(sendTimer)
{
    strcpy(m_ssid, ssid);
    strcpy(m_password, password);
    WiFi.begin(m_ssid, m_password);

    WiFi.mode(WIFI_AP); //Already established in WiFiSniffer but just to be safe
}

// If used in a separate task, include vTaskDelay(1) after calling
void MQTTHandler::Loop()
{
    if (m_sendTimer.Elapsed())
    {
        //Restart timer before mutex wait to keep to a consistent interval
        m_sendTimer.Start();
        
        SendData();
    }

    m_client.loop();
}

/// @brief Sends the current mac data in a thread-safe way
void MQTTHandler::SendData()
{
    //Wait until we receive the lock
    std::lock_guard<std::mutex> lock(macManipulationgMutex);

    // We have the lock
    for (MacData data : macData)
    {
        StaticJsonDocument<MQTT_BUFFER_LENGTH> doc;
        char mqttBuffer[MQTT_BUFFER_LENGTH];
        
        //We have the lock until the end of the scope
        doc["Name"] = DEVICE_NAME;
        doc["Time"] = m_realTimeHandler.GetLocalTimeAsString();
        doc["Channel"] = data.channel;
        doc["RSSI"] = data.rssi;
        doc["Receiver"] = DEVICE_NAME; //Note: Not saved, just pass DEVICE_NAME for now. Remove if it causes issues with node-red
        doc["Sender"] = data.macAddress;
        //optional
        doc["Randomized"] = data.IsRandomizedMac();

        serializeJson(doc, mqttBuffer);
        m_client.publish("Wifisniffer/data", mqttBuffer);
    }
}