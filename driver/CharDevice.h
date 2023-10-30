#ifndef __CHAR_DEVICE_H
#define __CHAR_DEVICE_H

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/device.h>

#include "utils/Status.h"

/** 
 * @brief: Create character device under give device class with given character device name.
 *         The char device with given name will be created under /dev.
*/

#define MAX_DEVICE_NAME_LEN 64

typedef ssize_t (*ReadCdevCallbackType)(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
typedef ssize_t (*WriteCdevCallbackType)(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);
typedef int (*OpenCdevCallbackType)(struct inode *inode, struct file *filp);
typedef int (*ReleaseCdevCallbackType)(struct inode *inode, struct file *flip);
typedef unsigned int (*PollCdevCallbackType)(struct file *filp, poll_table *wait);

typedef struct CharDeviceOps
{
    ReadCdevCallbackType read;
    WriteCdevCallbackType write;
    OpenCdevCallbackType open;
    ReleaseCdevCallbackType release;
    PollCdevCallbackType poll;
} CharDeviceOps;

struct CharDevicePrivateFields;
typedef struct CharDevice CharDevice;

struct CharDevice
{
    struct CharDevicePrivateFields *privateFields;

    STATUS_CODE (*Open)(CharDevice *instance);
    STATUS_CODE (*Close)(CharDevice *instance);

};

/**
 * @brief: Create character device
 * @param: ops - file operations for the character device
 * @param: name - character device name. Max size of the is MAX_DEVICE_NAME_LEN.
 * @param: deviceClass - Device class that the character device will be under.
 * @returns: New character device instance
*/
CharDevice *CharDevice_Create(CharDeviceOps ops, const char *name, struct class *deviceClass);

/**
 * @brief: Destroy character device instance.
 * @param:  instance
*/
void CharDevice_Destroy(CharDevice *instance);


#endif