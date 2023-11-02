#include "HooksManager.h"

#include "utils/Logger.h"

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/kernel.h>


#include <linux/ip.h>
#include <linux/skbuff.h>


static struct nf_hook_ops *g_localOutHookIpv4 = NULL;

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
        // This is packet we want to upload!
    }

    return NF_ACCEPT; // Continue packet processing.
}

STATUS_CODE HooksManager_SetHooks(void)
{
    g_localOutHookIpv4 = (struct nf_hook_ops*)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);
    if (NULL == g_localOutHookIpv4)
    {
        LOG_ERROR("Failed to allocate memory for nf_hook_ops");
        return STATUS_CODE_MEMORY_ERROR;
    }

    g_localOutHookIpv4->hook     = (nf_hookfn*)LocalOutHookIpv4Callback;  /* hook function */
    g_localOutHookIpv4->hooknum     = NF_INET_LOCAL_OUT;       /* received packets */
    g_localOutHookIpv4->pf     = PF_INET;              /* ARP protocol */
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