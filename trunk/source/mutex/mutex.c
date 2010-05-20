/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationMutex.c 
 *  Description: this file contains private mutex operations
 *  Author: chenwangxian
 *  Create:2010-03-15
 *  Modification history:
 *  2010-03-15, created the file,           chenwangxian
 *
 *
 **************************************************************************************/

#include "foundationInclude.h"
#include "foundationPthread.h"
#include "foundationMutex.h"

/*
 * =====================================================================
 * Function:MutexCreate()
 * Description: create a mutex. 
 * Input:   N/A
 * Output:  N/A
 * Return:  On success mutexId; on failure NULL.
 *======================================================================
 */
MUTEX_ID MutexCreate(void)
{
#ifdef LINUX_OS
    pthread_mutex_t* mutexId = AII_NULL;
    int rval;

    /* allocate memory in MutexCreate,
     * free memory later in MutexDelete */
    mutexId = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (mutexId == AII_NULL)
    {
        return AII_NULL;
    }
     
    rval = Pthread_mutex_init(mutexId);
    if (rval != 0)
    {
        free(mutexId);
        return AII_NULL;
    }
    return mutexId;
#elif VXWORKS_OS
    /* blocking tasks in the order of fifo,
     * the same with linux pthread_mutex */
    return semMCreate(SEM_Q_FIFO);
#endif
}

/*
 * =====================================================================
 * Function:MutexDelete()
 * Description: delete a mutex. 
 * Input:   mutexId -- mutex id
 * Output:  N/A
 * Return:  On success zero; on failure -1.
 *======================================================================
 */
int MutexDelete(MUTEX_ID mutexId)
{
#ifdef LINUX_OS
    int rval;

    if (mutexId == AII_NULL)
    {
        return (-1);
    }

    rval = Pthread_mutex_destroy((pthread_mutex_t*)mutexId);
    free(mutexId);
    mutexId = AII_NULL;
    if (rval != 0)
    {
        return (-1);
    }
    return 0;

#elif VXWORKS
    if (mutexId == AII_NULL)
    {
        return (-1);
    }
    return semDelete(mutexId);
#endif
}

/*
 * =====================================================================
 * Function:MutexLock()
 * Description: lock a mutex. 
 * Input:   mutexId -- mutex id
 *          timeTick -- waiting time while blocking
 *              WAIT_FOREVER,   this routine block until mutex available 
 *              NO_WAIT,        this routine return immediately with errorno
 *              wait time,      this routine block until mutex available or time up
 * Output:  N/A
 * Return:  On success zero; on failure -1.
 *======================================================================
 */
int MutexLock(MUTEX_ID mutexId, int timeTick)
{
#ifdef LINUX_OS
    int rval;

    if (mutexId == AII_NULL)
    {
        return (-1);
    }

    rval = Pthread_mutex_lock((pthread_mutex_t*)mutexId, timeTick);
    if (rval != 0)
    {
        return -1;
    }
    return 0;

#elif VXWORKS_OS
    if (mutexId == AII_NULL)
    {
        return (-1);
    }
    return semTake(mutexId, timeTick);
#endif
}

/*
 * =====================================================================
 * Function:MutexUnlock()
 * Description: unlock a mutex. 
 * Input:   mutexId -- mutex id
 * Output:  N/A
 * Return:  On success zero; on failure -1.
 *======================================================================
 */
int MutexUnlock(MUTEX_ID mutexId)
{
#ifdef LINUX_OS
    int rval;

    if (mutexId == AII_NULL)
    {
        return (-1);
    }

    rval = Pthread_mutex_unlock((pthread_mutex_t*)mutexId);
    if (rval != 0)
    {
        return (-1);
    }
    return 0;

#elif VXWORKS_OS
    if (mutexId == AII_NULL)
    {
        return (-1);
    }
    return semGive(mutexId);
#endif
}

