#include "WiFiSniffer.h"

#include "esp_wifi.h"
#include "esp_wifi_types.h"
// #include "esp_system.h"
//#include "nvs_flash.h"
#include "DebugPrint.h"
#include "MACTypeConverter.h"
#include "sstream"
#include "functional"

//src-only function declarations
void PromiscuousPacketHandler(void *buffer, wifi_promiscuous_pkt_type_t type);

/// @brief Get the instance of the singleton class
/// @return The instance
WiFiSniffer& WiFiSniffer::Instance()
{
    static WiFiSniffer instance;
    return instance;
}

/// @brief Sets up the ESP32 as a WiFiSniffer. Assigns the default callback for receiving a promiscuous packet.
/// @return True on success, stuck in an infinite loop otherwise
bool WiFiSniffer::SetUp()
{
    //TODO: Implement Own ESP_ERROR_CHECK?
    ESP_ERROR_CHECK(esp_netif_init());
    DEBUG_PRINTLN("[+] esp_netif_init");
    //esp_event_loop_create_default();
    wifi_init_config_t wifiConfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifiConfig));
    DEBUG_PRINTLN("[+] esp_wifi_init");

    wifi_country_t wifiCountry = {.cc = "NL", .schan = 1, .nchan = 13, .policy = WIFI_COUNTRY_POLICY_AUTO};
    ESP_ERROR_CHECK(esp_wifi_set_country(&wifiCountry));
    DEBUG_PRINTLN("[+] esp_wifi_set_country");

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    DEBUG_PRINTLN("[+] esp_wifi_set_mode");

    ESP_ERROR_CHECK(esp_wifi_start());
    DEBUG_PRINTLN("[+] esp_wifi_start");

    esp_wifi_set_promiscuous(true);
    SetPromiscuousPacketHandlerCallbackFunction(nullptr);

    return true;
}

/// @brief Sets the callback for receiving a promiscous packet.
/// @param callback The address of the callback function. Pass nullptr to get the default handler
void WiFiSniffer::SetPromiscuousPacketHandlerCallbackFunction(wifi_promiscuous_cb_t callback)
{
    if (callback == nullptr)
    {
        m_promiscuousPacketHandler = &WiFiSniffer::DefaultPromiscuousPacketHandler;

    }
    else
    {
        m_promiscuousPacketHandler = callback;
    }

    esp_wifi_set_promiscuous_rx_cb(m_promiscuousPacketHandler);
}

/// @brief Sets the WiFi channel
/// @param channel A number between 1-13 (for 2.4GHz)
/// @return True on success, false on failure
bool WiFiSniffer::SetWiFiChannel(uint8_t channel)
{
    return esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE) == ESP_OK;
}

//Default handler
void WiFiSniffer::DefaultPromiscuousPacketHandler(void *buffer, wifi_promiscuous_pkt_type_t type)
{
    if (type != WIFI_PKT_MGMT)
    {
        //We only care about the management frame
        return;
    }
    
    const wifi_promiscuous_pkt_t *packet = (wifi_promiscuous_pkt_t *)buffer;
    const wifi_ieee80211_packet *ieee8021Packet = (wifi_ieee80211_packet *)packet->payload;
    const wifi_ieee80211_mac_hdr *header = &ieee8021Packet->hdr;

#ifdef ONLY_CLOSE
    if (packet->rx_ctrl.rssi < MAX_RSSI)
    {
        //Too far away
        return;
    }
#endif

    //Note: Untested
    std::string receiverMac = MACTypeConverter::ToString(std::begin(header->addr1), std::end(header->addr1));

    std::string senderMac = MACTypeConverter::ToString(std::begin(header->addr2), std::end(header->addr2));

    std::stringstream messageStream;
    messageStream << "Timestamp: " << packet->rx_ctrl.timestamp << '\n' <<
                     "Channel: " << packet->rx_ctrl.channel << '\n' <<
                     "RSSI: " << packet->rx_ctrl.rssi << '\n' << 
                     "Receiver: " << receiverMac << '\n' <<
                     "Sender: " << senderMac;
    
    DEBUG_PRINTLN(messageStream.str().data());
}

