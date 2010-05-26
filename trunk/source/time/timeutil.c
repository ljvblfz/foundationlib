/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: timeutil.c
 *  Description: This file contains API for time utilities.
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *  2009-04-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *  2010-05-26, modify all the APIs,        Ben Shaw 
 *
 *
 **************************************************************************************/ 
#include "timeutil.h"

#ifdef LINUX_OS
#include <time.h>
#include <sys/time.h>
#endif

#ifdef VXWORKS_OS
#endif

#ifdef WINDOWS_OS_32
#endif

static UINT64_T gregorian2sec_64(DATE_TIME_PTR date_time_ptr)
{
    UINT64_T dt_sec, days, years, year_64, hour_64, minute_64,second_64;
    UINT32_T year_32;

    if ((date_time_ptr->month -= 2) <= 0)
    {
        date_time_ptr->month += 12;
        date_time_ptr->year -= 1;
    }
    
    year_64 = date_time_ptr->year;
    year_32 = date_time_ptr->year;
    hour_64 = date_time_ptr->hour;
    minute_64 = date_time_ptr->minute;
    second_64 = date_time_ptr->second;

    years = year_32/4 - year_32/100 + year_32/400 + 367*date_time_ptr->month/12 + date_time_ptr->mday;
    days = years + year_64*365-719499;
    dt_sec = ((days*24+hour_64)*60+minute_64)*60+second_64;
    
    return dt_sec;
}

static UINT32_T gregorian2sec_32(DATE_TIME_PTR date_time_ptr)
{
    UINT32_T year, month, day, hour, minute, second;

    if ((INT32_T)(month -= 2) <= 0)
    {
        month += 12;
        year -= 1;
    }

    return ((((UINT32_T)(year/4 - year/100 + year/400 + 367*month/12 + day)
        + year*365 - 719499
        )*24 + hour           
        )*60 + minute 
        )*60 + second; 
}

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
 * WARNING: Even though this function support 64bit computing, however some machine
 *  does not support 64bit integer computing. Thus this function will overflow on 
 *  2106-02-07 06:28:16 on machines were long is 32-bit! (However, as time_t is signed, we
 *  will already get problems at other places on 2038-01-19 03:14:08)
 * 
 */
UINT64_T gregorian2sec(DATE_TIME_PTR date_time_ptr)
{
    if (sizeof(UINT64_T) == 8)
    {
        return gregorian2sec_64(date_time_ptr);
    }
    else
    {
        return gregorian2sec_32(date_time_ptr);
    }
}


/*
 * =====================================================================
 * Function:setCalendarTime()
 * Description: Set time in second
 * Input:  calendarTime -- time in second
 * Output:   N/A
 * Return:  0 on SUCCESS, -1 otherwise.
 *======================================================================
 */
int setCalendarTime(time_t calendarTime)
{
#ifdef LINUX_OS
    struct timeval newtime;

    newtime.tv_sec = calendarTime;
    newtime.tv_usec = 0;

    return settimeofday((const struct timeval*)&newtime, AII_NULL);
#elif VXWORKS_OS
    struct timespec newtime;

    newtime.tv_sec = calendarTime;
    newtime.tv_nsec = 0;

	return clock_settime (CLOCK_REALTIME, (const struct timespec*)&newtime);
#endif
}

/*
 * =====================================================================
 * Function:getCalendarTime()
 * Description: Get time in second
 * Input:  calendarTime -- save space of time in second
 * Output:   N/A
 * Return:  0 on SUCCESS, -1 otherwise.
 *======================================================================
 */
int getCalendarTime(time_t* calendarTime)
{
    time_t curCalendarTime; 

    if (calendarTime == AII_NULL)
    {
        return (-1);
    }
     
    curCalendarTime = time(AII_NULL);
    *calendarTime = curCalendarTime;

    return 0;
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
#ifdef LINUX_OS
    struct timeval newtime;
    unsigned long timeSec;

    if (normalTime == AII_NULL)
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
    newtime.tv_usec = 0;
    return settimeofday((const struct timeval*)&newtime, AII_NULL);
#elif VXWORKS_OS
    struct timespec newtime;
    unsigned long timeSec;

    if (normalTime == AII_NULL)
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
	return clock_settime (CLOCK_REALTIME, (const struct timespec*)&newtime);
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
    /*     int tm_mday;	|+ day of the month		    - [1, 31] +|      */
    /*     int tm_mon;	|+ months since January		- [0, 11] +|  */
    /*     int tm_year;	|+ years since 1900	+|                    */
    /*     int tm_wday;	|+ days since Sunday		- [0, 6] +|   */
    /*     int tm_yday;	|+ days since January 1		- [0, 365] +| */
    /*     int tm_isdst;	|+ Daylight Saving Time flag +|       */
    /* };                                                     */
    time_t curCalendarTime; 
    struct tm curGmTime;

    if (normalTime == AII_NULL)
    {
        return (-1);
    }
     
    curCalendarTime = time(AII_NULL);
    gmtime_r((const time_t*)&curCalendarTime, (struct tm*)&curGmTime);
    normalTime->second = curGmTime.tm_sec;
    normalTime->minite = curGmTime.tm_min;
    normalTime->hour = curGmTime.tm_hour;
    normalTime->day = curGmTime.tm_mday;
    normalTime->month = curGmTime.tm_mon + 1;
    normalTime->year = curGmTime.tm_year + 1900;

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

