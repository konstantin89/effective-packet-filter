#ifndef __NETWORK_UTILS_H
#define __NETWORK_UTILS_H

#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/udp.h>

#define IPV4_HEADER_LENGTH(ipv4Header)(ipv4Header->ihl * 4)
#define UDP_HEADER_LENGTH (sizeof(struct udphdr))

struct iphdr* NetworkUtils_GetIpv4Header(struct sk_buff *skb);
char* NetworkUtils_GetTransportHeader(struct sk_buff *skb);

#endif