#include "utils/RingBuffer.h"
#include "utils/Logger.h"
#include "utils/Status.h"
#include "utils/Assert.h"
#include "utils/MacroUtils.h"

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static int Write(struct RingBuffer* instance, const void* dataToWrite, int bufLen);
static int ReadByUser(struct RingBuffer* instance, char __user *outBuffer, int bufLen);
static bool IsEmpty(struct RingBuffer* instance);
static bool IsFull(struct RingBuffer* instance);

static STATUS_CODE Constructor(RingBuffer *instance, unsigned int entrySize, unsigned int numOfEntries);
static void Destructor(RingBuffer *instance);
static RingBuffer *Alloc(void);
static void Free(RingBuffer *instance);

struct RingBufferPrivateFields
{
    spinlock_t lock;
    unsigned int writePos;
    unsigned int readPos;
    unsigned char *buffer;
    unsigned int bufferSize;
    unsigned int entrySize;
};

static unsigned int FreeEntriesCount(struct RingBufferPrivateFields *privateFields)
{
    ASSERT(NULL != privateFields);

    if (privateFields->readPos == privateFields->writePos)
    {
        return (privateFields->bufferSize / privateFields->entrySize);
    }

    else if (privateFields->readPos < privateFields->writePos)
    {
        return (privateFields->writePos - privateFields->readPos) / privateFields->entrySize;
    }
    else
    {
        int freeBytes = (privateFields->bufferSize - privateFields->readPos) + privateFields->writePos;
        ASSERT(freeBytes >= 0);

        return freeBytes / privateFields->entrySize;
    }
} 

static unsigned int UsedEntriesCount(struct RingBufferPrivateFields *privateFields)
{
    return (privateFields->bufferSize / privateFields->entrySize) - FreeEntriesCount(privateFields);
} 

RingBuffer * RingBuffer_Create(unsigned int entrySize, unsigned int numOfEntries)
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
    unsigned int entriesToWrite = 0;
    unsigned int freeEntriesCount = 0;
    int i=0;

    struct RingBufferPrivateFields *privateFields = NULL;

    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields);

    // Temp variable to prevention of long dereference chains.
    privateFields = instance->privateFields;

    spin_lock(&privateFields->lock);

    entriesToWrite = bufLen / privateFields->entrySize;
    freeEntriesCount = FreeEntriesCount(privateFields);

    entriesToWrite = MIN(entriesToWrite, freeEntriesCount);

    for(i=0; i<entriesToWrite; i++)
    {
        memcpy(privateFields->buffer + privateFields->writePos, 
               dataToWrite + (i*privateFields->entrySize), 
               privateFields->entrySize);

        privateFields->writePos = (privateFields->writePos + (privateFields->entrySize)) % privateFields->bufferSize;
    }

    spin_unlock(&privateFields->lock);
    return (entriesToWrite * privateFields->entrySize);
}

int ReadByUser(struct RingBuffer* instance, char __user *outBuffer, int bufLen)
{
    int i=0;
    struct RingBufferPrivateFields *privateFields = NULL;
    ssize_t  copyToUserReturn = 0;

    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields);

    privateFields = instance->privateFields;

    unsigned int entriesAvailable = UsedEntriesCount(privateFields);
    unsigned int entriesToRead = bufLen / privateFields->entrySize;

    entriesToRead = MIN(entriesAvailable, entriesToRead);

    for(i=0; i<entriesToRead; i++)
    {
         copyToUserReturn = copy_to_user(outBuffer + (i*privateFields->entrySize),
                                         privateFields->readPos,
                                         privateFields->entrySize);

        if (copyToUserReturn != privateFields->entrySize)
        {
            // Handle this case?
            LOG_ERROR("copy_to_user written only %d bytes \n", copyToUserReturn);
        }

        privateFields->readPos =  (privateFields->readPos + privateFields->entrySize) % privateFields->bufferSize;
    }

    spin_unlock(&privateFields->lock);
    return (entriesToRead *  privateFields->entrySize);
}

bool IsEmpty(struct RingBuffer* instance)
{
    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields);

    return (instance->privateFields->readPos == instance->privateFields->writePos);
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

static STATUS_CODE Constructor(RingBuffer *instance, unsigned int entrySize, unsigned int numOfEntries)
{
    ASSERT(NULL != instance);
    ASSERT(NULL != instance->privateFields)

    memset(instance->privateFields, 0, sizeof(struct RingBufferPrivateFields));

    instance->ReadByUser = ReadByUser;
    instance->Write = Write;
    instance->IsEmpty = IsEmpty;
    instance->IsFull = IsFull;

    instance->privateFields->bufferSize = entrySize*numOfEntries;

    instance->privateFields->buffer = kmalloc(instance->privateFields->bufferSize, GFP_KERNEL);
    if(NULL == instance->privateFields->buffer)
    {
        LOG_ERROR("Failed to allocate[%d] bytes for ring buffer \n", instance->privateFields->bufferSize);
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