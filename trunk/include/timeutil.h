/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationTime.h 
 *  Description: this file contains api list of time operations
 *  Author: caoyun
 *  Create:2009-04-25
 *  Modification history:
 *  2009-04-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#ifndef _TIME_API_H
#define _TIME_API_H

#include "libtype.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    INT32_T second;  /* seconds after the minute - [0,59] */
    INT32_T minute;  /* minutes after the hour - [0,59] */
    INT32_T hour;    /* hours since midnight - [0,23] */
    INT32_T mday;    /* day of the month - [1,31] */
    INT32_T month;   /* months since January - [1,12] */
    INT32_T year;    /* years since 0000 */
    INT32_T week;    /* days since Sunday - [0,6] */
    INT32_T yday;    /* days since January 1 - [0,365] */
    INT32_T isdst;   /* daylight savings time flag */
}GRE_DATE_TIME,*GRE_DATE_TIME_PTR;

typedef struct
{
    INT64_T tv_sec;     /* seconds */
    INT32_T tv_nsec;    /* nanoseconds (0 - 1,000,000,000) */
}TIMESPEC,*TIMESPEC_PTR;

typedef INT64_T TIME_T;

/*
 * =====================================================================
 * Function:setCalendarTime()
 * Description: Set time in second
 * Input:  calendarTime -- time in second
 * Output:   N/A
 * Return:  0 on SUCCESS, -1 otherwise.
 *======================================================================
 */
int setCalendarTime(time_t calendarTime);

/*
 * =====================================================================
 * Function:getCalendarTime()
 * Description: Get time in second
 * Input:  calendarTime -- save space of time in second
 * Output:   N/A
 * Return:  0 on SUCCESS, -1 otherwise.
 *======================================================================
 */
int getCalendarTime(time_t* calendarTime);

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
        int year;       ,[1900,2308]
    }TIME_NORMAL;
 * Output:   N/A
 * Return:  0 on SUCCESS, -1 otherwise.
 *======================================================================
 */
int setNormalTime(const TIME_NORMAL* normalTime);

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
int getNormalTime(TIME_NORMAL* normalTime);

/*
 * =====================================================================
 * Function:getSysClkRate()
 * Description: Get system clock Rate, times/second
 * Input:   N/A
 * Output:  N/A
 * Return:  clock rate(perSecond) on SUCCESS, -1 otherwise.
 *======================================================================
 */
int getSysClkRate(void);

/*
 * =====================================================================
 * Function:getSysMsPerTick()
 * Description: Get times of one tick, in millisecond.
 * Input:   N/A
 * Output:  N/A
 * Return:  times of millisencod(ms) on SUCCESS, -1 otherwise.
 *======================================================================
 */
int getSysMsPerTick(void);


#ifdef __cplusplus
}
#endif

#endif










