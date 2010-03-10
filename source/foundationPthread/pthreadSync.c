/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: pthreadSync.c 
 *  Description: this file contains pthread synchorism apis
 *  Author: caoyun
 *  Create:2010-01-25
 *  Modification history:
 *  2010-01-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/
#include "foundationInclude.h"


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
		debug_info(DEBUG_LEVEL_3, "getTimespec: clock_gettime call fail\n");
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
	//debug_info(DEBUG_LEVEL_1， "getTimespec: after time sec = %ld, time = %ld, nsec = %ld\n", 
	//		tp->tv_sec, time(NULL)+1, tp->tv_nsec);
	return;
}

/*================================================
 *				1.	互斥量：主要用于互斥
 * ==============================================*/
/*************************************************
* Function:		  Pthread_mutex_init()
* Description:    初始化互斥量 
* Input:          *mutex 
* Output:         *mutex 
* Return:         0/errno 
*************************************************/
int Pthread_mutex_init(pthread_mutex_t *mutex)
{
	int	rval;
	
	if(mutex == NULL)
	{
		return -1;	
	}

	if((rval = pthread_mutex_init(mutex, NULL)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "Pthread_mutex_init() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Pthread_mutex_destroy()
* Description:    删除互斥量 
* Input:          *mutex 
* Output:         *mutex 
* Return:         0/errno 
*************************************************/
int Pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	int	rval;

	if(mutex == NULL)
	{
		return -1;	
	}

	if((rval = pthread_mutex_destroy(mutex)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "pthread_mutex_destroy() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:		  Pthread_mutex_lock()
* Description:    锁住互斥量 
* Input:          *mutex
*				  wait_ms---NO_WAIT、WAIT_FOREVER、ms
* Output:         *mutex 
* Return:         0/errno 
*************************************************/
int Pthread_mutex_lock(pthread_mutex_t *mutex, int wait_ms)
{
	int rval;
	struct timespec timeout;
	
	if(mutex == NULL)
	{
		return -1;	
	}

	if (wait_ms == NO_WAIT)
	{
		if((rval = pthread_mutex_trylock(mutex)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "pthread_mutex_trylock() failed!\n");	
		}
	}
	else if (wait_ms == WAIT_FOREVER) 
	{
		if((rval = pthread_mutex_lock(mutex)) != 0)
		{
			debug_info(DEBUG_LEVEL_4, "pthread_mutex_lock() failed!\n");	
		}
	}
	else
	{
#ifdef LINUX_OS
		getTimespec(wait_ms, &timeout);
		if((rval = pthread_mutex_timedlock(mutex, &timeout)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "pthread_mutex_timedlock() failed!\n");	
		}
#endif
#ifdef VXWORKS_OS
        printf("This function is not yet available Vxworks!\n");
        rval = -1;
#endif
	}

	return rval;
}

/*************************************************
* Function:		  Pthread_mutex_unlock()
* Description:    解锁互斥量 
* Input:          *mutex 
* Output:         *mutex 
* Return:         0/errno 
*************************************************/
int Pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	int	rval;

	if(mutex == NULL)
	{
		return -1;	
	}

	if((rval = pthread_mutex_unlock(mutex)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "pthread_mutex_unlock() failed!\n");	
	}

	return rval;
}


/*=====================================================
 *				2.	信号量:主要用于同步
 * ==================================================*/
/*****************************************************
 *				   1.无名信号量
 *用途:用于线程间同步
 *API: sem_init()、sem_destroy()
 * **************************************************/

/*************************************************
* Function:		  Sem_init()
* Description:    初始化无名信号量 
* Input:          *sem
*				  value 
* Output:         *sem 
* Return:         0/-1 
*************************************************/
int Sem_init(sem_t *sem, unsigned value)
{
	int rval;

	if(sem == NULL)
	{
		return -1;	
	}

	/* the semaphore is shared between threads of a process */
	if((rval = sem_init(sem, 0, value)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "sem_init() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:		  Sem_destroy()
* Description:    删除信号量 
* Input:          *sem
* Output:         N/A 
* Return:         0/errno 
*************************************************/
int Sem_destroy(sem_t *sem)
{
	int rval;

	if(sem == NULL)
	{
		return -1;	
	}

	if((rval = sem_destroy(sem)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "sem_destroy() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:		  Sem_post()
* Description:    P操作 
* Input:          *sem 
* Output:         *sem 
* Return:         0/-1 
*************************************************/
int Sem_post(sem_t *sem)
{
	int rval;

	if(sem == NULL)
	{
		return -1;	
	}

	if((rval = sem_post(sem)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "sem_post() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:		  Sem_wait()
* Description:    V操作 
* Input:          *sem
*				  wait_ms 
* Output:         *sem 
* Return:         0/-1 
*************************************************/
int Sem_wait(sem_t *sem, int wait_ms)
{
	int rval;
	struct timespec timeout;

	if(sem == NULL)
	{
		return -1;	
	}

	if (wait_ms == NO_WAIT)
	{
		if((rval = sem_trywait(sem)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "sem_trywait() failed!\n");	
		}
	}
	else if (wait_ms == WAIT_FOREVER) 
	{
		if((rval = sem_wait(sem)) != 0)
		{
			debug_info(DEBUG_LEVEL_4, "sem_wait() failed!\n");	
		}
	}
	else
	{
#ifdef LINUX_OS
		getTimespec(wait_ms, &timeout);
		if((rval = sem_timedwait(sem, &timeout)) != 0)
		{
            /* debug_info(DEBUG_LEVEL_3, "sem_timedwait() failed!\n");	 */
		}
#endif
#ifdef VXWORKS_OS
        printf("This function is not yet available Vxworks!\n");
        rval = -1;
#endif
	}

	return rval;
}

/*****************************************************
 *				   2.有名信号量
 *用途:用于进程间同步，但是linux没有实现
 *API: sem_open()、sem_unlink()、sem_close()
 * **************************************************/
sem_t *Sem_open(const char *name, int oflag, ...)
{
	sem_t *rval;
	va_list args;

	va_start(args, oflag);
	rval = sem_open(name, oflag, args);
	va_end(args);

	return rval;
}

int Sem_unlink(const char *name)
{
	int rval;

	rval = sem_unlink(name);

	return rval;
}

int Sem_close(sem_t *sem)
{
	int rval;

	rval = sem_close(sem);

	return rval;
}


/*======================================================
 *					3.读写锁---共享(读)-独占(写)锁
 *a.比互斥量有更高的并行性
 *b.非常适合于对数据结构读的次数远大于写的情况
 * ====================================================*/
/*************************************************
* Function:		  Pthread_rwlock_Init()
* Description:    初始化读写锁 
* Input:          *rwlock
* Output:         *rwlock 
* Return:         0/errno 
*************************************************/
int Pthread_rwlock_init(pthread_rwlock_t *rwlock)
{
	int	rval;

	if(rwlock == NULL)
	{
		return -1;	
	}

	if((rval = pthread_rwlock_init(rwlock, NULL)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "pthread_rwlock_init() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:		  Pthread_rwlock_destroy()
* Description:    清理读写锁 
* Input:          *rwlock
* Output:         *rwlock 
* Return:         0/errno 
*************************************************/
int Pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
	int	rval;

	if(rwlock == NULL)
	{
		return -1;	
	}

	if((rval = pthread_rwlock_destroy(rwlock)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "pthread_rwlock_destroy() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:		  Pthread_rwlock_rdlock()
* Description:    获取读锁 
* Input:          *rwlock
*				  wait_ms:NO_WAIT、WAIT_FOREVER、(ms未实现) 
* Output:         *rwlock 
* Return:         0/-1 
*************************************************/
int Pthread_rwlock_rdlock(pthread_rwlock_t *rwlock, int wait_ms)
{
	int	rval = 0;
	//struct timespec timeout;

	if(rwlock == NULL)
	{
		return -1;	
	}

	if (wait_ms == NO_WAIT)
	{
		if((rval = pthread_rwlock_tryrdlock(rwlock)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "pthread_rwlock_tryrdlock() failed!\n");	
		}
	}
	else if (wait_ms == WAIT_FOREVER) 
	{
		if((rval = pthread_rwlock_rdlock(rwlock)) != 0)
		{
			debug_info(DEBUG_LEVEL_4, "pthread_rwlock_rdlock() failed!\n");	
		}
	}
#if 0 //linux未实现pthread_rwlock_timedrdlock()
	else
	{
		getTimespec(wait_ms, &timeout);
		if((rval = pthread_rwlock_timedrdlock(rwlock, &timeout)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "pthread_rwlock_timedrdlock() failed!\n");	
		}
	}
#endif

	return rval;
}

/*************************************************
* Function:		  Pthread_rwlock_wrlock()
* Description:    获取写锁 
* Input:          *rwlock
*				  wait_ms:NO_WAIT、WAIT_FOREVER、(ms未实现)
* Output:         *rwlock 
* Return:         0/-1 
*************************************************/
int Pthread_rwlock_wrlock(pthread_rwlock_t *rwlock, int wait_ms)
{
	int	rval = 0;
	//struct timespec timeout;

	if(rwlock == NULL)
	{
		return -1;	
	}

	if (wait_ms == NO_WAIT)
	{
		if((rval = pthread_rwlock_trywrlock(rwlock)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "pthread_rwlock_trywrlock() failed!\n");	
		}
	}
	else if (wait_ms == WAIT_FOREVER) 
	{
		if((rval = pthread_rwlock_wrlock(rwlock)) != 0)
		{
			debug_info(DEBUG_LEVEL_4, "pthread_rwlock_wrlock() failed!\n");	
		}
	}
#if 0 //linux未实现pthread_rwlock_timedwrlock()
	else
	{
		getTimespec(wait_ms, &timeout);
		if((rval = pthread_rwlock_timedwrlock(rwlock, &timeout)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "pthread_rwlock_timedwrlock() failed!\n");	
		}
	}
#endif

	return rval;
}

/*************************************************
* Function:       Pthread_rwlock_unlock()
* Description:    释放读写锁 
* Input:          *rwlock
* Output:         *rwlock 
* Return:         0/-1 
*************************************************/
int Pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
	int	rval;

	if(rwlock == NULL)
	{
		return -1;	
	}

	if((rval = pthread_rwlock_unlock(rwlock)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "pthread_rwlock_unlock() failed!\n");	
	}

	return rval;
}

/*======================================================
 *					4.条件变量
 *条件变量给多个线程提供了一个回合的场所。条件变量与互斥量、
 *一起使用时，允许线程以无竞争的方式等待特定条件的发生。
 * ====================================================*/
/*************************************************
* Function:		  Pthread_cond_init()
* Description:    初始化条件变量 
* Input:          *cond
* Output:         *cond 
* Return:         0/-1 
*************************************************/
int Pthread_cond_init(pthread_cond_t *cond)
{
	int	rval;

	if(cond == NULL)
	{
		return -1;	
	}

	if((rval = pthread_cond_init(cond, NULL)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "pthread_cond_init() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Pthread_cond_destroy()
* Description:    释放条件变量 
* Input:          *cond
* Output:         *cond 
* Return:         0/-1 
*************************************************/
int Pthread_cond_destroy(pthread_cond_t *cond)
{
	int	rval;

	if(cond == NULL)
	{
		return -1;	
	}

	if((rval = pthread_cond_destroy(cond)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "pthread_cond_destroy() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Pthread_cond_wait()
* Description:    获取条件变量(深刻理解!!!, 到底等待多久返回) 
* Input:          *cond
*				  *mutex
* Output:         *cond 
* Return:         0/-1 
*************************************************/
int Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	int	rval;

	if(cond == NULL || mutex == NULL)
	{
		return -1;	
	}

	if((rval = pthread_cond_wait(cond,mutex)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "pthread_cond_wait() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Pthread_cond_timedwait()
* Description:    获取条件变量(深刻理解!!!) 
* Input:          *cond
*				  *mutex
*				  wait_ms---NO_WAIT、ms
* Output:         *cond 
* Return:         0/-1 
*************************************************/
int Pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, int wait_ms)
{
	int	rval;
	struct timespec timeout;

	if(cond == NULL || mutex == NULL)
	{
		return -1;	
	}

	if (wait_ms == NO_WAIT)
	{
		getTimespec(0, &timeout);
		if((rval = pthread_cond_timedwait(cond, mutex, &timeout)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "pthread_cond_timedwait() failed!\n");	
		}
	}
	else
	{
		getTimespec(wait_ms, &timeout);
		if((rval = pthread_cond_timedwait(cond, mutex, &timeout)) != 0)
		{
			debug_info(DEBUG_LEVEL_3, "pthread_cond_timedwait() failed!\n");	
		}
	}

	return rval;
}

/*************************************************
* Function:       Pthread_cond_signal()
* Description:    唤醒等待该条件的某个线程 
* Input:          *cond
* Output:         *cond 
* Return:         0/-1 
*************************************************/
int Pthread_cond_signal(pthread_cond_t *cond)
{
	int	rval;

	if(cond == NULL)
	{
		return -1;	
	}

	if((rval = pthread_cond_signal(cond)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "pthread_cond_signal() failed!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Pthread_cond_broadcast()
* Description:    唤醒等待该条件的所有线程 
* Input:          *cond
* Output:         *cond 
* Return:         0/-1 
*************************************************/
int Pthread_cond_broadcast(pthread_cond_t *cond)
{
	int	rval;

	if(cond == NULL)
	{
		return -1;	
	}

	if((rval = pthread_cond_broadcast(cond)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "pthread_cond_broadcast() failed!\n");	
	}

	return rval;
}

