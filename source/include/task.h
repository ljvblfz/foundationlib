/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationTask.h 
 *  Description: this file contains api list of task operations, create, delete, sleep...
 *  Author: chenwangxian
 *  Create:2010-03-09
 *  Modification history:
 *  2010-03-09, created the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#ifndef _TASK_API_H
#define _TASK_API_H


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LINUX_OS 
typedef pthread_t   TASK_ID;
#elif VXWORKS_OS
typedef int         TASK_ID; 
#endif

/*==========================================================
 *				Private task library interface
 * TaskCreate()        -- create task
 * TaskIdSelf()        -- return calling task id
 * TaskIdVerify()      -- check task validity
 * TaskSleep()         -- sleep and delay the task
 *=========================================================*/

/*
 * =====================================================================
 * Function:TaskCreate()
 * Description: This routine creates and activates a new task with a specified priority 
 *              and stacksize and returns a system-assigned ID.
 * Input:   taskname -- name of new task, just for Vxworks.
 *                      In Vxworks,This name will appear in displays generated by various system 
 *                      information facilities such as i( ). If name is specified as NULL, an ASCII 
 *                      name will be assigned to * the task of the form number.
 *          priority -- priority of new task
 *          stacksize -- size(bytes) of stack
 *          funcptr -- task entry point
 *          argnums -- argument amount, support max to 10
 *          ... -- aruments if neccessary
 * Output:  N/A
 * Return:  The task ID, or ERROR if memory is insufficient or the task cannot be created. 
 *======================================================================
 */
TASK_ID TaskCreate(char* taskname, int priority, int stacksize, void* funcptr, int argnums, ...);

/*
 * =====================================================================
 * Function:TaskIdSelf()
 * Description: This function returns the calling thread's ID. 
 * Input:   N/A
 * Output:  N/A
 * Return:  Calling thread's ID. 
 *======================================================================
 */
TASK_ID TaskIdSelf(void);

/*
 * =====================================================================
 * Function:TaskIdVerify()
 * Description: verify the existence of a pthread 
 *              This routine verifies the existence of a specified task by validating the task ID. 
 * Input:   taskId -- id of task
 * Output:  N/A
 * Return:  returns 0 if task is valid, -1 if invalid
 *======================================================================
 */
int TaskIdVerify(TASK_ID taskId);

/*
 * =====================================================================
 * Function:TaskSleep()
 * Description: sleep a task from executing for a period (in ticks)
 *              This routine causes the calling task to relinquish the CPU for the duration specified (in ticks). 
 *              This is commonly referred to as manual rescheduling, but it is also useful when waiting for some 
 *              external condition that does not have an interrupt associated with it.
 * Input:   sleepTick -- sleep time (in ticks) 
 * Output:  N/A
 * Return:  N/A
 *======================================================================
 */
void TaskSleep(int sleepTick);


#ifdef __cplusplus
}
#endif

#endif

