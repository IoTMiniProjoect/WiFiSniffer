#pragma once

#include <stdint.h>
#include <vector>
#include <set>
#include <functional>
#include "esp_wifi.h"
#include "CustomWiFiStructs.h"

#define RSSIBoundry_NONE 0x10

class WiFiSniffer
{
    private:
        static std::function<void(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type)> m_promiscuousPacketHandler;
        static std::set<std::vector<uint8_t>> m_detectedMacs;

        //Disalow outside creation
        WiFiSniffer() = default;

        static void PromisciousPacketHandlerWrapper(void *buffer, wifi_promiscuous_pkt_type_t type);

        static int m_farthestRSSI;
        static int m_closestRSSI;

        //TODO: Add a MAC Handler class?
    
    public:
        WiFiSniffer(WiFiSniffer &other) = delete;
        void operator=(const WiFiSniffer & rVal) = delete;
        ~WiFiSniffer() = default;

        static WiFiSniffer& Instance();

        bool SetUp();
        void SetPromiscuousPacketHandlerCallbackFunction(std::function<void(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type)> callback);
        bool SetWiFiChannel(uint8_t channel);
        static std::set<std::vector<uint8_t>> GetDetectedMacs();
        bool SetRSSIRange(int farthestRSSI, int closestRSSI = 0);
};
