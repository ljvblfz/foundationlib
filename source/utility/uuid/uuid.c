#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sysdep.h"
#include "uuid.h"

/* data type for UUID generator persistent state */
typedef struct 
{
    INT32_T         IsInitialized;
    uuid_time_t     ts;       /* saved timestamp */
    uuid_node_t     node;     /* saved node ID */
    UINT16_T        cs;       /* saved clock sequence */
} uuid_state;

/* various forward declarations */
static void format_uuid_v1( UUID_T* uuid, UINT16_T clock_seq, uuid_time_t timestamp, uuid_node_t uuid_node );
static void get_current_time(uuid_time_t *timestamp);
static INT32_T true_random(void);

static uuid_state* global_uuid_state_ptr = NULL;

INT32_T uuid_sys_init(STRING ifname)
{
    FILE* fp = NULL;
    INT32_T witems, ritems;
    uuid_node_t node;
    uuid_time_t current_time;

    global_uuid_state_ptr = malloc(sizeof(uuid_state));
    if (global_uuid_state_ptr == NULL)
    {
        return ERROR;
    }
    memset(global_uuid_state_ptr, 0x00, sizeof(uuid_state));
    memset(&node, 0x00, sizeof(node));
    memcpy(node.if_name, ifname, min(sizeof(node.if_name)-1,strlen(ifname)));
    get_ieee_node_identifier(&node);

    fp = fopen(UUID_STATE_FILE_PATH, "rb");
    if (fp == NULL)
    {
        global_uuid_state_ptr->node = node;
        global_uuid_state_ptr->cs = (UINT16_T) true_random();
        get_current_time(&global_uuid_state_ptr->ts);
        global_uuid_state_ptr->IsInitialized = 1;
        /* First access state file, create the initial file */
        if ((fp = fopen(UUID_STATE_FILE_PATH, "wb")) == NULL)
        {
            printf("Cannot create the uuid state file!\n");
            goto ErrExit;
        }
        witems = fwrite(global_uuid_state_ptr,sizeof(uuid_state), 1, fp);
        if (witems != 1)
        {
            goto ErrExit;
        }
        fclose(fp);
        fp = NULL;
        return OK;
    }
    
    
    
    
    ritems = fread(global_uuid_state_ptr, sizeof(uuid_state), 1, fp);
    if (ritems != 1)
    {
        global_uuid_state_ptr->node = node;
        global_uuid_state_ptr->cs = (UINT16_T) true_random();
        get_current_time(&global_uuid_state_ptr->ts);
        global_uuid_state_ptr->IsInitialized = 1;
        /* First access state file, create the initial file */
        if ((fp = fopen(UUID_STATE_FILE_PATH, "wb")) == NULL)
        {
            printf("Cannot create the uuid state file!\n");
            goto ErrExit;
        }
        witems = fwrite(global_uuid_state_ptr,sizeof(uuid_state), 1, fp);
        if (witems != 1)
        {
            goto ErrExit;
        }
        fclose(fp);
        fp = NULL;
        return OK;
    }

    get_current_time(&current_time);
    if (current_time < global_uuid_state_ptr->ts)
    {
        global_uuid_state_ptr->cs++;
    }
    global_uuid_state_ptr->ts = current_time;
    global_uuid_state_ptr->node = node;
    global_uuid_state_ptr->IsInitialized = 1;
    fclose(fp);
    fp = NULL;

    if ((fp = fopen(UUID_STATE_FILE_PATH, "wb")) == NULL)
    {
        printf("Cannot create the uuid state file!\n");
        goto ErrExit;
    }
    witems = fwrite(global_uuid_state_ptr,sizeof(uuid_state), 1, fp);
    if (witems != 1)
    {
        goto ErrExit;
    }
    fclose(fp);
    fp = NULL;

    return OK;

ErrExit:
    if (global_uuid_state_ptr)
    {
        free(global_uuid_state_ptr);
        global_uuid_state_ptr = NULL;
    }

    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

    return ERROR;
}


/* uuid_create -- generator a UUID */
INT32_T uuid_create(UUID_T *uuid)
{
    uuid_time_t timestamp, last_time;
    UINT16_T clockseq;
    uuid_node_t last_node;
    INT32_T f;

    /* get time, node ID, saved state from non-volatile storage */
    get_current_time(&timestamp);
    clockseq = global_uuid_state_ptr->cs;
    
    /* stuff fields into the UUID */
    format_uuid_v1(uuid, clockseq, timestamp, global_uuid_state_ptr->node);
    return 1;
}

/* format_uuid_v1 -- make a UUID from the timestamp, clockseq, and node ID */
static void format_uuid_v1(
                    UUID_T* uuid, 
                    UINT16_T clock_seq, 
                    uuid_time_t timestamp, 
                    uuid_node_t uuid_node
                    )
{
    /* Construct a version 1 uuid with the information we've gathered
       plus a few constants. */
    uuid->time_low = (unsigned long)(timestamp & 0xFFFFFFFF);
    uuid->time_mid = (unsigned short)((timestamp >> 32) & 0xFFFF);
    uuid->time_hi_and_version = (unsigned short)((timestamp >> 48) & 0x0FFF);
    uuid->time_hi_and_version |= (1 << 12);
    uuid->clock_seq_low = clock_seq & 0xFF;
    uuid->clock_seq_hi_and_reserved = (clock_seq & 0x3F00) >> 8;
    uuid->clock_seq_hi_and_reserved |= 0x80;
    memcpy(uuid->node, uuid_node.nodeID, sizeof(uuid_node.nodeID));
}


/* get-current_time -- get time as 60-bit 100ns ticks since UUID epoch.
   Compensate for the fact that real clock resolution is
   less than 100ns. */
static void get_current_time(uuid_time_t *timestamp)
{
    static INT32_T IsFirstTime = 1;
    static uuid_time_t time_last;
    static UINT16_T uuids_this_tick = 0;
    uuid_time_t time_now;

    if (IsFirstTime) 
    {
        get_system_time(&time_now);
        time_last = time_now;
        *timestamp = time_now;
        IsFirstTime = 0;
        return;
    }
    
    get_system_time(&time_now);
    if (time_now == time_last)
    {
        uuids_this_tick++;
        if (uuids_this_tick >= UUIDS_PER_TICK)
        {
            uuids_this_tick = 0;
        }
    }
    else
    {
        uuids_this_tick = 0;
    }

    *timestamp = time_now + uuids_this_tick;
    
    return;
}

/* true_random -- generate a crypto-quality random number.
   **This sample doesn't do that.** */
static INT32_T true_random(void)
{
    uuid_time_t time_now;

    get_system_time(&time_now);
    time_now = time_now / UUIDS_PER_TICK;
    srand((UINT32_T)(((time_now >> 32) ^ time_now) & 0xffffffff));

    return rand();
}

/* uuid_compare --  Compare two UUID's "lexically" and return */
#define CHECK(f1, f2) if ((f1) != (f2)) return (f1) < (f2) ? -1 : 1
int uuid_compare(UUID_T *u1, UUID_T *u2)
{
    INT32_T i;

    CHECK(u1->time_low, u2->time_low);
    CHECK(u1->time_mid, u2->time_mid);

    CHECK(u1->time_hi_and_version, u2->time_hi_and_version);
    CHECK(u1->clock_seq_hi_and_reserved, u2->clock_seq_hi_and_reserved);
    CHECK(u1->clock_seq_low, u2->clock_seq_low);

    for (i = 0; i < 6; i++) 
    {
        if (u1->node[i] < u2->node[i])
        {
            return -1;
        }

        if (u1->node[i] > u2->node[i])
        {
            return 1;
        }
    }
    return 0;
}

#undef CHECK
