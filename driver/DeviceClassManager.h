#include <linux/device.h>
#include "utils/Status.h"

/**
 * @brief: Device class manager handles device class register/derigester operations.
 *         All character devices created by this module will be under the class created.
 *
 *         For example:
 *         We register class with name `traffic_filter_class`.
 *         We careate character device 'character_device_name' under this class.
 * 
 *         The class will be created: /sys/devices/virtual/traffic_filter_class
 *         The char device will be created: /sys/devices/virtual/traffic_filter_class/character_device_name
 * 
 *         Link to the char device will be created under /sys/dev/char as well.
*/

STATUS_CODE DeviceClassManager_Init(void);

struct class *DeviceClassManager_GetClass(void);

void DeviceClassManager_Teardown(void);