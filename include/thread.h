/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: thread.h
 *  Description: thread api from linux/vxworks/win32
 *  Author: Ben Shaw
 *  Create:2010-05-17
 *  Modification history:
 *  2010-05-17, created the file,         Ben Shaw
 *
 *
 **************************************************************************************/ 
#include "libtype.h"

typedef void* thread_handle;

typedef enum 
{
    FCFS        = 0,
    PRIORITY    = 1,
    RR          = 2,
}SCHEDULE_POLICY_T;

typedef struct
{
    INT32_T             stack_size;
    INT32_T             priority;
    SCHEDULE_POLICY_T   schedule_policy;
    /*
     *	Reserve for vxWorks option flag.
     */
    INT32_T             other_options;
}THREAD_PARAM,*THREAD_PARAM_PTR;

typedef struct
{
    THREAD_PARAM_PTR    thread_params;
    UINT32_T            thread_id;
    void*               thread_attr;
}THREAD_PROPERTY,*THREAD_PROPERTY_PTR;

