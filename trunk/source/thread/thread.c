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

#ifdef WINDOWS_OS_32
#include <Windows.h>
#endif

#ifdef VXWORKS_OS
#include <taskLib.h>
#endif

THREAD_HANDLE thread_handle_init(void)
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

    return (THREAD_HANDLE) thread_property_ptr;

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
                      THREAD_HANDLE thread_id, 
                      THREAD_PARAM_PTR thread_param_ptr
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
#ifdef WINDOWS_OS_32
    HANDLE WinThreadHandle;
#endif
    
    if (thread_id == AII_NULL || thread_param_ptr == AII_NULL)
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
    

    /* Create thread */
    retval = pthread_create((pthread_t*)&thread_property_ptr->thread_id, thread_property_ptr->thread_attr,
        thread_param_ptr->route_ptr, thread_param_ptr->args);
    if (retval != 0)
    {
        goto ErrExit;
    }
    
    thread_property_ptr->is_created = TRUE;

#endif

#ifdef VXWORKS_OS
    argi = (INT32_T) thread_param_ptr->args;
    task_id = taskSpawn(
        thread_param_ptr->thread_name,
        thread_param_ptr->priority,
        thread_param_ptr->vxworks_option_flag,
        thread_param_ptr->stack_size,
        thread_param_ptr->route_ptr,	
        argi,0,0,0,0,0,0,0,0,0
        );

    if (task_id == -1)
    {
        goto ErrExit;
    }
    
    thread_property_ptr->is_created = TRUE;
    thread_property_ptr->thread_id = task_id;
    
#endif

#ifdef WINDOWS_OS_32
    WinThreadHandle = CreateThread(
        AII_NULL,
        thread_param_ptr->stack_size,
        (LPTHREAD_START_ROUTINE)thread_param_ptr->route_ptr,
        (LPVOID)thread_param_ptr->args,
        thread_param_ptr->win_create_flag,
        AII_NULL
        );
    if (WinThreadHandle == AII_NULL)
    {
        goto ErrExit;
    }
    
    SetThreadPriority(WinThreadHandle, thread_param_ptr->priority);
    thread_property_ptr->thread_id = (UINT32_T) WinThreadHandle;
    
#endif
    
    memcpy(thread_property_ptr->thread_params, thread_param_ptr, sizeof(THREAD_PARAM));
    return AII_OK;

ErrExit:
#ifdef LINUX_OS
    pthread_attr_destroy(thread_property_ptr->thread_attr);
#endif
    
    return AII_ERROR;
}


INT32_T thread_suspend(THREAD_HANDLE thread_handle)
{
    THREAD_PROPERTY_PTR thread_property_ptr;

    if (thread_handle == AII_NULL)
    {
        return AII_ERROR;
    }
    
    thread_property_ptr = (THREAD_PROPERTY_PTR) thread_handle;
    if (thread_property_ptr->is_initialized == FALSE || 
        thread_property_ptr->is_created == FALSE)
    {
        return AII_ERROR;
    }

#ifdef LINUX_OS
    
#endif

#ifdef VXWORKS_OS
    if (taskSuspend(thread_property_ptr->thread_id) == ERROR)
    {
        return AII_ERROR;
    }
#endif
    
#ifdef WINDOWS_OS_32
    if (SuspendThread((HANDLE)thread_property_ptr->thread_id) == -1)
    {
        return AII_ERROR;
    }
#endif
    
    return AII_OK;
}

INT32_T thread_resume(THREAD_HANDLE thread_handle)
{
    THREAD_PROPERTY_PTR thread_property_ptr;

    if (thread_handle == AII_NULL)
    {
        return AII_ERROR;
    }

    thread_property_ptr = (THREAD_PROPERTY_PTR) thread_handle;
    if (thread_property_ptr->is_initialized == FALSE || 
        thread_property_ptr->is_created == FALSE)
    {
        return AII_ERROR;
    }

#ifdef LINUX_OS

#endif

#ifdef VXWORKS_OS
    if (taskResume(thread_property_ptr->thread_id) == ERROR)
    {
        return AII_ERROR;
    }
#endif

#ifdef WINDOWS_OS_32
    if (ResumeThread((HANDLE)thread_property_ptr->thread_id) == -1)
    {
        return AII_ERROR;
    }
#endif

    return AII_OK;
}

void thread_sleep(UINT32_T millisecond)
{
#ifdef LINUX_OS
    usleep(millisecond*1000);
#endif

#ifdef VXWORKS_OS
    INT32_T sleep_tick;
    
    sleep_tick = millisecond>>4;
    if (millisecond == 0)
    {
        sleep_tick = 0;
    }
    else if (sleep_tick == 0)
    {
        sleep_tick = 1;
    }

    taskDelay(sleep_tick);
#endif
#ifdef WINDOWS_OS_32
    sleep(millisecond);
#endif
}

INT32_T thread_priority_set(THREAD_HANDLE thread_handle, INT32_T priority)
{
    THREAD_PROPERTY_PTR thread_property_ptr;
#ifdef LINUX_OS
    struct sched_param	param;
#endif

    if (thread_handle == AII_NULL)
    {
        return AII_ERROR;
    }
    thread_property_ptr = (THREAD_PROPERTY_PTR)thread_handle;
    if (thread_property_ptr->is_initialized == FALSE || thread_property_ptr->is_created == FALSE)
    {
        return AII_ERROR;
    }

#ifdef LINUX_OS
    param.__sched_priority = priority;
    if (pthread_attr_setschedparam(thread_property_ptr->thread_attr, &param) != 0)
    {
        return AII_ERROR;
    }
#endif

#ifdef VXWORKS_OS

#endif
}

