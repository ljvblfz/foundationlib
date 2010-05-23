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
#endif

#ifdef VXWORKS_OS
#include <semLib.h>
#endif

#ifdef WINDOWS_OS_32
#include <Windows.h>
#endif


SEM_ID sem_create(unsigned initValue)
{
#ifdef LINUX_OS
    sem_t* semId = AII_NULL;
    int rval;

    semId = (sem_t*)malloc(sizeof(sem_t));
    if (semId == AII_NULL)
    {
        return AII_NULL;
    }

    rval = Sem_init((sem_t*)semId, initValue);
    if (rval != 0)
    {
        free(semId);
        return AII_NULL;
    }
    return semId;

#elif VXWORKS_OS
    return semBCreate(SEM_Q_FIFO, initValue);
#endif
}

/*
 * =====================================================================
 * Function:SemDelete()
 * Description: delete a semaphore. 
 * Input:   semId -- sync semaphore id
 * Output:  N/A
 * Return:  On success zero; on failure -1.
 *======================================================================
 */
int SemDelete(SEM_ID semId)
{
#ifdef LINUX_OS
    int rval;

    if (semId == AII_NULL)
    {
        return (-1);
    }
    rval = Sem_destroy((sem_t*)semId);
    free(semId);
    semId = AII_NULL;
    return rval;

#elif VXWORKS_OS
    if (semId == AII_NULL)
    {
        return (-1);
    }
    return semDelete(semId);
#endif
}

/*
 * =====================================================================
 * Function:SemLock()
 * Description: lock a semaphore. 
 * Input:   semId -- sync semaphore id
 *          timeTick -- waiting time while blocking
 *              WAIT_FOREVER,   this routine block until semaphore available 
 *              NO_WAIT,        this routine return immediately with errorno
 *              wait time,      this routine block until semaphore available or time up
 * Output:  N/A
 * Return:  On success zero; on failure -1.
 *======================================================================
 */
int SemLock(SEM_ID semId, int timeTick)
{
#ifdef LINUX_OS
    if (semId == AII_NULL)
    {
        return (-1);
    }
    return Sem_wait((sem_t*)semId, timeTick);

#elif VXWORKS_OS
    if (semId == AII_NULL)
    {
        return (-1);
    }
    return semTake(semId, timeTick);
#endif
}

/*
 * =====================================================================
 * Function:SemUnLock()
 * Description: unlock a semaphore. 
 * Input:   semId -- sync semaphore id
 * Output:  N/A
 * Return:  On success zero; on failure -1.
 *======================================================================
 */
int SemUnlock(SEM_ID semId)
{
#ifdef LINUX_OS
    if (semId == AII_NULL)
    {
        return (-1);
    }
    return Sem_post((sem_t*)semId);

#elif VXWORKS_OS
    if (semId == AII_NULL)
    {
        return (-1);
    }
    return semGive(semId);
#endif
}

