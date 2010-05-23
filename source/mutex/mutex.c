/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: mutex.c 
 *  Description:
 *  This library contains mutex API cross platform: linux, vxWorks, Windows.
 *  
 *  Author: Ben Shaw
 *  Create:2010-05-21
 *  
 *  Modification history:
 *  2010-05-21, created the file,           Ben Shaw
 *
 *  
 **************************************************************************************/


#include <stdio.h>
#include <stdlib.h>

#include "mutex.h"

#ifdef LINUX_OS
#include <pthread.h>
#include <time.h>
#endif

#ifdef VXWORKS_OS
#include <taskLib.h>
#include <semLib.h>
#endif

#ifdef WINDOWS_OS_32
#include <Windows.h>
#endif

/*
 *	Description:
 *  Create a mutex handle.
 *  
 *	Platform scope:
 *	Linux: supported.
 *  vxWorks: supported. This function creates a mutex semaphore. The mutual-exclusion semaphore 
 *  is a specialized version of the binary semaphore, designed to address issues 
 *  inherent in mutual exclusion, such as recursive access to resources, priority 
 *  inversion, and deletion safety. The fundamental behavior of the mutual-exclusion 
 *  semaphore is identical to the binary semaphore except for the following restrictions:
 *  - It can only be used for mutual exclusion.
 *  - It can only be given by the task that took it.
 *  - It may not be taken or given from interrupt level.
 *  win32: supported. This function creates a mutex handle in win32 platform.
 *  
 *	Parameters:
 *	none.
 *  
 *	Return value:
 *	If the function succeeds, the return value is a handle of mutex.
 *  Otherwise, it return AII_NULL.
 */
MUTEX_HANDLE mutex_create(void)
{

#ifdef LINUX_OS
    pthread_mutex_t* mutex_handle = AII_NULL;
    INT32_T rval;

    /* allocate memory in mutex_create,
     * free memory later in MutexDelete */
    mutex_handle = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (mutex_handle == AII_NULL)
    {
        return AII_NULL;
    }
     
    rval = pthread_mutex_init(mutex_handle, AII_NULL);
    if (rval != 0)
    {
        free(mutex_handle);
        return AII_NULL;
    }

    return (MUTEX_HANDLE) mutex_handle;
#endif

#ifdef VXWORKS_OS
    SEM_ID sem_id;
    
    sem_id = semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);

    if (sem_id == NULL)
    {
        return AII_NULL;
    }

    return (MUTEX_HANDLE) sem_id;

#endif

#ifdef WINDOWS_OS_32
    HANDLE ghMutex;

    ghMutex = CreateMutex(NULL, FALSE, NULL);
    if (ghMutex == NULL)
    {
        return AII_NULL;
    }

    return (MUTEX_HANDLE) ghMutex;

#endif

}

/*
 *	Description:
 *  This function delete a mutex handle.
 *  
 *	Platform scope:
 *	Linux: Support.
 *  vxWorks: Support.
 *  win32: Support.
 *  
 *	Parameters:
 *	MUTEX_HANDLE mutex_handle [input] (valid mutex handle) mutex hande.
 *  
 *	Return value:
 *	If the function succeeds, it return AII_OK.
 *  Otherwise, it return AII_ERROR.
 *  
 */
INT32_T mutex_delete(MUTEX_HANDLE mutex_handle)
{
    if (mutex_handle == AII_NULL)
    {
        return AII_ERROR;
    }

#ifdef LINUX_OS
    if (pthread_mutex_destroy((pthread_mutex_t*)mutex_handle) != 0)
    {
        return AII_ERROR;
    }

    free(mutex_handle);

#endif

#ifdef VXWORKS_OS
    if (semDelete((SEM_ID)mutex_handle) == ERROR)
    {
        return AII_ERROR;
    }

#endif

#ifdef WINDOWS_OS_32
    CloseHandle((HANDLE) mutex_handle);
#endif

    return AII_OK;
}

/*
 *	Description:
 *  This function locks the mutex semaphore for mutual exclusion access to 
 *  critical section. It can lock the mutex semaphore for infinite long time
 *  if the millisec = WAITFOREVER or specific finite long time in millisecond unit.
 *  
 *	Platform scope:
 *	Linux: support.
 *  vxWorks: support.
 *  win32: support.
 *  
 *	Parameters:
 *	MUTEX_HANDLE mutex_handle [input] (valid mutex handle) mutex handle.
 *  INT32_T millisec [input] (WAITFOREVER, >=0) lock duration.
 *  
 *	Return value:
 *	If the function succeeds, it returns AII_OK.
 *  Otherwise, it returns AII_ERROR.
 *  
 */
INT32_T mutex_lock(MUTEX_HANDLE mutex_handle, INT32_T millisec)
{
    if (mutex_handle == AII_NULL)
    {
        return AII_ERROR;
    }

#ifdef LINUX_OS
    struct timespec timeout;

    if (millisec == WAIT_FOREVER)
    {
        if (pthread_mutex_lock((pthread_mutex_t*) mutex_handle) != 0)
        {
            return AII_ERROR;
        }
    }
    else
    {
        timeout.tv_sec = millisec / 1000;
        timeout.tv_nsec = (millisec - timeout.tv_sec*1000) * 1000000;

        if (pthread_mutex_timedlock((pthread_mutex_t*)mutex_handle, &timeout) != 0)
        {
            return AII_ERROR;
        }
    }
    
#endif

#ifdef VXWORKS_OS
    INT32_T tick;

    if (millisec == WAIT_FOREVER)
    {
        semTake((SEM_ID)mutex_handle, WAIT_FOREVER);
    }
    else
    {
        tick = millisec>>4;
        if (millisec == 0)
        {
            tick = 0;
        }
        else if (tick == 0)
        {
            tick = 1;
        }

        semTake((SEM_ID) mutex_handle, tick);
    }

#endif

#ifdef WINDOWS_OS_32
    UINT32_T dwWaitResult;

    if (millisec == WAIT_FOREVER)
    {
        millisec = INFINITE;
    }

    dwWaitResult = WaitForSingleObject(mutex_handle, millisec);
    
    if (dwWaitResult == WAIT_ABANDONED)
    {
        return AII_ERROR;
    }

#endif
    
    return AII_OK;
}

/*
 *	Description:
 *  This function unlock a mutex semaphore.
 *  
 *	Platform scope:
 *	Linux: support.
 *  vxWorks: support. Note: It can only be unlocked by the thread that took it.
 *  win32: support.
 *  
 *	Parameters:
 *	MUTEX_HANDLE mutex_handle [input] (valid mutex handle) mutex handle.
 *  
 *	Return value:
 *	If the function succeeds, it returns AII_OK.
 *  Otherwise it returns AII_ERROR.
 *  
 */
INT32_T mutex_unlock(MUTEX_HANDLE mutex_handle)
{
    if (mutex_handle == AII_NULL)
    {
        return AII_ERROR;
    }

#ifdef LINUX_OS
    if (pthread_mutex_unlock((pthread_mutex_t*)mutex_handle) != 0)
    {
        return AII_ERROR;
    }
    
#endif

#ifdef VXWORKS_OS
    if (semGive((SEM_ID)mutex_handle) == ERROR)
    {
        return AII_ERROR;
    }
#endif

#ifdef WINDOWS_OS_32
    if (ReleaseMutex((HANDLE)mutex_handle) == 0)
    {
        return AII_ERROR;
    }

#endif

    return AII_OK;
}


