#include "HooksManager.h"

#include "utils/Logger.h"
#include "utils/RingBuffer.h"
#include "utils/Assert.h"

#include "network/NetworkUtils.h"

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



static unsigned int LocalOutHookIpv4Callback(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct udphdr *udpHeader = NULL;

    struct iphdr *ipv4Header = NetworkUtils_GetIpv4Header(skb);
    unsigned int payloadSize = 0;

    if (NULL == ipv4Header)
    {
        goto exit;
    }

    if (ipv4Header->protocol == IPPROTO_UDP)
    {
        LOG_DEBUG("Got UDP packet \n");


        udpHeader = (struct udphdr *)NetworkUtils_GetTransportHeader(skb);
        if (NULL == udpHeader)
        {
            goto exit;
        }

        if (ntohs(udpHeader->dest) == 53)
        {
            memset(&gPacket, 0, sizeof(gPacket));

            memcpy(&gPacket.ipHeader.ipv4Header, ipv4Header, IPV4_HEADER_LENGTH(ipv4Header));
            
            payloadSize = ntohs(ipv4Header->tot_len) - IPV4_HEADER_LENGTH(ipv4Header) - UDP_HEADER_LENGTH;

            memcpy(&gPacket.transportHeader.udpHeader, udpHeader, UDP_HEADER_LENGTH);
            memcpy(&gPacket.payload, udpHeader + UDP_HEADER_LENGTH, payloadSize);

            LOG_DEBUG("Got outgoing DNS packet, payload size is [%d] \n", payloadSize);

            g_packetsRingBuffer->Write(g_packetsRingBuffer, &gPacket, sizeof(gPacket));

            LOG_DEBUG("Outgoing DNS packet added to ring buffer\n");
        }
    }

exit:
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