/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationTime.c
 *  Description: this file contains api list of posix time operations
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *  2009-04-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#include "foundationInclude.h"
#include "foundationTime.h"


/* Converts Gregorian date to seconds since 1970-01-01 00:00:00.
 * Assumes input in normal date format, i.e. 1980-12-31 23:59:59
 * => year=1980, mon=12, day=31, hour=23, min=59, sec=59.
 *
 * [For the Julian calendar (which was used in Russia before 1917,
 * Britain & colonies before 1752, anywhere else before 1582,
 * and is still in use by some communities) leave out the
 * -year/100+year/400 terms, and add 10.]
 *
 * This algorithm was first published by Gauss (I think).
 *
 * WARNING: this function will overflow on 2106-02-07 06:28:16 on
 * machines were long is 32-bit! (However, as time_t is signed, we
 * will already get problems at other places on 2038-01-19 03:14:08)
 */
unsigned long convertTime (unsigned int year, unsigned int mon,
	       	unsigned int day, unsigned int hour,
	       	unsigned int minute, unsigned int sec)
{

	if ( 0 >= (int) (mon -= 2) ) {        /* 1..12 -> 11,12,1..10 */
		mon += 12;      /* Puts Feb last since it has leap day */
		year -= 1;
	}

	return
		((((unsigned long)((year/4 - year/100) + year/400 + 367*mon/12 + day)
		   + year*365 - 719499
		  )*24 + hour 		/* now have hours */
		 )*60 + minute 		/* now have minutes */
		)*60 + sec; 		/* finally seconds */
}

/*
 * =====================================================================
 * Function:setNormalTime()
 * Description: Set time in our struct
 * Input:  normalTime
    typedef struct
    {
        int second;     ,[0,59] 
        int minite;     ,[0,59] 
        int hour;       ,[0,23]
        int day;        ,[1,31]
        int month;      ,[1,12]
        int year;       ,[1900,2038]
    }TIME_NORMAL;
 * Output:   N/A
 * Return:  0 on SUCCESS, -1 otherwise.
 *======================================================================
 */
int setNormalTime(const TIME_NORMAL* normalTime)
{
    struct timeval newtime;
    unsigned long timeSec;

    if (normalTime == NULL)
    {
        return (-1);
    }
    if (normalTime->year > 2038 ||normalTime->month > 12 ||normalTime->day > 31
        ||normalTime->hour > 23 ||normalTime->minite > 59 ||normalTime->second > 59)
    {
        return (-1);
    }

    timeSec = convertTime(normalTime->year, normalTime->month, normalTime->day,
                          normalTime->hour, normalTime->minite, normalTime->second);
    newtime.tv_sec = timeSec;
    newtime.tv_nsec = 0;
#ifdef LINUX_OS
    return settimeofday((const struct timeval*)&newtime, NULL);
#elif VXWORKS_OS
	return clock_settime (CLOCK_REALTIME, (const struct timeval*)&newtime);
#endif
}

/*
 * =====================================================================
 * Function:getNormalTime()
 * Description: Get time in our struct
 * Input:   N/A
 * Output:  normalTime
    typedef struct
    {
        int second;     ,[0,59] 
        int minite;     ,[0,59] 
        int hour;       ,[0,23]
        int day;        ,[1,31]
        int month;      ,[1,12]
        int year;       ,[1900,2308]
    }TIME_NORMAL;
 * Return:  0 on SUCCESS, -1 otherwise.
 *======================================================================
 */
int getNormalTime(TIME_NORMAL* normalTime)
{
    /* struct tm                                                  */
    /* {                                                      */
    /*     int tm_sec;	|+ seconds after the minute	- [0, 59] +|  */
    /*     int tm_min;	|+ minutes after the hour	- [0, 59] +|  */
    /*     int tm_hour;	|+ hours after midnight		- [0, 23] +|  */
    /*     int tm_mday;	|+ day of the month		- [1, 31] +|      */
    /*     int tm_mon;	|+ months since January		- [0, 11] +|  */
    /*     int tm_year;	|+ years since 1900	+|                    */
    /*     int tm_wday;	|+ days since Sunday		- [0, 6] +|   */
    /*     int tm_yday;	|+ days since January 1		- [0, 365] +| */
    /*     int tm_isdst;	|+ Daylight Saving Time flag +|       */
    /* };                                                     */
    time_t curCalendarTime; 
    struct tm curGmTime;

    if (normalTime == NULL)
    {
        return (-1);
    }
     
    curCalendarTime = time(NULL);
    gmtime_r((const time_t*)&curCalendarTime, (struct tm*)curGmTime);
    normalTime->second = curGmTime.tm_sec;
    normalTime->minite = curGmTime.tm_min;
    normalTime->hour = curGmTime.tm_hour;
    normalTime->day = curGmTime.tm_mday;
    normalTime->month = curGmTime.tm_mon + 1;
    normalTime->year = curGmTime.tm_year + 1900;
    normalTime->wday = curGmTime.tm_wday;

    return 0;
}

/*
 * =====================================================================
 * Function:getSysClkRate()
 * Description: Get system clock Rate, times/second
 * Input:   N/A
 * Output:  N/A
 * Return:  clock rate(perSecond) on SUCCESS, -1 otherwise.
 *======================================================================
 */
int getSysClkRate(void)
{
#ifdef LINUX_OS
    return 100;
#elif VXWORKS_OS
    return sysClkRateGet();
#endif
}

/*
 * =====================================================================
 * Function:getSysMsPerTick()
 * Description: Get times of one tick, in millisecond.
 * Input:   N/A
 * Output:  N/A
 * Return:  times of millisencod(ms) on SUCCESS, -1 otherwise.
 *======================================================================
 */
int getSysMsPerTick(void)
{
	int ms;
	
	ms = (1*1000)/getSysClkRate();		//系统时钟节拍：10毫秒

	return ms;	
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

