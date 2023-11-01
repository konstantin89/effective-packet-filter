#include "CharDevicesManager.h"
#include "CharDevice.h"

#include "utils/Logger.h"

#define CHAR_DEV_ENTRY(enum, file_ops, ptr, name) file_ops,
static struct CharDeviceOps* g_charDevOps[] = {CHAR_DEVS};
#undef CHAR_DEV_ENTRY

#define CHAR_DEV_ENTRY(enum, file_ops, ptr, name) ptr,
static struct CharDevice* g_charDevPtrs[] = {CHAR_DEVS};
#undef CHAR_DEV_ENTRY

#define CHAR_DEV_ENTRY(enum, file_ops, ptr, name) name,
static char* g_charDevNames[] = {CHAR_DEVS};
#undef CHAR_DEV_ENTRY

STATUS_CODE CharDevicesManager_Init(struct class *deviceClass)
{
    int i=0;
    for(i=0; i<CHAR_DEV_NUM_OF; i++)
    {
        // TODO - *g_charDevOps[i] bad API!

        g_charDevPtrs[i] = CharDevice_Create(*g_charDevOps[i], g_charDevNames[i], deviceClass);
        if (NULL == g_charDevPtrs[i])
        {
            CharDevicesManager_Teardown();
            return STATUS_CODE_MEMORY_ERROR;
        }
    }

    return STATUS_CODE_SUCCESS;
}

STATUS_CODE CharDevicesManager_Teardown(void)
{
    return STATUS_CODE_SUCCESS;
}

STATUS_CODE CharDevicesManager_OpenCharDevs(void)
{
    int i=0;
    STATUS_CODE status = STATUS_CODE_SUCCESS;

    for(i=0; i<CHAR_DEV_NUM_OF; i++)
    {
        if (NULL == g_charDevPtrs[i])
        {
            continue;
        }

        status = g_charDevPtrs[i]->Open(g_charDevPtrs[i]);
        if (status != STATUS_CODE_SUCCESS)
        {
            LOG_ERROR("Failed to open char dev %s \n", g_charDevNames[i]);
        }
    }

    return STATUS_CODE_SUCCESS;
}

STATUS_CODE CharDevicesManager_CloseCharDevs(void)
{
    int i=0;
    STATUS_CODE status = STATUS_CODE_SUCCESS;

    for(i=0; i<CHAR_DEV_NUM_OF; i++)
    {
        if (NULL == g_charDevPtrs[i])
        {
            continue;
        }
    
        status = g_charDevPtrs[i]->Close(g_charDevPtrs[i]);
        if (status != STATUS_CODE_SUCCESS)
        {
            LOG_ERROR("Failed to close char dev %s \n", g_charDevNames[i]);
        }
    }

    return STATUS_CODE_SUCCESS;}