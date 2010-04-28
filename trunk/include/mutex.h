/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationMutex.h 
 *  Description: this file contains api list of mutex
 *  Author: chenwangxian
 *  Create:2010-03-09
 *  Modification history:
 *  2010-03-09, created the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#ifndef _MUTEX_API_H
#define _MUTEX_API_H


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LINUX_OS
typedef pthread_mutex_t* MUTEX_ID; 
#elif VXWORKS
typedef SEM_ID           MUTEX_ID; 
#endif

/*
 * =====================================================================
 * Function:MutexCreate()
 * Description: create a mutex. 
 * Input:   N/A
 * Output:  N/A
 * Return:  On success mutexId; on failure NULL.
 *======================================================================
 */
MUTEX_ID MutexCreate(void);

/*
 * =====================================================================
 * Function:MutexDelete()
 * Description: delete a mutex. 
 * Input:   mutexId -- mutex id
 * Output:  N/A
 * Return:  On success zero; on failure -1.
 *======================================================================
 */
int MutexDelete(MUTEX_ID mutexId);

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
int MutexLock(MUTEX_ID mutexId, int timeTick);

/*
 * =====================================================================
 * Function:MutexUnlock()
 * Description: unlock a mutex. 
 * Input:   mutexId -- mutex id
 * Output:  N/A
 * Return:  On success zero; on failure -1.
 *======================================================================
 */
int MutexUnlock(MUTEX_ID mutexId);


#ifdef __cplusplus
}
#endif

#endif

