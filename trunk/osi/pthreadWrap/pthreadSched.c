/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: pthreadSched.c 
 *  Description: �̵߳���ԭ���װ��
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *
 *
 **************************************************************************************/
#include "pthreadHead.h"

/*************************************************
  * Function:		Sched_get_priority_min()
  * Description:    ��ȡ���Ȳ�����С���ȼ��������� 
  * Input:          policy---���Ȳ��� 
  * Output:          
  * Return:         minPriority/errno 
*************************************************/
int Sched_get_priority_min(int policy)
{
	int minPriority;

	if((minPriority = sched_get_priority_min(policy)) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Sched_get_priority_min() failed!\n");	
	}

	return minPriority;
}

/*************************************************
  * Function:		Sched_get_priority_max()
  * Description:    ��ȡ���Ȳ���������ȼ��������� 
  * Input:          policy---���Ȳ��� 
  * Output:          
  * Return:         maxPriority/errno 
*************************************************/
int Sched_get_priority_max(int policy)
{
	int maxPriority;

	if((maxPriority = sched_get_priority_max(policy)) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Sched_get_priority_max() failed!\n");	
	}

	return maxPriority;
}

/*************************************************
  * Function:       Sched_getscheduler()
  * Description:    Retrieve scheduling algorithm for a particular purpose.   
  * Input:          pid---�̶߳�Ӧ���ں˽���ID
  * Output:          
  * Return:         0/errno 
*************************************************/
int Sched_getscheduler(pid_t pid)
{
	int rval;

	if((rval = sched_getscheduler(pid)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "Sched_getscheduler() failed!\n");	
	}

	return rval;
}

/*************************************************
  * Function:       Sched_setscheduler()
  * Description:    Set scheduling algorithm and/or parameters for a process.   
  * Input:          pid---�̶߳�Ӧ���ں˽���ID
  * Output:          
  * Return:         0/errno/-1 
*************************************************/
int Sched_setscheduler(pid_t pid, int policy, struct sched_param *param)
{
	int rval;
	
	if(param == NULL)
	{
		return -1;	
	}

	if((rval = sched_setscheduler(pid, policy, param)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "Sched_setscheduler() failed!\n");	
	}

	return rval;
}

/*************************************************
  * Function:       Sched_getparam()
  * Description:    Retrieve scheduling parameters for a particular process.  
  * Input:          pid---�̶߳�Ӧ���ں˽���ID
  * Output:         param---�̵߳��Ȳ��� 
  * Return:         0/errno/-1 
*************************************************/
int Sched_getparam(pid_t pid, struct sched_param *param)
{
	int rval;
	
	if(param == NULL)
	{
		return -1;	
	}

	if((rval = sched_getparam(pid, param)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "Sched_getparam() failed!\n");	
	}

	return rval;

}

/*************************************************
  * Function:       Sched_setparam()
  * Description:    Set scheduling parameters for a process.   
  * Input:          pid---�̶߳�Ӧ���ں˽���ID
  *                 param---�̵߳��Ȳ���
  * Output:         N/A  
  * Return:         0/errno/-1 
*************************************************/
int Sched_setparam(pid_t pid, struct sched_param *param)
{
	int rval;
	
	if(param == NULL)
	{
		return -1;	
	}

	if((rval = sched_setparam(pid, param)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "Sched_setparam() failed!\n");	
	}

	return rval;

}

/*************************************************
  * Function:       Sched_rr_get_interval()
  * Description:    Get the SCHED_RR interval for the named process. 
  * Input:          pid---�̶߳�Ӧ���ں˽���ID 
  * Output:         tp---�̶߳�Ӧ��ʱ��Ƭ 
  * Return:         0/errno/-1 
*************************************************/
int Sched_rr_get_interval(pid_t pid, struct timespec *tp)
{
	int rval;

	if(tp == NULL)
	{
		return -1;	
	}

	if((rval = sched_rr_get_interval(pid, tp)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "Sched_rr_get_interval() failed!\n");	
	}

	return rval;
}

