/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationSem.h 
 *  Description: this file contains api list of semaphore
 *  Author: chenwangxian
 *  Create:2010-03-09
 *  Modification history:
 *  2010-03-09, created the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#ifndef _SEM_API_H
#define _SEM_API_H

#include "libtype.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LINUX_OS
    typedef sem_t* SEM_ID;
#elif VXWORKS
    /* typedef SEM_ID SEM_ID; */
#endif

#define SEM_INIT_LOCK      SEM_EMPTY   /* init sync semaphore lock */
#define SEM_INIT_UNLOCK    SEM_FULL    /* init sync semaphore unlock */

/*
 * =====================================================================
 * Function:SemCreate()
 * Description: create a semaphore to synchromism. 
 * Input:   initValue -- SEM_INIT_LOCK or SEM_INIT_UNLOCK
 * Output:  N/A
 * Return:  On success semId; on failure NULL.
 *======================================================================
 */
SEM_ID SemCreate(unsigned initValue);

/*
 * =====================================================================
 * Function:SemDelete()
 * Description: delete a semaphore. 
 * Input:   semId -- sync semaphore id
 * Output:  N/A
 * Return:  On success zero; on failure -1.
 *======================================================================
 */
int SemDelete(SEM_ID semId);

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
int SemLock(SEM_ID semId, int timeTick);

/*
 * =====================================================================
 * Function:SemUnLock()
 * Description: unlock a semaphore. 
 * Input:   semId -- sync semaphore id
 * Output:  N/A
 * Return:  On success zero; on failure -1.
 *======================================================================
 */
int SemUnlock(SEM_ID semId);

#ifdef __cplusplus
}
#endif

#endif

