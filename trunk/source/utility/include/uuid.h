#include "sysdep.h"

#ifndef UUID_HEAD_FILE
#define UUID_HEAD_FILE

typedef struct 
{
    UINT32_T  time_low;
    UINT16_T  time_mid;
    UINT16_T  time_hi_and_version;
    UINT8_T   clock_seq_hi_and_reserved;
    UINT8_T   clock_seq_low;
    UINT8_T   node[6];
}UUID_T;

#define UUID_STATE_FILE_PATH "uuid.bin"

INT32_T uuid_sys_init(STRING ifname);
INT32_T uuid_create(UUID_T *uuid);
/* uuid_compare --  Compare two UUID's "lexically" and return
        -1   u1 is lexically before u2
         0   u1 is equal to u2
         1   u1 is lexically after u2
   Note that lexical ordering is not temporal ordering!
*/
int uuid_compare(UUID_T *u1, UUID_T *u2);

#endif
