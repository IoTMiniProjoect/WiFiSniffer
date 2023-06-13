#pragma once

#include "ArduinoJson.h"
#include "EspMQTTClient.h"
#include "TimerModule.h"
#include "RealTime.h"

#define MQTT_BUFFER_LENGTH 256
#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_TOPIC "Wifisniffer/data"

class MQTTHandler
{
private:
    RealTime &m_realTimeHandler;
    Timer &m_sendTimer;
    EspMQTTClient &m_client;

    char m_ssid[50];
    char m_password[30];
public:
    MQTTHandler(const char *ssid, const char *password, RealTime &realTimeHandler, EspMQTTClient &client, Timer &sendTimer);
    ~MQTTHandler() = default;

    void SendData();

    void Loop();
};