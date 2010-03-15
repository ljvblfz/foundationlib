/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationTimer.h 
 *  Description: this file contains api list of timer operations
 *  Author: caoyun
 *  Create:2009-04-25
 *  Modification history:
 *  2009-04-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#ifndef _TIMER_API_H
#define _TIMER_API_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * =====================================================================
 * Function:getSysClkRate()
 * Description: Get system clock Rate, times/second
 * Input:   N/A
 * Output:  N/A
 * Return:  clock rate on SUCCESS, -1 otherwise.
 *======================================================================
 */
int getSysClkRate(void);

/*
 * =====================================================================
 * Function:getSysMsPerTick()
 * Description: Get times of one tick, in millisecond.
 * Input:   N/A
 * Output:  N/A
 * Return:  times of millisencod on SUCCESS, -1 otherwise.
 *======================================================================
 */
int getSysMsPerTick(void);

/*=========================================================
 *						定时相关API
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










