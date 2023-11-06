#include "network/NetworkUtils.h"

#include "utils/Logger.h"
#include "utils/Assert.h"

struct iphdr* NetworkUtils_GetIpv4Header(struct sk_buff *skb)
{
    ASSERT(NULL != skb);

    struct iphdr *ip_header;

    if (skb->protocol == htons(ETH_P_IP)) 
    {
        ip_header = ip_hdr(skb);
        return ip_header;
    }

    return NULL;
}

char* NetworkUtils_GetTransportHeader(struct sk_buff *skb)
{
    char* transportHeader = NULL;
    ASSERT(NULL != skb);

    transportHeader = skb_transport_header(skb);
    if (NULL == transportHeader)
    {
        LOG_ERROR("skb_transport_header returned NULL \n");
        return NULL;
    }

    return transportHeader;
}
