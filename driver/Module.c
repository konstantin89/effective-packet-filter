#include "Module.h"

#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>

#include "utils/Logger.h"
#include "utils/Assert.h"
#include "hooks/HooksManager.h"
#include "DeviceClassManager.h"
#include "char_devices/CharDevicesManager.h"

MODULE_AUTHOR("konstantin89");
MODULE_DESCRIPTION("");
MODULE_LICENSE("GPL");

static int __init init(void)
{
    int exit_code = 0;

    LOG_DEBUG("Filter module is loading \n");

    if (STATUS_CODE_SUCCESS != DeviceClassManager_Init())
    {
        LOG_ERROR("DeviceClassManager_Init failed \n");
        return 1;
    }

    if (STATUS_CODE_SUCCESS != HooksManager_SetHooks())
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

    DeviceClassManager_Teardown();
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
    return 0;
}

ssize_t PacketCharDevWrite(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    return 0;
}

int PacketCharDevOpen(struct inode *inode, struct file *filp)
{
    return 0;
}

int PacketCharDevRelease(struct inode *inode, struct file *flip)
{
    return 0;
}

unsigned int PacketCharDevPoll(struct file *filp, poll_table *wait)
{
    return 0;
}