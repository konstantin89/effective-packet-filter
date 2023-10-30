#include "DeviceClassManager.h"

#include "utils/Assert.h"
#include "utils/Logger.h"

#define DEVICE_CLASS_NAME "traffic_filter_class"

static struct class *g_deviceClass = NULL;

STATUS_CODE DeviceClassManager_Init()
{
    ASSERT(g_deviceClass == NULL);

    g_deviceClass = class_create(THIS_MODULE, DEVICE_CLASS_NAME);
    if (IS_ERR(g_deviceClass))
    {
        LOG_ERROR("class_create failed \n");
        return STATUS_CODE_FAILED;
    }

    return STATUS_CODE_SUCCESS;
}

struct class *DeviceClassManager_GetClass(void)
{
    return g_deviceClass;
}

void DeviceClassManager_Teardown()
{
    ASSERT(g_deviceClass != NULL);
    class_destroy(g_deviceClass);
}