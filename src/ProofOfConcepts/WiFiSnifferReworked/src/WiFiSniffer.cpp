#include "WiFiSniffer.h"

#include "esp_wifi.h"
#include "esp_wifi_types.h"
// #include "esp_system.h"
//#include "nvs_flash.h"
#include "DebugPrint.h"
#include "MACTypeConverter.h"
#include "sstream"
#include "functional"
#include "PromiscuousPacketHandlers.h"

//Static member variable definitions
std::set<std::vector<uint8_t>> WiFiSniffer::m_detectedMacs = std::set<std::vector<uint8_t>>();
int WiFiSniffer::m_farthestRSSI = RSSIBoundry_NONE;
int WiFiSniffer::m_closestRSSI = 0;
std::function<void(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type)> WiFiSniffer::m_promiscuousPacketHandler = {};   //Same as = nullptr

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
    esp_wifi_set_promiscuous_rx_cb(&PromisciousPacketHandlerWrapper);
    
    SetPromiscuousPacketHandlerCallbackFunction(&VerbosePromiscuousPacketHandler);

    return true;
}

/// @brief Sets the callback for receiving a promiscous packet.
/// @param callback The address of the callback function. Pass nullptr to get the default handler
void WiFiSniffer::SetPromiscuousPacketHandlerCallbackFunction(std::function<void(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type)> callback)
{
    m_promiscuousPacketHandler = callback;
}

/// @brief Sets the WiFi channel
/// @param channel A number between 1-13 (for 2.4GHz)
/// @return True on success, false on failure
bool WiFiSniffer::SetWiFiChannel(uint8_t channel)
{
    return esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE) == ESP_OK;
}

std::set<std::vector<uint8_t>> WiFiSniffer::GetDetectedMacs()
{
    return m_detectedMacs;
}

/// @brief Sets the RSSI detection range. 
/// @param farthestRSSI A value less than closestRSSI, the lower the number, the farther away. Use RSSIBoundry_NONE or 0x10 for no limit.
/// @param closestRSSI A value higher than farthestRSSI, maximum of 0.
/// @return True on successfully setting the range, false if the parameters are wrong
bool WiFiSniffer::SetRSSIRange(int farthestRSSI, int closestRSSI)
{
    if (closestRSSI > 0)
    {
        return false;
    }

    if (farthestRSSI > closestRSSI)
    {
        return false;
    }

    return true;
}

/// @brief Wrapper for the promiscuous handler defined by the user. Provides under-the-hood-logic
void WiFiSniffer::PromisciousPacketHandlerWrapper(void *buffer, wifi_promiscuous_pkt_type_t type)
{
    if (type != WIFI_PKT_MGMT)
    {
        //We only care about the management frame
        return;
    }

    const wifi_promiscuous_pkt_t *packet = (wifi_promiscuous_pkt_t *)buffer;
    int rssi = packet->rx_ctrl.rssi;

    if (rssi > m_closestRSSI || rssi < m_farthestRSSI)
    {
        //RSSI not within specified boundaries
        return;
    }

    const wifi_ieee80211_packet *ieee8021Packet = (wifi_ieee80211_packet *)packet->payload;
    const wifi_ieee80211_mac_hdr *header = &ieee8021Packet->hdr;

    //Add mac to the known macs
    m_detectedMacs.insert(MACTypeConverter::GetVectorFromArray(header->addr2));


    //Everything is fine, we can call the worker function
    if (m_promiscuousPacketHandler == nullptr)
    {
        //No handler
        return;
    }

    m_promiscuousPacketHandler(packet, type);
}
