/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: pthread.c 
 *  Description:pthread APIs 
 *  Author: caoyun
 *  Create:2010-1-25 
 *  Modification history:
 *  2010-01-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#include "foundationPthread.h"
#include "foundationDbg.h"


typedef void *(*FUNCPTR)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*);
typedef void *(*START_ROUTINE)(void*);

/* function wrapper structure */
typedef struct
{
	FUNCPTR entry; /* execution entry point address for thread */
	void *arg[10]; /* arguments */
}FUNC_WRAPPER2;

typedef struct
{
	int (*entry)(int,int,int,int,int,int,int,int,int,int); /* execution entry point address for thread */
	int parms[10]; /* arguments */
}FUNC_WRAPPER;

/*************************************************
  * Function:		Pthread_create()
  * Description:    �̴߳����������� 
  * Input:          *attr---�߳����� 
  *					*start_routine---�߳�ִ�к���
  *					*agr---ִ���̲߳��� 
  * Output:         *tid---�����߳�ID
  * Return:         0---sueccess/-1---fail
*************************************************/
int Pthread_create(
                   pthread_t *tid, 
                   const pthread_attr_t *attr, 
                   void *(*start_routine)(void*), 
                   void *arg
                  )
{
	int rval;

	if(start_routine == NULL)
	{
		return -1;	
	}

	if((rval = pthread_create(tid, attr, start_routine, arg)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "Pthread_create() failed!\n");	
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_self()
  * Description:    ��ȡ�߳�ID�������� 
  * Input:          N/A 
  * Output:         N/A 
  * Return:         �߳�ID 
*************************************************/
int Pthread_self(void)
{
	return pthread_self();	
}

/*************************************************
  * Function:		Pthread_self()
  * Description:    ��ȡ�߳�ID�������� 
  * Input:          N/A 
  * Output:         N/A 
  * Return:         �߳�ID 
*************************************************/
int Pthread_equal(pthread_t tid1, pthread_t tid2)
{
	return pthread_equal(tid1, tid2);
}

/*************************************************
  * Function:		Pthread_cancel()
  * Description:    ����ȡ���̰߳������� 
  * Input:          tid---ȡ���߳�ID 
  * Output:         N/A 
  * Return:         0/errno 
*************************************************/
int Pthread_cancel(pthread_t tid)
{
	int rval;

	if((rval = pthread_cancel(tid)) != 0)
	{
		debug_info(DEBUG_LEVEL_3, "Pthread_cancel() failed!\n");	
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_exit()
  * Description:    �߳��˳��������� 
  * Input:          N/A 
  * Output:         N/A 
  * Return:         N/A 
*************************************************/
void Pthread_exit(void *value)
{
	pthread_exit(value);	
}

/*************************************************
  * Function:		Pthread_join()
  * Description:    �ȴ��߳��˳��������� 
  * Input:          thread---�˳��߳�ID
  *					value---�˳��̷߳���״̬���� 
  * Output:         N/A 
  * Return:         0/errno 
*************************************************/
int Pthread_join(pthread_t thread, void **value)
{
	int rval;

	if((rval = pthread_join(thread, value)) != 0)
	{
		debug_info(DEBUG_LEVEL_3,"Pthread_join() failed!\n");	
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_cleanup_push()
  * Description:    ע���߳�����������������---ʵ��Ϊ��
  *					��������������������
  *					1.����pthread_exitʱ��
  *					2.��Ӧȡ������ʱ(pthread_cancel);
  *					3.�÷���execute����pthread_cleanup_pop.
  * Input:          routine---�߳����������
  *					arg---�߳�������������ָ�� 
  * Output:         N/A 
  * Return:         N/A 
*************************************************/

/*************************************************
  * Function:		Pthread_cleanup_pop()
  * Description:    �����߳�����������������---ʵ��Ϊ��	
  * Input:          execute---��0ʱִ��Pthread_cleanup_push��
  *							  Ϊ0ʱ��ִ��
  * Output:         N/A 
  * Return:         N/A 
*************************************************/

/*************************************************
  * Function:		Pthread_detach()
  * Description:    �̷߳���������� 
  * Input:          tid---�߳�ID 
  * Output:         N/A 
  * Return:         N/A 
*************************************************/
int Pthread_detach(pthread_t tid)
{
	int rval;

	if((rval = pthread_detach(tid)) != 0)
	{
		debug_info(DEBUG_LEVEL_3, "Pthread_detach() failed!\n");	
	}

	return rval;
}

/*************************************************
  * Function:       pthreadIdVerify()
  * Description:    �߳���֤���ں��� 
  * Input:          tid---�߳������ID 
  * Output:         N/A 
  * Return:         0/errno 
*************************************************/
int pthreadIdVerify(pthread_t tid)
{
	int rval;

	if((rval = pthread_kill(tid, 0)) != 0)
	{
		debug_info(DEBUG_LEVEL_3, "pthread_kill(0) failed!\n");	
	}

	return rval;
}

/*************************************************
  * Function:		threadWrapper2()
  * Description:    �߳�ִ�к��� 
  * Input:          arg---�߳�ִ�к�������
  * Output:         N/A 
  * Return:         NULL 
*************************************************/
static void *threadWrapper2(void *arg)
{
	FUNC_WRAPPER2 func = *((FUNC_WRAPPER2 *)arg);	
	free((char *)arg);

	(*(func.entry))(func.arg[0], func.arg[1], func.arg[2], func.arg[3], func.arg[4],
			func.arg[5], func.arg[6], func.arg[7], func.arg[8], func.arg[9]);

	return NULL;
}

/*************************************************
  * Function:       pthreadSpawn()
  * Description:    Linux�´����̵߳ķ�װ������������VxWorks�µĴ����̺߳��� 
  * Input:          priority---�߳����ȼ�
  *                 stacksize---�̶߳�ջ��С
  *                 funcptr---�̵߳�ִ�к���
  *					args---�߳�ִ�к����Ĳ�������
  *					...---�߳�ִ�к����Ĳ��������10��
  * Output:         ptid---�߳�ID 
  * Return:         0/errno 
*************************************************/
int pthreadSpawn(pthread_t *ptid, int priority, size_t stacksize, void *funcptr, unsigned args, ...)
{
	int i, rval;
	void *arg[] = {[0 ... 9] = NULL};
	START_ROUTINE start;
	pthread_t tid, *raw;
	pthread_attr_t attr;
	va_list ap;

	if (funcptr == NULL || args > 10)
	{
		return EINVAL;
	}

	/*�ռ��̲߳���*/
	va_start(ap, args);
	for (i = 0; i < args; i++)
	{
		arg[i] = va_arg(ap, void *);
		//debug_info(DEBUG_LEVEL_1, "pthreadSpawn: arg[%d] = %d\n", i, (int)arg[i]);
	}
	va_end(ap);

	/*�����߳�����*/
	rval = Pthread_attr_set(&attr, priority, stacksize);
	if (rval != 0)	
	{
		return rval;
	}

	/*�����߳�*/
	raw = (ptid != NULL)? ptid:(&tid);
	if (args <= 1)
	{
		rval = Pthread_create(raw, &attr, (START_ROUTINE)funcptr, arg[0]);
	}
	else
	{
		FUNC_WRAPPER2 *func;
		func = (FUNC_WRAPPER2 *)malloc(sizeof(FUNC_WRAPPER2));
		if (func == NULL)
		{
			(void) Pthread_attr_destroy(&attr);
			return ENOMEM;
		}

		start = threadWrapper2;
		memset((char *)func, 0, sizeof(FUNC_WRAPPER2));
		func->entry = funcptr;
		for (i = 0; i < args; i++)
		{
			func->arg[i] = arg[i];
		}
		rval = Pthread_create(raw, &attr, start, (void *)func);
		if (rval != 0)
		{
			free((char *)func);
		}
	}

	/*�ͷ��ѳ�ʼ���߳����Խṹ*/
	Pthread_attr_destroy(&attr); 

	return rval;
}

