#pragma once

#include <stdint.h>

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
} /*__attribute__((packed))*/;

struct wifi_ieee80211_packet
{
	wifi_ieee80211_mac_hdr hdr;
    //tail padding
	uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */ 
} /*__attribute__((packed)) //Same as payload[0]*/;
