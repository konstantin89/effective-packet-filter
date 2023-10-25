#ifndef __STATUS_H
#define __STATUS_H

#define STATUS_VALUES           \
    STATUS_ENTRY(SUCCESS)       \
    STATUS_ENTRY(FAILED)        \
    STATUS_ENTRY(MEMORY_ERROR)  \
    STATUS_ENTRY(NUM_OF)

#define STATUS_ENTRY(status) STATUS_CODE_ ## status,
typedef enum STATUS_CODE
{
    STATUS_VALUES
} STATUS_CODE;
#undef STATUS_ENTRY

const char *Status_GetString(STATUS_CODE status);

#endif
