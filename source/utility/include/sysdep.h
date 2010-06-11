#include "libtype.h"

#ifndef UTILITY_SYSDEP_HEAD_FILE
#define UTILITY_SYSDEP_HEAD_FILE

#include <sys/types.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include "md5.h"

/* set the following to the number of 100ns ticks of the actual
   resolution of your system's clock */
#define UUIDS_PER_TICK 1024

/* Set this to what your compiler uses for 64-bit data type */

#define unsigned64_t unsigned long long
#define I64(C) C##LL

typedef unsigned64_t uuid_time_t;

typedef struct 
{
    char if_name[32];
    char nodeID[6];
} uuid_node_t;

INT32_T get_ieee_node_identifier(uuid_node_t *node);
void get_system_time(uuid_time_t *uuid_time);

#endif
