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

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    int second;     /* [0,59] */
    int minite;     /* [0,59] */
    int hour;       /* [0,23] */
    int day;        /* [1,31] */
    int month;      /* [1,12] */
    int year;       /* [1900,2308] */
}TIME_NORMAL;


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

/*=========================================================
 *						��ʱ���API
 * =======================================================*/
int Timer_create(clockid_t clockId, struct sigevent *eventHandle, timer_t *timerId);
int Timer_delete(timer_t timer);
int	Timer_getoverrun(timer_t timerId);
int Timer_gettime(timer_t timerId, struct itimerspec *ovalue);
int Timer_settime(timer_t timerId, int flags, const struct itimerspec *value, struct itimerspec *ovalue);

#ifdef __cplusplus
}
#endif

#endif










