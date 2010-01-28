/**************************************************************************************
 *	Copyright 2003-2010 Hangzhou Hikvision Digital Technology Co., Ltd. 
 *  Filename: timeApiWrap.h 
 *  Description: ʱ��API��װͷ�ļ� 
 *  Author: caoyun
 *  Create:2009-04-1 
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#ifndef _TIME_API_WRAP_H
#define _TIME_API_WRAP_H


#ifdef __cplusplus
extern "C"
{
#endif

//unsigned int sleep(unsigned int seconds);
//int usleep(useconds_t usec);
int msleep(unsigned int ms);
int getSysMsPerTick(void);
int taskDelay(int ticks);

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










