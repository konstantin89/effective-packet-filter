#pragma once

namespace packet
{
class PacketInterface
{
public:
    enum class TransportType
    {
        Unclassified,
        Udp,
        Tcp
    };

    enum class PayloadType
    {
        Unclassified,
        DnsRequest,
        DnsResponse,
        Http
    };

    enum class PacketVerdict
    {
        Unclassified,
        Drop,
        Allow
    };

    virtual char *GetIpHeader() = 0;
    virtual char *GetTransportHeader() = 0;
    virtual char *GetPayload() = 0;

protected:
    TransportType mTransportType = TransportType::Unclassified;
    PayloadType mPayloadType = PayloadType::Unclassified;
    PacketVerdict mPacketVerdict = PacketVerdict::Unclassified;

};
}