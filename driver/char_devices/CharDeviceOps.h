#ifndef __CHAR_DEVICE_OPS_H
#define __CHAR_DEVICE_OPS_H

#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/device.h>

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

#endif