#ifndef __PACKET_MESSAGE_H
#define __PACKET_MESSAGE_H

#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/udp.h>
#include <linux/tcp.h>

#define MAX_PACKET_PAYLOAD_SIZE 256

union IpHeader
{
    struct iphdr ipv4Header;
    struct ipv6hdr ipv6Header;
};

union TransportHeader
{
    struct udphdr udpHeader;
    struct tcphdr tcpHeader;
};

typedef struct PacketMessage
{
    union IpHeader ipHeader;
    union TransportHeader transportHeader;

    unsigned char payload[MAX_PACKET_PAYLOAD_SIZE];

} PacketMessage;

#endif // __PACKET_MESSAGE_H