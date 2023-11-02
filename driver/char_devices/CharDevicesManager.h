#ifndef __CHAR_DEVICES_MANAGER_H
#define __CHAR_DEVICES_MANAGER_H

#include "common/Defs.h"
#include "utils/Status.h"
#include "Module.h"

struct class;

extern struct CharDeviceOps g_packetCharDevOps;

#define CHAR_DEVS                                                                                                   \
    /*             enum                   file ops struct         ptr,     ,name                                */  \
    CHAR_DEV_ENTRY(PACKETS_CHAR_DEV,      &g_packetCharDevOps,    NULL,     PACKETS_CHAR_DEVICE_NAME              ) \
    CHAR_DEV_ENTRY(CHAR_DEV_NUM_OF,       NULL,                   NULL,     NULL                                  )  

#define CHAR_DEV_ENTRY(enum, file_ops, ptr, name) enum,
enum CharDevType
{
    CHAR_DEVS
};
#undef CHAR_DEV_ENTRY

STATUS_CODE CharDevicesManager_Init(struct class *deviceClass);

STATUS_CODE CharDevicesManager_OpenCharDevs(void);
STATUS_CODE CharDevicesManager_CloseCharDevs(void);
STATUS_CODE CharDevicesManager_Teardown(void);


#endif
