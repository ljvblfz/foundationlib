/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationTimer.c
 *  Description: this file contains api list of posix timer operations
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *  2009-04-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#include "foundationInclude.h"

int getSysClkRate(void)
{
#ifdef LINUX_OS
    return 100;
#elif VXWORKS_OS
    return sysClkRateGet();
#endif
}

int getSysMsPerTick(void)
{
	int ms;
	
	ms = (1*1000)/getSysClkRate();		//ϵͳʱ�ӽ��ģ�10����

	return ms;	
}

#if 0
int msleep(unsigned int ms)
{
	return usleep(ms*1000);
}

int taskDelay(int ticks)
{
	int msPertick;

	msPertick = getSysMsPerTick();
	msleep(msPertick*ticks);

	return 0;	
}
#endif


/*=========================================================
 *						��ʱ���API
 * =======================================================*/
/*************************************************
* Function:		  Timer_create()
* Description:    ����POSIXʵʱ��չ��ʱ�� 
* Input:          clockId---��ʱ������
* Output:         eventHandle---��ʱ������ʱ����ʽ 
*				  timerId---�����Ķ�ʱ��ID
* Return:         0/errno 
*************************************************/
int Timer_create(clockid_t clockId, struct sigevent *eventHandle, timer_t *timerId)
{
	int rval;

	if((rval = timer_create(clockId, eventHandle, timerId)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "%s%d: timer_create error!\n", __FILE__, __LINE__);	
	}

	return rval;	
}

/*************************************************
* Function:		  Timer_delete()
* Description:    ɾ���Ѵ�����POSIXʵʱ��չ��ʱ�� 
* Input:          timer---��ʱ��
* Output:         N/A 
* Return:         0/errno 
*************************************************/
int Timer_delete(timer_t timer)
{
	int rval;

	if((rval = timer_delete(timer)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "%s%d: timer_delete error!\n", __FILE__, __LINE__);	
	}

	return rval;	
}

/*************************************************
* Function:		  Timer_getoverrun()
* Description:    ����timerID��ȡ��ʱ��overruns 
* Input:          clockId---��ʱ������
* Output:         N/A 
* Return:         overruns/errno 
*************************************************/
int	Timer_getoverrun(timer_t timerId)
{
	int overruns; 

	if((overruns = timer_getoverrun(timerId)) == EINVAL)
	{
		debug_info(DEBUG_LEVEL_4, "%s%d: timer_getoverrun error!\n", __FILE__, __LINE__);	
	}

	return overruns;
}

/*************************************************
* Function:		  Timer_gettime()
* Description:    ����timerID��ȡ��ʱ��ʣ�ඨʱʱ�� 
* Input:          clockId---��ʱ������
* Output:         ovalue---��ʱ��ʣ�ඨʱʱ�� 
* Return:         0/errno 
*************************************************/
int Timer_gettime(timer_t timerId, struct itimerspec *ovalue)
{
	int rval;
	
	if((rval = timer_gettime(timerId, ovalue)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "%s%d: timer_gettime error!\n", __FILE__, __LINE__);	
	}

	return rval;	
}

/*************************************************
* Function:		  Timer_settime()
* Description:    ����timerID���ö�ʱ�� 
* Input:          clockId---��ʱ������
* Output:         value---��ʱ����ʱʱ�� 
*				  ovalue---��ʱ��ʣ�ඨʱʱ�� 
* Return:         0/errno 
*************************************************/
int Timer_settime(timer_t timerId, int flags, const struct itimerspec *value, struct itimerspec *ovalue)
{
	int rval;
	
	if((rval = timer_settime(timerId, flags, value, ovalue)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "%s%d: timer_settime error!\n", __FILE__, __LINE__);	
	}

	return rval;	
}

