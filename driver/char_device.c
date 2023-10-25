#include "char_device.h"

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

static CharDevice* Allocate(void);
static void Free(CharDevice*instance);

struct CharDevicePrivateFields
{
    dev_t deviceNumber;
    struct cdev charDev;
    struct class *charDevClass;
    struct device *charDevDevice;
    struct file_operations fileOps;
};

CharDevice *CharDevice_Create(CharDeviceOps ops, const char *name)
{
    CharDevice *instance = Allocate();
    if (NULL == instance)
    {
        return NULL;
    }

    instance->privateFields->fileOps.open    = ops.open;
    instance->privateFields->fileOps.release = ops.release;
    instance->privateFields->fileOps.write   = ops.write;
    instance->privateFields->fileOps.read    = ops.read;
    instance->privateFields->fileOps.poll    = ops.poll;
    instance->privateFields->fileOps.owner   = THIS_MODULE;

    return instance;
}

void CharDevice_Destroy(CharDevice *instance)
{
    Free(instance);
}

static CharDevice* Allocate()
{
    CharDevice *instance = kmalloc(sizeof(CharDevice), GFP_KERNEL);
    if (NULL == instance)
    {
        return NULL;
    }

    instance->privateFields = kmalloc(sizeof(struct CharDevicePrivateFields), GFP_KERNEL);
    if (NULL == instance->privateFields)
    {
        kfree(instance);
        return NULL;
    }

    return instance;
}

static void Free(CharDevice*instance)
{
    if ((instance != NULL) && (instance->privateFields != NULL))
    {
        kfree(instance->privateFields);
    }
    if (instance != NULL)
    {
        kfree(instance);
    }
}
