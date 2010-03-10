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
	//debug_info(DEBUG_LEVEL_1�� "getTimespec: after time sec = %ld, time = %ld, nsec = %ld\n", 
	//		tp->tv_sec, time(NULL)+1, tp->tv_nsec);
	return;
}

/*================================================
 *				1.	����������Ҫ���ڻ���
 * ==============================================*/
/*************************************************
* Function:		  Pthread_mutex_init()
* Description:    ��ʼ�������� 
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
* Description:    ɾ�������� 
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
* Description:    ��ס������ 
* Input:          *mutex
*				  wait_ms---NO_WAIT��WAIT_FOREVER��ms
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
* Description:    ���������� 
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
 *				2.	�ź���:��Ҫ����ͬ��
 * ==================================================*/
/*****************************************************
 *				   1.�����ź���
 *��;:�����̼߳�ͬ��
 *API: sem_init()��sem_destroy()
 * **************************************************/

/*************************************************
* Function:		  Sem_init()
* Description:    ��ʼ�������ź��� 
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
* Description:    ɾ���ź��� 
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
* Description:    P���� 
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
* Description:    V���� 
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
 *				   2.�����ź���
 *��;:���ڽ��̼�ͬ��������linuxû��ʵ��
 *API: sem_open()��sem_unlink()��sem_close()
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
 *					3.��д��---����(��)-��ռ(д)��
 *a.�Ȼ������и��ߵĲ�����
 *b.�ǳ��ʺ��ڶ����ݽṹ���Ĵ���Զ����д�����
 * ====================================================*/
/*************************************************
* Function:		  Pthread_rwlock_Init()
* Description:    ��ʼ����д�� 
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
* Description:    �����д�� 
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
* Description:    ��ȡ���� 
* Input:          *rwlock
*				  wait_ms:NO_WAIT��WAIT_FOREVER��(msδʵ��) 
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
#if 0 //linuxδʵ��pthread_rwlock_timedrdlock()
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
* Description:    ��ȡд�� 
* Input:          *rwlock
*				  wait_ms:NO_WAIT��WAIT_FOREVER��(msδʵ��)
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
#if 0 //linuxδʵ��pthread_rwlock_timedwrlock()
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
* Description:    �ͷŶ�д�� 
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
 *					4.��������
 *��������������߳��ṩ��һ���غϵĳ��������������뻥������
 *һ��ʹ��ʱ�������߳����޾����ķ�ʽ�ȴ��ض������ķ�����
 * ====================================================*/
/*************************************************
* Function:		  Pthread_cond_init()
* Description:    ��ʼ���������� 
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
* Description:    �ͷ��������� 
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
* Description:    ��ȡ��������(������!!!, ���׵ȴ���÷���) 
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
* Description:    ��ȡ��������(������!!!) 
* Input:          *cond
*				  *mutex
*				  wait_ms---NO_WAIT��ms
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
* Description:    ���ѵȴ���������ĳ���߳� 
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
* Description:    ���ѵȴ��������������߳� 
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

