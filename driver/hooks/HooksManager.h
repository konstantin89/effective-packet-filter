#ifndef __HOOKS_MANAGER_H
#define __HOOKS_MANAGER_H

#include "utils/Status.h"

struct RingBuffer;

STATUS_CODE HooksManager_SetHooks(struct RingBuffer *packetsRingBuffer);

STATUS_CODE HooksManager_DropHooks(void);

#endif