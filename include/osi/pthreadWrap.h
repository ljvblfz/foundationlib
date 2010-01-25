/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: pthreadWrap.h 
 *  Description: �̷߳�װ���������ͷ�ļ� 
 *  Author: caoyun
 *  Create:2010-1-25 
 *  Modification history:
 *
 *
 **************************************************************************************/
#ifndef _PTHREADWRAP_H
#define _PTHREADWRAP_H

#include <stdarg.h>
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*uclibc�е�pthread_attr_t�ṹ*/
#if 0
/* Attributes for threads.  */
typedef struct __pthread_attr_s
{
  int __detachstate;					//�̵߳ķ�������
  int __schedpolicy;					//�̵߳ĵ��Ȳ���
  struct __sched_param __schedparam;	//�̵߳ĵ��Ȳ���
  int __inheritsched;					//
  int __scope;							//�̵߳����ȼ���Χ
  size_t __guardsize;					//�߳�ջ�ľ��仺������С
  int __stackaddr_set;				    //	
  void *__stackaddr;					//�߳�ջ���ڴ浥Ԫ����͵�ַ
  size_t __stacksize;					//�߳�ջ�Ĵ�С
} pthread_attr_t;
#endif

/*uclibc�е�sched_param�ṹ*/
#if 0
struct sched_param
{
	int __sched_priority;
};
#endif

#define NO_WAIT	0
#define WAIT_FOREVER (-1)

/*�߳�ԭ��*/
int pthreadSpawn(pthread_t *ptid, int priority, size_t stacksize, void *funcptr, unsigned args, ...);
int Pthread_self(void);
int Pthread_equal(pthread_t tid1, pthread_t tid2);
int Pthread_cancel(pthread_t tid);
void Pthread_exit(void *value);
int Pthread_join(pthread_t thread, void **value);
//Pthread_cleanup_push()��Pthread_cleanup_pop()���������߳���ͬ������������ƥ��Ե���ʽʹ��
#define Pthread_cleanup_push(routine, args) pthread_cleanup_push(void (*routine)(void*), void *args) 
#define Pthread_cleanup_pop(execute) pthread_cleanup_pop(int execute)
int pthreadIdVerify(pthread_t tid);

/*�߳�����ԭ��*/
int Pthread_attr_set(pthread_attr_t *attr, int priority, size_t stacksize);
int Pthread_attr_init(pthread_attr_t *attr);
int Pthread_attr_destroy(pthread_attr_t *attr);
int Pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
int Pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int Pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
int Pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int Pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param * param);
int Pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
int Pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched);
int Pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched);
int Pthread_attr_getschedpriority(const pthread_attr_t *attr, int *priority);
int Pthread_attr_setschedpriority(pthread_attr_t *attr, int priority);
int Pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
int Pthread_attr_setscope(pthread_attr_t *attr, int scope);
int Pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
int Pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int Pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);
int Pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);
int Pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize);
int Pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);

/*����ԭ��*/
int Sched_get_priority_min(int policy);
int Sched_get_priority_max(int policy);
int Sched_getscheduler(pid_t pid);
int Sched_setscheduler(pid_t pid, int policy, struct sched_param *param);
int Sched_getparam(pid_t pid, struct sched_param *param);
int Sched_setparam(pid_t pid, struct sched_param *param);
int Sched_rr_get_interval(pid_t pid, struct timespec *tp);

/*==========================================================
 *					�߳�ͬ��ԭ��
 *=========================================================*/
//������
int Pthread_mutex_init(pthread_mutex_t *mutex);
int Pthread_mutex_destroy(pthread_mutex_t *mutex);
int Pthread_mutex_lock(pthread_mutex_t *mutex, int wait_ms);
int Pthread_mutex_unlock(pthread_mutex_t *mutex);

//�ź���
#define		SEM_EMPTY		0
#define		SEM_FULL		1
int Sem_init(sem_t *sem, unsigned value);
int Sem_destroy(sem_t *sem);
int Sem_post(sem_t *sem);
int Sem_wait(sem_t *sem, int wait_ms);

//��д��(ע��:pthread_rwlock_t ������ϱ���ѡ��ѡ��-D_GNU_SOURCE!!!)
int Pthread_rwlock_init(pthread_rwlock_t *rwlock);
int Pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
int Pthread_rwlock_rdlock(pthread_rwlock_t *rwlock, int wait_ms);
int Pthread_rwlock_wrlock(pthread_rwlock_t *rwlock, int wait_ms);
int Pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

//��������
int Pthread_cond_init(pthread_cond_t *cond);
int Pthread_cond_destroy(pthread_cond_t *cond);
int Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int Pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, int wait_ms);
int Pthread_cond_signal(pthread_cond_t *cond);
int Pthread_cond_broadcast(pthread_cond_t *cond);


//POSIX ��Ϣ����
mqd_t Mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);
int Mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, int wait_ms, unsigned msg_prio);
ssize_t Mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, int wait_ms, unsigned *msg_prio);
int Mq_getattr(mqd_t mqdes, struct mq_attr *mqstat);
int Mq_setattr(mqd_t mqdes, const struct mq_attr *mqstat, struct mq_attr *omqstat);
int Mq_unlink(const char *name);
int Mq_close(mqd_t mqdes);

#ifdef __cplusplus
}
#endif

#endif


