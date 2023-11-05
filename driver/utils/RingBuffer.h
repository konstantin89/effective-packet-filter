#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include <linux/types.h>

struct RingBufferPrivateFields;

typedef struct RingBuffer
{
    struct RingBufferPrivateFields *privateFields;

    int (*Write)(struct RingBuffer* instance, const void* dataToWrite, int bufLen);
    int (*ReadByUser)(struct RingBuffer* instance, char __user *outBuffer, int bufLen);
    bool (*IsEmpty)(struct RingBuffer* instance);
    bool (*IsFull)(struct RingBuffer* instance);

} RingBuffer;

RingBuffer * RingBuffer_Create(unsigned int entrySize, unsigned int numOfEntries);
void RingBuffer_Destroy(struct RingBuffer* instance);

#endif