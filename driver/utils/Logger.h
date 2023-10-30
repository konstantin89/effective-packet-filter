#ifndef __LOGGER_H
#define __LOGGER_H

#include <linux/kernel.h>

#define LOG_DEBUG(fmt, ...) printk(KERN_DEBUG fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  printk(KERN_INFO fmt,  ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printk(KERN_ERR fmt,   ##__VA_ARGS__)

#endif