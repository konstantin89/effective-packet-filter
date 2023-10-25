#ifndef __CHAR_DEVICE_H
#define __CHAR_DEVICE_H

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/poll.h>

#include "status.h"

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

    STATUS_CODE (*Init)(CharDevice *instance);
    STATUS_CODE (*Purge)(CharDevice *instance);

};

CharDevice *CharDevice_Create(CharDeviceOps ops, const char *name);

void CharDevice_Destroy(CharDevice *instance);


#endif