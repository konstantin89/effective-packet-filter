#ifndef __PACKET_MESSAGE_H
#define __PACKET_MESSAGE_H

typedef struct PacketMessage
{
    unsigned char content[256];
} PacketMessage;

#endif // __PACKET_MESSAGE_H