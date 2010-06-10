#include "libtype.h"

#ifndef UTILITY_SYSDEP_HEAD_FILE
#define UTILITY_SYSDEP_HEAD_FILE

#ifdef WINDOWS_OS_32
#include <windows.h>
#endif
#ifdef LINUX_OS
#include <sys/types.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#endif
#include "md5.h"

/* set the following to the number of 100ns ticks of the actual
   resolution of your system's clock */
#define UUIDS_PER_TICK 1024

/* Set the following to a calls to get and release a global lock */
#define LOCK
#define UNLOCK

/* Set this to what your compiler uses for 64-bit data type */
#ifdef WINDOWS_OS_32
#define unsigned64_t unsigned __int64
#define I64(C) C
#endif

#ifdef LINUX_OS
#define unsigned64_t unsigned long long
#define I64(C) C##LL
#endif

typedef unsigned64_t uuid_time_t;

typedef struct 
{
    char if_name[32];
    char nodeID[6];
} uuid_node_t;

INT32_T get_ieee_node_identifier(uuid_node_t *node);
void get_system_time(uuid_time_t *uuid_time);

#endif
