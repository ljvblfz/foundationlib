/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationTask.c
 *  Description: this file contains api list of task operations
 *  Author: chenwangxian
 *  Create:2010-0315
 *  Modification history:
 *  2010-03-15, create the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#include "foundationInclude.h"
#include "foundationPthread.h"


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
TASK_ID TaskCreate(char* taskname, int priority, int stacksize, void* funcptr, int argnums, ...)
{
#ifdef LINUX_OS
    pthread_t pid;
    int rval;

	if (funcptr == NULL || argnums > 10) 
	{
        debug_info(DEBUG_LEVEL_4, "%s %s %d:input error!",__FILE__, __FUNCTION__, __LINE__);
		return (-1);
	}

    rval = pthreadSpawn(&pid, priority, stacksize, funcptr, argnums, ...);
    if (rval != 0)
    {
        debug_info(DEBUG_LEVEL_4, "%s %s %d:pthreadSpawn error!",__FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    return pid;

#elif VXWORKS_OS
	void *arg[] = {[0 ... 9] = NULL};
	va_list ap;
    int tid;

	if (funcptr == NULL || argnums > 10) 
	{
        debug_info(DEBUG_LEVEL_4, "%s %s %d:input error!",__FILE__, __FUNCTION__, __LINE__);
		return (-1);
	}

	va_start(ap, argnums);
	for (i = 0; i < argnums; i++)
	{
		arg[i] = va_arg(ap, void *);
	}
	va_end(ap);

    tid = taskSpawn(NULL, priority, 0, stacksize, funcptr, 
                    arg[0], arg[1], arg[2], arg[3], arg[4],
                    arg[5], arg[6], arg[7], arg[8], arg[9]);
    if (tid == -1)
    {
        debug_info(DEBUG_LEVEL_4, "%s %s %d:pthreadSpawn error!",__FILE__, __FUNCTION__, __LINE__);
        return (-1);
    }
    return tid;
#endif
}

/*
 * =====================================================================
 * Function:TaskIdSelf()
 * Description: This function returns the calling thread's ID. 
 * Input:   N/A
 * Output:  N/A
 * Return:  Calling thread's ID. 
 *======================================================================
 */
TASK_ID TaskIdSelf(void)
{
#ifdef LINUX_OS
    return Pthread_self();
#elif VXWORKS_OS
    return taskIdSelf();
#endif
}

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
int TaskIdVerify(TASK_ID taskId)
{
#ifdef LINUX_OS
    int rval;

    rval = pthreadIdVerify(taskId);
    if (rval != 0)
    {
        return (-1);
    }
    return 0;

#elif VXWORKS_OS
    int rval;

    rval = taskIdVerify(taskId);
    if (rval != 0)
    {
        return (-1);
    }
    return 0;
#endif
}

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
void TaskSleep(int sleepTick)
{
#ifdef LINUX_OS
    int msPerTick;
    msPerTick = getSysMsPerTick();
    usleep(sleepTick*msPerTick*1000);

#elif VXWORKS_OS
    taskDelay(sleepTick);
#endif
}

