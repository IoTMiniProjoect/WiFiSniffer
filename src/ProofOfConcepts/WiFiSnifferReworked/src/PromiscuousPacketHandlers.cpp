#include "PromiscuousPacketHandlers.h"
#include <string>
#include <sstream>
#include "DebugPrint.h"
#include "MACTypeConverter.h"


void VerbosePromiscuousPacketHandler(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type)
{
    const wifi_ieee80211_packet *ieee8021Packet = (wifi_ieee80211_packet *)packet->payload;
    const wifi_ieee80211_mac_hdr *header = &ieee8021Packet->hdr;

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

void SilentPromiscuousPacketHandler(const wifi_promiscuous_pkt_t *packet, wifi_promiscuous_pkt_type_t type)
{
}