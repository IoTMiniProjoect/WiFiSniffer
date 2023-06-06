#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"

#include "WiFi.h"
#include "time.h"
#include "DebugPrint.h"
#include "string"
#include "sstream"

#include "EspMQTTClient.h"   
#include "ArduinoJson.h"

#define BUF_LEN 256
#define SECONDS *1000
//Undefine to show all devices

#define MAX_RSSI -30    //The lower the number, the closer the device is
#define MAX_BUF_SIZE 100

const char* SSID     = "12connect";
const char* PASSWORD = "";



bool messageArrived = false;




TaskHandle_t Task1;
TaskHandle_t Task2;



// Structs
//Note: original uses bitfields
struct wifi_ieee80211_mac_hdr
{
    //Bitfields
	unsigned frame_ctrl:16;
	unsigned duration_id:16;
	uint8_t addr1[6]; /* receiver address */
	uint8_t addr2[6]; /* sender address */
	uint8_t addr3[6]; /* filtering address */
	unsigned sequence_ctrl:16;
	uint8_t addr4[6]; /* optional */
    //Without bitfields
    // uint16_t frame_ctrl;
	// uint16_t duration_id;
	// uint8_t addr1[6]; /* receiver address */
	// uint8_t addr2[6]; /* sender address */
	// uint8_t addr3[6]; /* filtering address */
	// uint16_t sequence_ctrl;
	// uint8_t addr4[6]; /* optional */
};

struct wifi_ieee80211_packet
{
	wifi_ieee80211_mac_hdr hdr;
    //tail padding
	uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */ 
};

//Function declarations
void setup_alt();
void PromiscuousPacketHandler(void *buffer, wifi_promiscuous_pkt_type_t type);


void ReadPacketsTask(void* parameter)
{
    ESP_ERROR_CHECK(esp_netif_init());
    DEBUG_PRINTLN("[+] esp_netif_init");
    //esp_event_loop_create_default();
    wifi_init_config_t wifiConfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifiConfig));
    DEBUG_PRINTLN("[+] esp_wifi_init");

    //TODO: Do you need to set country?
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    DEBUG_PRINTLN("[+] esp_wifi_set_mode");

    ESP_ERROR_CHECK(esp_wifi_start());
    DEBUG_PRINTLN("[+] esp_wifi_start");
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(&PromiscuousPacketHandler);

    for(;;)
    {
        vTaskDelay(1);
    }
}

#pragma region WIFI

std::string nameString = "ESP3";
std::string timeString = "";
std::string receiverString = "";
std::string senderString = "";
int channel = 0;
int RSSI = 0;

EspMQTTClient client(
  SSID,
  PASSWORD,
  "test.mosquitto.org", // MQTT Broker
  "",   // Can be omitted if not needed
  "",   // Can be omitted if not needed
  "",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

std::string GetLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time\n");
        return "";
    }
    std::stringstream timeStream;
    timeStream << timeinfo.tm_hour + 1;
    timeStream << ':';
    timeStream << timeinfo.tm_min;
    timeStream << ':';
    timeStream << timeinfo.tm_sec;
    return timeStream.str();
}

void WifiTask(void* parameter)
{
    //WiFi Setup
    //NOTE: possible failing point
    WiFi.begin(SSID,PASSWORD);

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    GetLocalTime();
    WiFi.mode(WIFI_AP); //Remove, already set in WiFiSniffer

    //WiFi Loop
    for(;;)
    {
        timeString = GetLocalTime();    //TODO: Move this inside the if() to prevent unnecessary calls?
        if (messageArrived == true) //TODO: Replace with your own print timer
        {
            messageArrived = false; //TODO: replace with timer.Start()
            char mqttBuf[BUF_LEN];
            StaticJsonDocument<BUF_LEN> doc;    //Move as global for optimization and do doc.clear(); ?

            //TODO: Extract to method
            doc["Name"] = nameString;
            doc["Time"] = timeString;
            doc["Channel"] = channel;
            doc["RSSI"] = RSSI;
            doc["Receiver"] = receiverString;
            doc["Sender"] = senderString;
            serializeJson(doc, mqttBuf);
            client.publish("Wifisniffer/data", mqttBuf);
        }
        client.loop();
        vTaskDelay(1);
    }
}

void setup()
{
    Serial.begin(115200);
    
    xTaskCreatePinnedToCore(WifiTask, "MQTT", 4000, NULL, 1, &Task1, 1);
    delay(500);
    //TODO: Implement the other sniffer here
    xTaskCreatePinnedToCore(ReadPacketsTask, "Packets", 2000, NULL, 1, &Task2, 0);
}

void loop()
{
    //Nothing runs here, event driven code
}

void onConnectionEstablished()
{
    client.subscribe("Wifisniffer/data", [](const String & payload) {
    Serial.println(payload);
  });
}


void PromiscuousPacketHandler(void *buffer, wifi_promiscuous_pkt_type_t type)
{
    if (type != WIFI_PKT_MGMT)
    {
        //We only care about the management frame
        return;
    }

    //const wifi_promiscuous_pkt_t *packet = static_cast<wifi_promiscuous_pkt_t *>(buffer);   //Note: use c-style cast if it doesn't work 
    
    const wifi_promiscuous_pkt_t *packet = (wifi_promiscuous_pkt_t *)buffer;   //Note: use c-style cast if it doesn't work 

    const wifi_ieee80211_packet *ieee8021Packet = (wifi_ieee80211_packet *)packet->payload;
    
    const wifi_ieee80211_mac_hdr *header = &ieee8021Packet->hdr;

#ifdef ONLY_CLOSE
    if (packet->rx_ctrl.rssi < MAX_RSSI)
    {
        //Too far away
        return;
    }
#endif

    channel = packet->rx_ctrl.channel;
    RSSI = packet->rx_ctrl.rssi;

    std::stringstream receiverStream;
    receiverStream << std::hex << (int)header->addr1[0] << ":"
                   << std::hex << (int)header->addr1[1] << ":"
                   << std::hex << (int)header->addr1[2] << ":"
                   << std::hex << (int)header->addr1[3] << ":"
                   << std::hex << (int)header->addr1[4] << ":"
                   << std::hex << (int)header->addr1[5];
    receiverString = receiverStream.str();

    std::stringstream senderStream;
    senderStream << std::hex << (int)header->addr2[0] << ":" 
                 << std::hex << (int)header->addr2[1] << ":"
                 << std::hex << (int)header->addr2[2] << ":"
                 << std::hex << (int)header->addr2[3] << ":"
                 << std::hex << (int)header->addr2[4] << ":"
                 << std::hex << (int)header->addr2[5];
    senderString = senderStream.str();
    messageArrived = true;

    // std::stringstream messageStream;
    // messageStream << "Packet type: MGMT\n" <<
    //                  "Time: " << timeString << '\n' <<
    //                  "Channel: " << packet->rx_ctrl.channel << '\n' <<
    //                  "RSSI: " << packet->rx_ctrl.rssi << '\n' << 
    //                  "Receiver: " << receiverString << '\n' << 
    //                  "Sender: " << senderString;

    //DEBUG_PRINTLN(messageStream.str().data());
}

