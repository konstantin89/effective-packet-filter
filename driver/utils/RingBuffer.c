#include "utils/RingBuffer.h"
#include "utils/Logger.h"
#include "utils/Status.h"
#include "utils/Assert.h"

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/slab.h>

static int Write(struct RingBuffer* instance, const void* dataToWrite, int bufLen);
static int Read(struct RingBuffer* instance, char __user *outBuffer, int bufLen);
static bool IsEmpty(struct RingBuffer* instance);
static bool IsFull(struct RingBuffer* instance);

static STATUS_CODE Constructor(RingBuffer *instance);
static RingBuffer *Alloc(void);
static void Free(RingBuffer *instance);

struct RingBufferPrivateFields
{
    spinlock_t lock;
    uint32_t writePos;
    uint32_t readPos;
    uint32_t size;
    uint32_t entrySize;
    uint8_t *buffer;
};

static STATUS_CODE Constructor(RingBuffer *instance)
{
    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields)

    memset(instance->privateFields, 0, sizeof(struct RingBufferPrivateFields));

    instance->Read = Read;
    instance->Write = Write;
    instance->IsEmpty = IsEmpty;
    instance->IsFull = IsFull;


    return STATUS_CODE_SUCCESS;
}

RingBuffer * RingBuffer_Create(uint32_t entrySize, uint32_t numOfEntries)
{
    RingBuffer *instance = Alloc();
    if (NULL == instance)
    {
        return NULL;
    }

    if (STATUS_CODE_SUCCESS != Constructor(instance))
    {
        Free(instance);
        return NULL;
    }

    return instance;
}

void RingBuffer_Destroy(struct RingBuffer* instance)
{

}

int Write(struct RingBuffer* instance, const void* dataToWrite, int bufLen)
{
    return 0;
}

int Read(struct RingBuffer* instance, char __user *outBuffer, int bufLen)
{
    return 0;
}

bool IsEmpty(struct RingBuffer* instance)
{
    return false;
}

bool IsFull(struct RingBuffer* instance)
{
    return false;
}

RingBuffer *Alloc(void)
{
    RingBuffer *instance = kmalloc(sizeof(RingBuffer), GFP_KERNEL);
    if (NULL == instance)
    {
        LOG_ERROR("Failed to allocate memory for RingBuffer");
        return NULL;
    }

    instance->privateFields = kmalloc(sizeof(struct RingBufferPrivateFields), GFP_KERNEL);
    if (NULL == instance->privateFields)
    {
        LOG_ERROR("Failed to allocate memory for RingBufferPrivateFields");
        kfree(instance);
        return NULL;
    }

    return instance;
}

static void Free(RingBuffer *instance)
{
    if ((NULL != instance) && (NULL != instance->privateFields))
    {
        kfree(instance->privateFields);
        instance->privateFields = NULL;
    }

    if (NULL != instance)
    {
        kfree(instance);
    }
}
