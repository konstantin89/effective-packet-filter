#include "Module.h"

#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>

#include "utils/Logger.h"
#include "utils/Assert.h"
#include "utils/RingBuffer.h"
#include "hooks/HooksManager.h"
#include "DeviceClassManager.h"
#include "char_devices/CharDevicesManager.h"
#include "common/PacketMessage.h"

MODULE_AUTHOR("konstantin89");
MODULE_DESCRIPTION("");
MODULE_LICENSE("GPL");

static RingBuffer *g_packetsRingBuffer = NULL;

static int __init init(void)
{
    int exit_code = 0;
    struct class *deviceClass = NULL;

    LOG_DEBUG("Filter module is loading \n");

    if (STATUS_CODE_SUCCESS != DeviceClassManager_Init())
    {
        LOG_ERROR("DeviceClassManager_Init failed \n");
        return 1;
    }

    deviceClass = DeviceClassManager_GetClass();

    g_packetsRingBuffer = RingBuffer_Create(sizeof(PacketMessage), 20);
    if (NULL == g_packetsRingBuffer)
    {
        LOG_ERROR("RingBuffer_Create failed \n");
        return 1;
    }

    if (STATUS_CODE_SUCCESS != CharDevicesManager_OpenCharDevs())
    {
        LOG_ERROR("CharDevicesManager_OpenCharDevs failed \n");
        return 1;
    }
    if (STATUS_CODE_SUCCESS != CharDevicesManager_Init(deviceClass))
    {
        LOG_ERROR("CharDevicesManager_Init failed \n");
        return 1;
    }

    if (STATUS_CODE_SUCCESS != HooksManager_SetHooks(g_packetsRingBuffer))
    {
        LOG_ERROR("HooksManager_SetHooks failed \n");
        return 1;
    }

    return exit_code;
}

static void __exit cleanup(void)
{
    LOG_DEBUG("Filter module is unloading \n");

    HooksManager_DropHooks();

    CharDevicesManager_CloseCharDevs();

    CharDevicesManager_Teardown();

    DeviceClassManager_Teardown();

    RingBuffer_Destroy(g_packetsRingBuffer);
}

module_init(init);
module_exit(cleanup);

struct CharDeviceOps g_packetCharDevOps = 
{
    .read = PacketCharDevRead,
    .write = PacketCharDevWrite,
    .open = PacketCharDevOpen,
    .release = PacketCharDevRelease,
    .poll = PacketCharDevPoll
};

ssize_t PacketCharDevRead(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
    LOG_DEBUG("Packets char dev - read called \n");

    if (NULL == g_packetsRingBuffer)
    {
        LOG_ERROR("gTestRingBuffer is NULL");
    }

    ssize_t bytesRead = g_packetsRingBuffer->ReadByUser(g_packetsRingBuffer, buff, count);
    *f_pos += bytesRead;

    return bytesRead;
}

ssize_t PacketCharDevWrite(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    LOG_DEBUG("Packets char dev - write called \n");
    return 0;
}

int PacketCharDevOpen(struct inode *inode, struct file *filp)
{
    LOG_DEBUG("Packets char dev - open called \n");
    return 0;
}

int PacketCharDevRelease(struct inode *inode, struct file *flip)
{
    LOG_DEBUG("Packets char dev - release called \n");
    return 0;
}

unsigned int PacketCharDevPoll(struct file *filp, poll_table *wait)
{
    LOG_DEBUG("Packets char dev - poll called \n");
    return 0;
}