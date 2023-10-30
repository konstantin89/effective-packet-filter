#include "Status.h"

#define INVALID_STATUS_CODE "INVALID_STATUS"

#define STATUS_ENTRY(status) [STATUS_CODE_ ## status] = #status,
static const char *gStatusCodeStrings[] = 
{
    STATUS_VALUES
};
#undef STATUS_ENTRY

const char *Status_GetString(STATUS_CODE status)
{
    if ((status < 0) || (status >= STATUS_CODE_NUM_OF))
    {
        return INVALID_STATUS_CODE;
    }

    return gStatusCodeStrings[status];
}