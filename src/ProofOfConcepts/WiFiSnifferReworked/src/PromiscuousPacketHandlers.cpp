#include "PromiscuousPacketHandlers.h"
#include "vector"
#include <string>
#include "DebugPrint.h"
#include "MACTypeConverter.h"
#include "ReceivedData.h"


void VerbosePromiscuousPacketHandler(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type)
{
    const wifi_ieee80211_packet *ieee8021Packet = (wifi_ieee80211_packet *)packet->payload;
    const wifi_ieee80211_mac_hdr *header = &ieee8021Packet->hdr;
    
    //addr1 -> receiver (esp), addr2 -> sender
    std::vector<uint8_t> senderMac = MACTypeConverter::GetVectorFromArray(header->addr2);

    ReceivedData data = ReceivedData(senderMac, packet->rx_ctrl.timestamp, packet->rx_ctrl.channel, packet->rx_ctrl.rssi);

    DEBUG_PRINTLN(data.ToString().c_str());
}

void SilentPromiscuousPacketHandler(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type)
{
    #warning Not implemented
}