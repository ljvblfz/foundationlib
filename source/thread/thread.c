/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: thread.c
 *  Description: thread api from linux/vxworks/win32
 *  Author: Ben Shaw
 *  Create:2010-05-17
 *  Modification history:
 *  2010-05-17, created the file,         Ben Shaw
 *
 *
 **************************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>

#include "libtype.h"
#include "thread.h"

#ifdef LINUX_OS
#include <pthread.h>
#include <bits/local_lim.h>
#endif

thread_handle thread_handle_init(void)
{
    THREAD_PROPERTY_PTR thread_property_ptr;

    thread_property_ptr = AII_NULL;

    thread_property_ptr = malloc(sizeof(THREAD_PROPERTY));
    if (thread_property_ptr == AII_NULL)
    {
        return AII_NULL;
    }
    memset(thread_property_ptr, 0x00, sizeof(THREAD_PROPERTY));

    thread_property_ptr->thread_params = 
        (THREAD_PARAM_PTR) malloc(sizeof(THREAD_PARAM));
    if (thread_property_ptr->thread_params == AII_NULL)
    {
        goto ErrExit;
    }
    memset(thread_property_ptr->thread_params, 0x00, sizeof(THREAD_PARAM));

#ifdef LINUX_OS
    thread_property_ptr->thread_attr = (void*) malloc(sizeof(pthread_attr_t));
    if (thread_property_ptr->thread_attr == AII_NULL)
    {
        goto ErrExit;
    }
    memset(thread_property_ptr->thread_attr, 0x00, sizeof(pthread_attr_t));

#endif
    
    thread_property_ptr->is_initialized = TRUE;

    return (thread_handle) thread_property_ptr;

ErrExit:
    if (thread_property_ptr->thread_attr)
    {
        free(thread_property_ptr->thread_attr);
        thread_property_ptr->thread_attr = AII_NULL;
    }
    
    if (thread_property_ptr->thread_params)
    {
        free(thread_property_ptr->thread_params);
        thread_property_ptr->thread_params = AII_NULL;
    }

    if (thread_property_ptr)
    {
        free(thread_property_ptr);
        thread_property_ptr = AII_NULL;
    }
    
    return AII_NULL;
}

INT32_T thread_create(
                      thread_handle thread_id, 
                      THREAD_PARAM_PTR thread_param_ptr,
                      void* route_ptr,
                      void* args  
                      )
{
    INT32_T retval;
    INT32_T schedule_policy;
    INT32_T min_priority, max_priority;
    THREAD_PROPERTY_PTR thread_property_ptr;
#ifdef LINUX_OS
    struct sched_param	param;
#endif
#ifdef VXWORKS_OS
    INT32_T argi, task_id;
#endif
    
    if (thread_id == AII_NULL || thread_param_ptr == AII_NULL || route_ptr == AII_NULL)
    {
        return AII_ERROR;
    }

    thread_property_ptr = (THREAD_PROPERTY_PTR) thread_id;
    if (thread_property_ptr->is_initialized == FALSE)
    {
        return AII_ERROR;
    }

    if (thread_property_ptr->is_created == TRUE)
    {
        return AII_OK;
    }

#ifdef LINUX_OS
    /* Initialize pthread attribute data */
    retval = pthread_attr_init((pthread_attr_t*)thread_property_ptr->thread_attr);
    if (retval != 0)
    {
        return AII_ERROR;
    }

    /* Do not inherit the attribute from father pthread */
    retval = pthread_attr_setinheritsched(thread_property_ptr->thread_attr, PTHREAD_EXPLICIT_SCHED);
    if (retval != 0)
    {
        goto ErrExit;
    }
    
    /* set CPU scope to system which is only scope profile supported by Linux */
    retval =  pthread_attr_setscope(thread_property_ptr->thread_attr, PTHREAD_SCOPE_SYSTEM);
    if (retval != 0)
    {
        goto ErrExit;
    }

    /* Set thread schedule algorithm */
    switch (thread_param_ptr->schedule_policy)
    {
    case FCFS_SCHED:
        schedule_policy = SCHED_FIFO;
        break;
    case OTHER_SCHED:
        schedule_policy = SCHED_OTHER;
        break;
    case RR_SCHED:
        schedule_policy = SCHED_RR;
        break;
    default:
        break;
    }
    
    retval = pthread_attr_setschedpolicy(thread_property_ptr->thread_attr, schedule_policy);
    if (retval != 0)
    {
        goto ErrExit;
    }

    /* set detach state */
    retval = pthread_attr_setdetachstate(thread_property_ptr->thread_attr, PTHREAD_CREATE_DETACHED);
    if (retval != 0)
    {
        goto ErrExit;
    }

    /* Set thread schedule priority */
    /*
     *	Get priority range
     */
    min_priority = sched_get_priority_min(thread_property_ptr->thread_params->schedule_policy);
    if (min_priority == -1)
    {
        goto ErrExit;
    }
    
    max_priority = sched_get_priority_max(thread_property_ptr->thread_params->schedule_policy);
    if (max_priority == -1)
    {
        goto ErrExit;
    }

    thread_param_ptr->priority = 
        thread_param_ptr->priority < min_priority ? min_priority : thread_param_ptr->priority;
    thread_param_ptr->priority =
        thread_param_ptr->priority > max_priority ? max_priority : thread_param_ptr->priority;
    /*
     *	Set schedule priority
     */
    retval = pthread_attr_getschedparam(thread_property_ptr->thread_attr, &param);
    if (retval != 0)
    {
        goto ErrExit;
    }
    param.__sched_priority = thread_param_ptr->priority;
    retval = pthread_attr_setschedparam(thread_property_ptr->thread_attr, &param);
    if (retval != 0)
    {
        goto ErrExit;
    }
    
    /* Set thread stack size */
    thread_param_ptr->stack_size  = 
        thread_param_ptr->stack_size < PTHREAD_STACK_MIN ? PTHREAD_STACK_MIN : thread_param_ptr->stack_size;

    retval = pthread_attr_setstacksize(thread_property_ptr->thread_attr, thread_param_ptr->stack_size);
    if (retval != 0)
    {
        goto ErrExit;
    }
    
    memcpy(thread_property_ptr->thread_params, thread_param_ptr, sizeof(THREAD_PARAM));

    /* Create thread */
    retval = pthread_create((pthread_t*)&thread_property_ptr->thread_id, thread_property_ptr->thread_attr,
        route_ptr, args);
    if (retval != 0)
    {
        goto ErrExit;
    }
    
    /* Release pthread attribute data, but must not free the attribute structure */
    pthread_attr_destroy(thread_property_ptr->thread_attr);

    thread_property_ptr->is_created = TRUE;

    return AII_OK;
    
#endif

#ifdef VXWORKS_OS
    argi = (INT32_T) args;
    task_id = 
#endif

ErrExit:
#ifdef LINUX_OS
    pthread_attr_destroy(thread_property_ptr->thread_attr);
#endif
    
    return AII_ERROR;
}