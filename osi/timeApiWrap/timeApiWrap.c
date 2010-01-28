/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: timeApiWrap.c
 *  Description: 时间管理API封装 
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#include "timeHeader.h"

#ifdef LINUX_OS
int msleep(unsigned int ms)
{
	return usleep(ms*1000);
}

int getSysHZ(void)
{
    return 100;
}

int getSysMsPerTick(void)
{
	int tick;
	
	tick = (1/getSysHZ())*1000;		//系统时钟节拍：10毫秒

	return tick;	
}

int taskDelay(int ticks)
{
	int msPertick;

	msPertick = getSysMsPerTick();
	msleep(msPertick*ticks);

	return 0;	
}

/*=========================================================
 *						定时相关API
 * =======================================================*/
/*************************************************
* Function:		  Timer_create()
* Description:    创建POSIX实时扩展定时器 
* Input:          clockId---定时器类型
* Output:         eventHandle---定时器到期时处理方式 
*				  timerId---创建的定时器ID
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
* Description:    删除已创建的POSIX实时扩展定时器 
* Input:          timer---定时器
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
* Description:    根据timerID获取定时器overruns 
* Input:          clockId---定时器类型
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
* Description:    根据timerID获取定时器剩余定时时间 
* Input:          clockId---定时器类型
* Output:         ovalue---定时器剩余定时时间 
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
* Description:    根据timerID设置定时器 
* Input:          clockId---定时器类型
* Output:         value---定时器定时时间 
*				  ovalue---定时器剩余定时时间 
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

#endif

