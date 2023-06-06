#pragma once

#include "esp_wifi.h"
#include "CustomWiFiStructs.h"

void VerbosePromiscuousPacketHandler(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type);

void SilentPromiscuousPacketHandler(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type);