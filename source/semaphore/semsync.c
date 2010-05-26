/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationSem.c 
 *  Description: this file contains private semaphore operations
 *  Author: chenwangxian
 *  Create:2010-03-15
 *  Modification history:
 *  2010-03-15, created the file,           chenwangxian
 *  2010-05-20, modification base on OS interfaces, Ben Shaw
 *
 *
 **************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "semsync.h"

#ifdef LINUX_OS
#include <semaphore.h>
#include <time.h>
#endif

#ifdef VXWORKS_OS
#include <semLib.h>
#endif

#ifdef WINDOWS_OS_32
#include <Windows.h>
#endif


SEM_HANDLE sem_create(INT32_T option, INT32_T init_count, INT32_T max_count)
{
    SEM_HANDLE sem_handle = AII_NULL;

#ifdef LINUX_OS
    sem_handle = (SEM_HANDLE)malloc(sizeof(sem_t));
    if (sem_handle == AII_NULL)
    {
        return AII_NULL;
    }
    
    if (sem_init((sem_t*)sem_handle, option, init_count) != 0)
    {
        free(sem_handle);
        return AII_NULL;
    }
    
#endif

#ifdef VXWORKS_OS
    sem_handle = (SEM_HANDLE) semCCreate(option, init_count);
    if (sem_handle == NULL)
    {
        return AII_NULL;
    }
#endif

#ifdef WINDOWS_OS_32
    sem_handle = (SEM_HANDLE) CreateSemaphore( 
        NULL,        /* default security attributes */
        init_count,  /* initial count */
        max_count,   /* maximum count */
        NULL);       /* unnamed semaphore */
    if (sem_handle == NULL)
    {
        return AII_NULL;
    }
#endif

    return sem_handle;
}


INT32_T sem_delete(SEM_HANDLE sem_handle)
{
    if (sem_handle == AII_NULL)
    {
        return AII_ERROR;
    }

#ifdef LINUX_OS
    if (sem_destroy((sem_t*)sem_handle) != 0)
    {
        /* Do not free semaphore handle memory block */
        return AII_ERROR;
    }
    free(sem_handle);
#endif

#ifdef VXWORKS_OS
    if (semDelete((SEM_ID)sem_handle) == ERROR)
    {
        return AII_ERROR;
    }
#endif

#ifdef WINDOWS_OS_32
    if (CloseHandle((HANDLE)sem_handle) == 0)
    {
        return AII_ERROR;
    }
#endif

    return AII_OK;
}


INT32_T sem_take(SEM_HANDLE sem_handle, INT32_T millisecond)
{
#ifdef LINUX_OS
    struct timespec wait_time;

    if (millisecond == WAIT_FOREVER)
    {
        if (sem_wait((sem_t*)sem_handle) != 0)
        {
            return AII_ERROR;
        }
    }
    else
    {
        wait_time.tv_sec = millisecond/1000;
        wait_time.tv_nsec = (millisecond - wait_time.tv_sec*1000)*1000000;
        if (sem_timedwait((sem_t*)sem_handle, &wait_time) != 0)
        {
            return AII_ERROR;
        }
    }

#endif

#ifdef VXWORKS_OS
    INT32_T tick;

    if (millisecond == WAIT_FOREVER)
    {
        semTake((SEM_ID)sem_handle, WAIT_FOREVER);
    }
    else
    {
        tick = millisecond>>4;
        if (millisecond == 0)
        {
            tick = 0;
        }
        else if (tick == 0)
        {
            tick = 1;
        }

        semTake((SEM_ID) sem_handle, tick);
    }
#endif

#ifdef WINDOWS_OS_32
    UINT32_T dwWaitResult;

    if (millisecond == WAIT_FOREVER)
    {
        millisecond = INFINITE;
    }

    dwWaitResult = WaitForSingleObject(sem_handle, millisecond);

    if (dwWaitResult == WAIT_ABANDONED)
    {
        return AII_ERROR;
    }

#endif

    return AII_OK;
}


INT32_T sem_give(SEM_HANDLE sem_handle)
{
#ifdef LINUX_OS
    if (sem_post((sem_t*)sem_handle) != 0)
    {
        return AII_ERROR;
    }
#endif

#ifdef VXWORKS_OS
    if (semGive((SEM_ID)sem_handle) == ERROR)
    {
        return AII_ERROR;
    }
#endif

#ifdef WINDOWS_OS_32
    if (ReleaseSemaphore((HANDLE)sem_handle,1,NULL) == 0)
    {
        return AII_ERROR;
    }
#endif

    return AII_OK;
}

