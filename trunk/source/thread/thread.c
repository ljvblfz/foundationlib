/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: thread.c
 *  Description: 
 *  
 *  thread api from linux/vxworks/win32
 *  AII thread library manages property information of threads created from
 *  any platform. 
 *      
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

/*
 *	Description:
 *	Initialize a thread handle, allocate THREAD_PROPERTY structure, allocate
 *  THREAD_PARAM structure, allocate thread_attr if the thread is Linux pthread.
 *  After all the structures are initialized, the is_initialized flag will be set
 *  to TRUE.
 *  
 *  thread_handle_init function only initialize all the data structures of thread handle,
 *  the thread has not created yet. The function thread_create should be called after
 *  thread_handle_init if user want to create the thread.
 *  
 *  Platform scope:
 *  Linux, no limit
 *  vxWorks, no limit
 *  win32, no limit
 *  
 *	Parameters:
 *	None.
 *  
 *  Return value:
 *  If the function succeeds, the return value is a handle to the thread.
 *  If the function fails, the return value is NULL.
 */
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

/*
 *	Description:
 *  Create thread under platform: Linux pthread, vxWorks task, WIN32 thread.
 *  The is_created flag will be set to TRUE after the thread is successfully created.
 *  
 *	Platform scope:
 *	Linux: create a pthread with attribute: no inherite attribute from father thread;
 *          system CPU scope; detached. 
 *  vxWorks: create a task with specific parameters.
 *  win32: create win32 thread. Note: win32 CreateThread function return a handle to represent
 *  a thread rather than a integer id. 
 *  
 *	Parameters:
 *	1. THREAD_HANDLE thread_handle [input] (valid void pointer), a thread_handle initialized by funtion thread_handle_init.
 *  2. THREAD_PARAM_PTR thread_param_ptr [input] (valid pointer), a pointer points to thread parameter data structure.
 *  
 *	Return value:
 *	If the function succeeds, it returns AII_OK.
 *  If the function fails, it returns AII_ERROR.
 *  
 */
INT32_T thread_create(
                      THREAD_HANDLE thread_handle, 
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
    
    if (thread_handle == AII_NULL || thread_param_ptr == AII_NULL)
    {
        return AII_ERROR;
    }

    thread_property_ptr = (THREAD_PROPERTY_PTR) thread_handle;
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

/*
 *	Description:
 *  This function suspend a specific thread. Note: this function cannot supported
 *  in Linux OS.
 *  
 *  This function is primary designed for debugger, should not use in formal
 *  program. 
 *
 *	Platform scope:
 *  Linux OS: unsupported.
 *  vxWorks: Refer to Tornado 5.5.1 document: Care should be taken with asynchronous
 *  use of this facility. The specified task is suspended regardless of its 
 *  current state. The task could, for instance, have mutual exclusion to 
 *  some system resource, such as the network * or system memory partition. 
 *  If suspended during such a time, the facilities engaged are unavailable, 
 *  and the situation often ends in deadlock. 
 *
 *  This routine is the basis of the debugging and exception handling packages. 
 *  However, as a synchronization mechanism, this facility should be rejected 
 *  in favor of the more general semaphore facility. 
 *
 *  win32:  Refer to Windows Visual Studio MSDN:
 *  This function is primarily designed for use by debuggers. It is not intended 
 *  to be used for thread synchronization. Calling SuspendThread on a thread 
 *  that owns a synchronization object, such as a mutex or critical section, 
 *  can lead to a deadlock if the calling thread tries to obtain a 
 *  synchronization object owned by a suspended thread. To avoid this situation, 
 *  a thread within an application that is not a debugger should signal the other 
 *  thread to suspend itself. The target thread must be designed to watch 
 *  for this signal and respond appropriately.
 *
 *	Parameters:
 *	THREAD_HANDLE thread_handle [input] (valid thread handle), thread handle.
 *
 *	Return value:
 *  If the function succeeds, it returns AII_OK.
 *  If the function fails, it returns AII_ERROR.
 *  
 */
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


/*
 *	Description:
 *  This function resume a specific thread which has been suspended. Note: this function cannot supported
 *  in Linux OS.
 *  
 *  This function is primary designed for debugger, should not use in formal
 *  program. 
 *  
 *	Platform scope:
 *	Linux: unsupported.
 *  vxWorks: supported.
 *  win32: supported.
 *  
 *	Parameters:
 *	THREAD_HANDLE thread_handle [input] (valid thread handle), thread handle.
 *  
 *	Return value:
 *  If the function succeeds, it returns AII_OK.
 *  If the function fails, it returns AII_ERROR.
 *
 */
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

/*
 *	Description:
 *  Sleep the thread for several milliseconds. 
 *  
 *	Platform scope:
 *	Linux: invoke usleep, adjust microsecond to millisecond.
 *  vxWorks: invoke taskDelay, adjust ticket to millisecond, maybe loss some precision.
 *          since the unite of taskDelay is ticket which equals 1/60 second, then 
 *          if millisecond == 0, ticket of taskDelay = 0, if millisecond > 0 but less
 *          than 1 ticket, ticket of taskDelay = 1. This facility guarantee at least
 *          1 ticket sleep for taskDelay.
 *  win32: invoke sleep under unite millisecond.
 *  
 *	Parameters:
 *	UINT32_T millisecond [input] (>0) sleep duration in millisecond unite.
 *  
 *	Return value:
 *  None.
 */
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

/*
 *	Description:
 *  This function changes a thread priority when a thread is running. 
 *  
 *	Platform scope:
 *	Linux: supported. Bigger priority number with higher priority schedule.
 *  vxWorks: supported. Smaller priority number with higher priority schedule.
 *  win32: supported. Bigger priority number with higher priority schedule. Please
 *  refer to Visual Studio MSDN document for more information.
 *  
 *	Parameters:
 *	THREAD_HANDLE thread_handle [input] (valid thread handle) thread handle.
 *  INT32_T priority [input] (valid priority value for specific platform) priority of thread.
 *  
 *	Return value:
 *	If the function succeeds, it returns AII_OK.
 *  if the function fails, it returns AII_ERROR.
 *  
 */
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
    if (taskPrioritySet(thread_property_ptr->thread_id, priority) == ERROR)
    {
        return AII_ERROR;
    }
#endif

#ifdef WINDOWS_OS_32
    /*
    	If the function succeeds, the return value is nonzero.
        If the function fails, the return value is zero. 
     */
    if (SetThreadPriority((HANDLE)thread_property_ptr->thread_id, priority) == 0)
    {
        return AII_ERROR;
    }
#endif
    
    thread_property_ptr->thread_params->priority = priority;
    return AII_OK;
}

/*
 *	Description:
 *  Get real time priority of a specific thread.
 *  
 *	Platform scope:
 *	Linux: supported.
 *  vxWorks: supported.
 *  win32: supported.
 *  
 *	Parameters:
 *	THREAD_HANDLE thread_handle [input] (valid thread handle) thread handle.
 *  INT32_T* priority_ptr [output] (valid initialized pointer) priority pointer for return.
 *  
 *	Return value:
 *	If the function succeeds, it returns AII_OK.
 *  if the function fails, it returns AII_ERROR.
 */
INT32_T thread_priority_get(THREAD_HANDLE thread_handle, INT32_T* priority_ptr)
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
    if (pthread_attr_getschedparam(thread_property_ptr->thread_attr, &param) != 0)
    {
        return AII_ERROR;
    }
    *priority_ptr = param.__sched_priority;
#endif

#ifdef VXWORKS_OS
    if (taskPriorityGet(thread_property_ptr->thread_id,priority_ptr) == ERROR)
    {
        return AII_ERROR;
    }
#endif

#ifdef WINDOWS_OS_32
    /* 
        If the function succeeds, the return value is the thread's priority level.
        If the function fails, the return value is THREAD_PRIORITY_ERROR_RETURN.
     */
    if ((*priority_ptr = GetThreadPriority((HANDLE)thread_property_ptr->thread_id)) == THREAD_PRIORITY_ERROR_RETURN)
    {
        return AII_ERROR;
    }

#endif

    if (*priority_ptr != thread_property_ptr->thread_params->priority)
    {
        thread_property_ptr->thread_params->priority = *priority_ptr;
    }

    return AII_OK;
}

/*
 *	Description:
 *  This function cancels/deletes/terminates a specific thread. Note: This function
 *      cannot guarantee identical behavior on all platforms. 
 *  
 *	Platform scope:
 *	Linux: supported. This function invokes pthread_cancel function in Linux OS 
 *          to cancel a thread. 
 *  vxWorks: supported. This function invokes taskDelete function in vxWorks OS
 *          to delete a task.
 *  win32: supported. This function invokes TerminateThread function in win32
 *          to terminate a win32 thread.
 *  
 *	Parameters:
 *	THREAD_HANDLE thread_handle [input] (valid thread handle) thread handle.
 *  
 *	Return value:
 *	If the function succeeds, it returns AII_OK.
 *  if the function fails, it returns AII_ERROR.
 *  
 */
INT32_T thread_cancel (THREAD_HANDLE thread_handle)
{
    THREAD_PROPERTY_PTR thread_property_ptr;
#ifdef WINDOWS_OS_32
    INT32_T ExitCode;
#endif

    if (thread_handle == AII_NULL)
    {
        return AII_ERROR;
    }

    thread_property_ptr = (THREAD_PROPERTY_PTR)thread_handle;
    if (thread_property_ptr->is_initialized == FALSE || 
        thread_property_ptr->is_created == FALSE)
    {
        return AII_ERROR;
    }

#ifdef LINUX_OS
    if (pthread_cancel((pthread_t)thread_property_ptr->thread_id) != 0)
    {
        return AII_ERROR;
    }
#endif

#ifdef VXWORKS_OS
    if (taskDelete(thread_property_ptr->thread_id) == ERROR)
    {
        return AII_ERROR;
    }
#endif

#ifdef WINDOWS_OS_32
    if (GetExitCodeThread((HANDLE)thread_property_ptr->thread_id, &ExitCode) == 0)
    {
        return AII_ERROR;
    }

    if (TerminateThread((HANDLE)thread_property_ptr->thread_id, ExitCode) == 0)
    {
        return AII_ERROR;
    }
#endif
    
    thread_property_ptr->is_created = FALSE;

    return AII_OK;
}

/*
 *	Description:
 *  This function destroys a specific thread. Free the handle data structure 
 *      along with all the attribute data structure and parameter data structure.
 *  
 *	Platform scope:
 *	    Linux: supported.
 *      vxWorks: supported.
 *      win32: supported.
 *	Parameters:
 *	THREAD_HANDLE thread_handle [input] (valid thread handle) thread handle.
 *  
 *	Return value:
 *	If the function succeeds, it returns AII_OK.
 *  if the function fails, it returns AII_ERROR.
 *	
 */
INT32_T thread_handle_destroy (THREAD_HANDLE thread_handle)
{
    THREAD_PROPERTY_PTR thread_property_ptr;

    if (thread_handle == AII_NULL)
    {
        return AII_ERROR;
    }
    
    thread_property_ptr = (THREAD_PROPERTY_PTR) thread_handle;

    if (thread_property_ptr->is_initialized == FALSE)
    {
        return AII_ERROR;
    }

    if (thread_property_ptr->is_created == TRUE)
    {
        if (thread_cancel(thread_handle) == AII_ERROR)
        {
            return AII_ERROR;
        }
    }

#ifdef LINUX_OS
    pthread_attr_destroy(thread_property_ptr->thread_attr);

    if (thread_property_ptr->thread_attr)
    {
        free(thread_property_ptr->thread_attr);
    }
    if (thread_property_ptr->thread_params)
    {
        free(thread_property_ptr->thread_params);
    }

#endif

#ifdef VXWORKS_OS
    if (thread_property_ptr->thread_params)
    {
        free(thread_property_ptr->thread_params);
    }
#endif

#ifdef WINDOWS_OS_32
    if (thread_property_ptr->thread_params)
    {
        free(thread_property_ptr->thread_params);
    }
#endif
    
    thread_property_ptr->is_initialized = FALSE;
    free(thread_property_ptr);

    return AII_OK;
}

