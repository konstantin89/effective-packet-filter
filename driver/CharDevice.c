#include "CharDevice.h"
#include "Assert.h"
#include "Logger.h"

#include <linux/string.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

#define CHAR_DEVICE_REGION_NAME "filter_device_cdev"

static CharDevice* Allocate(void);
static void Free(CharDevice *instance);

static STATUS_CODE Open(CharDevice*instance);
static STATUS_CODE Close(CharDevice *instance);

struct CharDevicePrivateFields
{
    dev_t deviceNumber;
    struct cdev charDev;
    struct class *charDevClass;
    struct device *charDevDevice;
    struct file_operations fileOps;
    char deviceName[MAX_DEVICE_NAME_LEN];
    bool isOpen;
};

CharDevice *CharDevice_Create(CharDeviceOps ops, const char *name, struct class *deviceClass)
{
    CharDevice *instance = NULL;

    ASSERT(NULL != name);
    ASSERT(NULL != deviceClass);

    instance = Allocate();
    if (NULL == instance)
    {
        return NULL;
    }

    if (strlen(name) > (sizeof(instance->privateFields->deviceName)-1))
    {
        LOG_ERROR("Char device name is too long: [%s] \n", name);
        return NULL;
    }

    instance->privateFields->fileOps.open    = ops.open;
    instance->privateFields->fileOps.release = ops.release;
    instance->privateFields->fileOps.write   = ops.write;
    instance->privateFields->fileOps.read    = ops.read;
    instance->privateFields->fileOps.poll    = ops.poll;
    instance->privateFields->fileOps.owner   = THIS_MODULE;

    instance->Close = Close;
    instance->Open = Open;
    
    strncpy(instance->privateFields->deviceName, name, sizeof(instance->privateFields->deviceName));

    instance->privateFields->charDevClass = deviceClass;

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

    memset(instance->privateFields, 0, sizeof(struct CharDevicePrivateFields));

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

static STATUS_CODE Open(CharDevice*instance)
{
    struct CharDevicePrivateFields *privFields = instance->privateFields;

    int ret = 0;
    ret = alloc_chrdev_region(&privFields->deviceNumber, 0, 1, CHAR_DEVICE_REGION_NAME);
    if (ret < 0)
    {
        goto error_out;
    }

    cdev_init(&privFields->charDev, &privFields->fileOps);
    privFields->charDev.owner = THIS_MODULE;

    ret = cdev_add(&privFields->charDev, privFields->deviceNumber, 1);
    if(ret < 0)
    {
        goto unreg_chrdev;
    }

    privFields->charDevDevice = device_create(privFields->charDevClass, NULL, privFields->deviceNumber, NULL, privFields->deviceName);
    if(IS_ERR(privFields->charDevDevice))
    {
        goto cdev_del;
    } 

    privFields->isOpen = true;
    return STATUS_CODE_SUCCESS;

cdev_del:
    cdev_del(&privFields->charDev);    
unreg_chrdev:
    unregister_chrdev_region(privFields->deviceNumber, 1);
error_out:
    return STATUS_CODE_FAILED;
}

static STATUS_CODE Close(CharDevice *instance)
{
    struct CharDevicePrivateFields *privFields = NULL;

    ASSERT (NULL != instance);
    ASSERT (NULL != instance->privateFields);

    privFields = instance->privateFields;

    if (false == privFields->isOpen)
    {
        return STATUS_CODE_FAILED;
    }

    device_destroy(privFields->charDevClass, privFields->deviceNumber);
    cdev_del(&privFields->charDev);
    unregister_chrdev_region(privFields->deviceNumber, 1);

    return STATUS_CODE_SUCCESS;
}