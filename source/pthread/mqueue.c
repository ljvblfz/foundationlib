/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: mqueue.c 
 *  Description: this file contains POSIX mqueue api list
 *  Author: caoyun
 *  Create:2009-04-01 
 *  Modification history:
 *  2009-04-01, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/
#include "foundationPthread.h"
#include "foundationDbg.h"

static inline void getTimespec(int wait_ms, struct timespec *tp)
{
	time_t sec, t;
	long long nsec;

	sec = 0;
	if (wait_ms == NO_WAIT)
	{
		nsec = 0;
	}
	else
	{
		nsec = wait_ms * 1000000LL;
	}

	if (clock_gettime(CLOCK_REALTIME, tp) == -1)
	{
		debug_info(DEBUG_LEVEL_1,"getTimespec: clock_gettime call fail, error %d(%s)\n", errno, strerror(errno));
		tp->tv_sec = time(NULL) + 1;
		tp->tv_nsec = 0;
	}
	else
	{
		t = time(NULL) + 1;
		if ((int)(tp->tv_sec - t) > 30) 
		{
			tp->tv_sec = t;
			tp->tv_nsec = 0;
		}
	}

	nsec += tp->tv_nsec;
	//debug_info(DEBUG_LEVEL_1, "getTimespec: current time sec = %ld, time = %ld, nsec = %ld, total nsec = %lld\n", 
	//		tp->tv_sec, time(NULL)+1, tp->tv_nsec, nsec);
	if (nsec >= 1000000000)
	{
		sec = nsec / 1000000000;
		nsec = nsec % 1000000000;
	}
	tp->tv_sec += sec;
	tp->tv_nsec = nsec;
	//debug_info(DEBUG_LEVEL_1, "getTimespec: after time sec = %ld, time = %ld, nsec = %ld\n", 
	//		tp->tv_sec, time(NULL)+1, tp->tv_nsec);
	return;
}

/*************************************************
* Function:       Mq_open()
* Description:    打开/创建一个消息队列 
* Input:          name---消息队列名称
*                 oflag---消息队列标志位
*                 mode---动作模式
*                 attr---消息队列属性
* Output:         N/A 
* Return:         0/errno
*************************************************/
mqd_t Mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr) 
{
	mqd_t rval;

	if((rval = mq_open(name, oflag, mode, attr)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "mq_open() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Mq_send()
* Description:    发送消息 
* Input:          mqdes---消息队列描述符
*                 msg_ptr---消息指针
*                 msg_len---消息长度
*                 wait_ms---发送消息等待时间
*                 msg_prio---消息优先级
* Output:         N/A 
* Return:         0/errno 
*************************************************/
int Mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, int wait_ms, unsigned msg_prio)
{
	int rval;
	struct timespec timeout;
    struct mq_attr omq_attr;

	if (wait_ms == NO_WAIT)
	{
        /* set msgQ nonblock, it returns immediately if msgQ blocking */
        omq_attr.mq_flags = O_NONBLOCK;
		if((rval = mq_setattr(mqdes, &omq_attr, NULL)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "mq_setattr() failed!\n");	
		}
        else
        {
            if((rval = mq_send(mqdes, msg_ptr, msg_len, msg_prio)) != 0)
            {
                debug_info(DEBUG_LEVEL_3, "mq_send() failed!\n");	
            }
        }
	}
	else if (wait_ms == WAIT_FOREVER) 
	{
        /* set msgQ block, it waits forever if msgQ blocking */
        omq_attr.mq_flags = 0;
		if((rval = mq_setattr(mqdes, &omq_attr, NULL)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "mq_setattr() failed!\n");	
		}
        else
        {
            if((rval = mq_send(mqdes, msg_ptr, msg_len, msg_prio)) != 0)
            {
                debug_info(DEBUG_LEVEL_3, "mq_send() failed!\n");	
            }
        }
	}
	else
	{
#ifdef LINUX_OS
		getTimespec(wait_ms, &timeout);
		if((rval = mq_timedsend(mqdes, msg_ptr, msg_len, msg_prio, &timeout)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "mq_timedsend() failed!\n");	
		}
#elif VXWORKS_OS
        printf("This function is not yet available Vxworks!\n");
        rval = -1;
#endif
	}

	return rval;
}

/*************************************************
* Function:       Mq_receive()
* Description:    接收消息 
* Input:          mqdes---消息队列描述符
*                 msg_ptr---消息指针
*                 msg_len---消息长度
*                 wait_ms---发送消息等待时间
*                 msg_prio---消息优先级
* Output:         N/A 
* Return:         0/errno 
*************************************************/
ssize_t Mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, int wait_ms, unsigned *msg_prio)
{
	ssize_t rval;
	struct timespec timeout;

	if (wait_ms == WAIT_FOREVER)
	{
		if((rval = mq_receive(mqdes, msg_ptr, msg_len, msg_prio)) != 0)
		{
            //debug_info(DEBUG_LEVEL_3, "mq_recrive() failed!\n");	 
		}
	}
	else
	{
#ifdef LINUX_OS
		getTimespec(wait_ms, &timeout);
		if((rval = mq_timedreceive(mqdes, msg_ptr, msg_len, msg_prio, &timeout)) != 0)
		{
			//debug_info(DEBUG_LEVEL_3, "mq_timedrecrive() failed!\n");	
		}
#elif VXWORKS_OS
        printf("This function is not yet available Vxworks!\n");
        rval = -1;
#endif
	}

	return rval;
}

/*************************************************
* Function:       Mq_getattr()
* Description:    获取消息队列属性 
* Input:          mqdes---消息队列描述符
*                 mqstat---消息队列属性
* Output:         N/A 
* Return:         0/errno 
*************************************************/
int Mq_getattr(mqd_t mqdes, struct mq_attr *mqstat)
{
	int rval;

	if((rval = mq_getattr(mqdes, mqstat)))
	{
		debug_info(DEBUG_LEVEL_3, "mq_getattr() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Mq_setattr()
* Description:    设置消息队列属性 
* Input:          mqdes---消息队列描述符
*                 mqstat---
*                 omqstat---
* Output:         N/A 
* Return:         0/errno 
*************************************************/
int Mq_setattr(mqd_t mqdes, const struct mq_attr *mqstat, struct mq_attr *omqstat)
{
	int rval;

	if((rval = mq_setattr(mqdes, mqstat, omqstat)) != 0)
	{
		debug_info(DEBUG_LEVEL_3, "mq_setattr() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Mq_unlink()
* Description:    清除之前使用的消息队列 
* Input:          name---消息队列名称 
* Output:         N/A 
* Return:         0/errno 
*************************************************/
int Mq_unlink(const char *name)
{
	int rval;

	if((rval = mq_unlink(name)) != 0)
	{
		//debug_info(DEBUG_LEVEL_3, "mq_unlink() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Mq_close()
* Description:    关闭消息队列 
* Input:          mqdes---消息队列描述符 
* Output:         N/A 
* Return:         0/errno 
*************************************************/
int Mq_close(mqd_t mqdes)
{
	int rval;

	if((rval = mq_close(mqdes)) != 0)
	{
		debug_info(DEBUG_LEVEL_3, "mq_close() failed!\n");	
	}

	return rval;
}

