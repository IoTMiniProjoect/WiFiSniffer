#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
// #include "esp_system.h"
//#include "nvs_flash.h"

#include "WiFi.h"
#include "DebugPrint.h"
#include "string"
#include "sstream"

#define SECONDS *1000
//Undefine to show all devices
//#define ONLY_CLOSE 
#define MAX_RSSI -30    //The lower the number, the closer the device is

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
bool SetWiFiChannel(uint8_t channel);

bool SetWiFiChannel(uint8_t channel)
{
    return esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE) == ESP_OK;
}


void setup_alt()
{
    Serial.begin(115200);
    //If this doesn't work, try setup_alt
    WiFi.begin();
    WiFi.mode(WIFI_AP);

    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(&PromiscuousPacketHandler);

}

void setup()
{
    Serial.begin(115200);
    //ESP_ERROR_CHECK(nvs_flash_init());
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
}

void loop()
{
    //Nothing runs here, event driven code
    // put your main code here, to run repeatedly:
    //delay(5 SECONDS);

    vTaskDelay(5 SECONDS / portTICK_PERIOD_MS);
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

    std::stringstream messageStream;
    messageStream << "Packet type: MGMT\n" <<
                     "Channel: " << packet->rx_ctrl.channel << '\n' <<
                     "RSSI: " << packet->rx_ctrl.rssi << '\n' << 
                     "Receiver: " << std::hex << (int)header->addr1[0] << ":" 
                                  << std::hex << (int)header->addr1[1] << ":"
                                  << std::hex << (int)header->addr1[2] << ":"
                                  << std::hex << (int)header->addr1[3] << ":"
                                  << std::hex << (int)header->addr1[4] << ":"
                                  << std::hex << (int)header->addr1[5] << '\n' <<
                     "Sender: " << std::hex << (int)header->addr2[0] << ":" 
                                  << std::hex << (int)header->addr2[1] << ":"
                                  << std::hex << (int)header->addr2[2] << ":"
                                  << std::hex << (int)header->addr2[3] << ":"
                                  << std::hex << (int)header->addr2[4] << ":"
                                  << std::hex << (int)header->addr2[5];
                     

    
    DEBUG_PRINTLN(messageStream.str().data());
}

