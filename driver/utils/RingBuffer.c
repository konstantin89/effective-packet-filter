#include "utils/RingBuffer.h"
#include "utils/Logger.h"
#include "utils/Status.h"
#include "utils/Assert.h"

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/slab.h>

static int Write(struct RingBuffer* instance, const void* dataToWrite, int bufLen);
static int ReadByUser(struct RingBuffer* instance, char __user *outBuffer, int bufLen);
static bool IsEmpty(struct RingBuffer* instance);
static bool IsFull(struct RingBuffer* instance);

static STATUS_CODE Constructor(RingBuffer *instance, uint32_t entrySize, uint32_t numOfEntries);
static void Destructor(RingBuffer *instance);
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

RingBuffer * RingBuffer_Create(uint32_t entrySize, uint32_t numOfEntries)
{
    RingBuffer *instance = Alloc();
    if (NULL == instance)
    {
        return NULL;
    }

    if (STATUS_CODE_SUCCESS != Constructor(instance, entrySize, numOfEntries))
    {
        Free(instance);
        return NULL;
    }

    return instance;
}

void RingBuffer_Destroy(struct RingBuffer* instance)
{
    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields);

    Destructor(instance);
    Free(instance);
}

int Write(struct RingBuffer* instance, const void* dataToWrite, int bufLen)
{
    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields);

    // Temp variable to prevention of long dereference chains.
    struct RingBufferPrivateFields *privateFields = instance->privateFields;

    spin_lock(&privateFields->lock);

    if ()


    spin_unlock(&privateFields->lock);
    return 0;
}

int ReadByUser(struct RingBuffer* instance, char __user *outBuffer, int bufLen)
{
    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields);

    return 0;
}

bool IsEmpty(struct RingBuffer* instance)
{
    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields);

    return false;
}

bool IsFull(struct RingBuffer* instance)
{
    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields);

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

static STATUS_CODE Constructor(RingBuffer *instance, uint32_t entrySize, uint32_t numOfEntries)
{
    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields)

    memset(instance->privateFields, 0, sizeof(struct RingBufferPrivateFields));

    instance->ReadByUser = ReadByUser;
    instance->Write = Write;
    instance->IsEmpty = IsEmpty;
    instance->IsFull = IsFull;

    instance->privateFields->size = entrySize*numOfEntries;

    instance->privateFields->buffer = kmalloc(instance->privateFields->size, GFP_KERNEL);
    if(NULL == instance->privateFields->buffer)
    {
        LOG_ERROR("Failed to allocate[%d] bytes for ring buffer \n", instance->privateFields->size);
        return STATUS_CODE_MEMORY_ERROR;
    }

    instance->privateFields->entrySize = entrySize;

    spin_lock_init(&instance->privateFields->lock);

    return STATUS_CODE_SUCCESS;
}

static void Destructor(RingBuffer *instance)
{
    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields);

    if (NULL != instance->privateFields->buffer)
    {
        kfree(instance->privateFields->buffer);
        instance->privateFields->buffer = NULL;
    }
}