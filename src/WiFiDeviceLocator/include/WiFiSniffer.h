#pragma once

#include <stdint.h>
#include <vector>
#include <set>
#include <functional>
#include "esp_wifi.h"
#include "CustomWiFiStructs.h"
#include "MacHandler.h"

#define RSSIBoundry_NONE 0x10


class WiFiSniffer final
{
    private:
        static std::function<void(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type)> m_promiscuousPacketHandler;

        //Disalow outside creation
        WiFiSniffer() = default;

        static void PromisciousPacketHandlerWrapper(void *buffer, wifi_promiscuous_pkt_type_t type);

        static int m_farthestRSSI;
        static int m_closestRSSI;

        static MacHandler m_macHandler;
    
    public:
        WiFiSniffer(WiFiSniffer &other) = delete;
        void operator=(const WiFiSniffer & rVal) = delete;
        ~WiFiSniffer() = default;

        static WiFiSniffer& Instance();

        bool SetUp();
        void Handle();
        
        void SetPromiscuousPacketHandlerCallbackFunction(std::function<void(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type)> callback);
        bool SetWiFiChannel(uint8_t channel);
        bool SetRSSIRange(int farthestRSSI, int closestRSSI = 0);
        static void SetMacTimeout(uint32_t timeoutMs);
        
        int GetCurrentMacsCount() const;
        std::string GetDetectedMacDataAsPrettyString() const;

};
