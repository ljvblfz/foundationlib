/*
 *  test_main.c
 *  foundationlib
 *
 *  Created by Ben Shaw on 6/10/10.
 *  Copyright 2010 AII, Inc. All rights reserved.
 *
 */

#include "uuid.h"

UUID_T NameSpace_DNS = { /* 6ba7b810-9dad-11d1-80b4-00c04fd430c8 */
    0x6ba7b810,
    0x9dad,
    0x11d1,
    0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
};

/* puid -- print a UUID */
void puid(UUID_T u)
{
    int i;
    
    printf("%8.8x-%4.4x-%4.4x-%2.2x%2.2x-", u.time_low, u.time_mid,
           u.time_hi_and_version, u.clock_seq_hi_and_reserved,
           u.clock_seq_low);
    for (i = 0; i < 6; i++)
        printf("%2.2x", u.node[i]);
    printf("\n");
}

/* Simple driver for UUID generator */
int main(int argc, char **argv)
{
    UUID_T u;
    int f;
    
    if (uuid_sys_init("eth0") == ERROR)
    {
        printf("UUID system initialization error!\n");
        return ERROR;
    }
    
    uuid_create(&u);
    puid(u);
    
    f = uuid_compare(&u, &u);
    printf("uuid_compare(u,u): %d\n", f);     /* should be 0 */
    f = uuid_compare(&u, &NameSpace_DNS);
    printf("uuid_compare(u, NameSpace_DNS): %d\n", f); /* s.b. 1 */
    f = uuid_compare(&NameSpace_DNS, &u);
    
    return OK;
}

