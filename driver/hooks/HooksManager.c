#include "HooksManager.h"

#include "utils/Logger.h"
#include "utils/RingBuffer.h"
#include "utils/Assert.h"

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/kernel.h>

#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/udp.h>

#include "common/PacketMessage.h"

static struct RingBuffer *g_packetsRingBuffer = NULL;
static struct nf_hook_ops *g_localOutHookIpv4 = NULL;

static PacketMessage gPacket;

struct iphdr *extract_ipv4_header(struct sk_buff *skb) 
{
    struct iphdr *ip_header;

    // Check if skb contains an IPv4 header
    if (skb->protocol == htons(ETH_P_IP)) 
    {
        // Extract IPv4 header
        ip_header = ip_hdr(skb);

        return ip_header;
    }

    return NULL;  // Not an IPv4 packet
}

static unsigned int LocalOutHookIpv4Callback(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct iphdr *ipv4Header = extract_ipv4_header(skb);
    if (NULL == ipv4Header)
    {
        LOG_ERROR("Failed to extract IPv4 header from sk_buff \n");
        return NF_ACCEPT;
    }

    if (ipv4Header->protocol == IPPROTO_UDP)
    {
        LOG_DEBUG("Got UDP packet \n");

        int ip_header_length = ipv4Header->ihl * 4;
        int udp_header_length = sizeof(struct udphdr);
        struct udphdr *udp_header = (struct udphdr *)((char *)ipv4Header + ip_header_length);

        if (ntohs(udp_header->dest) == 53)
        {
            LOG_DEBUG("Got outgoing DNS packet \n");

            memset(&gPacket, 0, sizeof(gPacket));
            memcpy(&gPacket.content, ipv4Header, ipv4Header->tot_len);
            g_packetsRingBuffer->Write(g_packetsRingBuffer, &gPacket, sizeof(gPacket));

            LOG_DEBUG("Outgoing DNS packet added to ring buffer\n");
        }
    }

    return NF_ACCEPT; // Continue packet processing.
}

STATUS_CODE HooksManager_SetHooks(struct RingBuffer *packetsRingBuffer)
{
    ASSERT(NULL != packetsRingBuffer);

    g_packetsRingBuffer = packetsRingBuffer;

    g_localOutHookIpv4 = (struct nf_hook_ops*)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);
    if (NULL == g_localOutHookIpv4)
    {
        LOG_ERROR("Failed to allocate memory for nf_hook_ops");
        return STATUS_CODE_MEMORY_ERROR;
    }

    g_localOutHookIpv4->hook         = (nf_hookfn*)LocalOutHookIpv4Callback;  /* hook function */
    g_localOutHookIpv4->hooknum      = NF_INET_LOCAL_OUT;       /* received packets */
    g_localOutHookIpv4->pf           = PF_INET;              /* ARP protocol */
    g_localOutHookIpv4->priority     = NF_IP_PRI_FIRST;              /* priority */
    //g_localOutHookIpv4->dev = dev;

    nf_register_net_hook(&init_net, g_localOutHookIpv4);

    return STATUS_CODE_SUCCESS;
}

STATUS_CODE HooksManager_DropHooks(void)
{
    if (NULL != g_localOutHookIpv4)
    {
        nf_unregister_net_hook(&init_net, g_localOutHookIpv4);
    }
    
    return STATUS_CODE_SUCCESS;
}