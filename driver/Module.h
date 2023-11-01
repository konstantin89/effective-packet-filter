#ifndef __MODULE_H
#define __MODULE_H

#include "char_devices/CharDeviceOps.h"

ssize_t PacketCharDevRead(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
ssize_t PacketCharDevWrite(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);
int PacketCharDevOpen(struct inode *inode, struct file *filp);
int PacketCharDevRelease(struct inode *inode, struct file *flip);
unsigned int PacketCharDevPoll(struct file *filp, poll_table *wait);



#endif