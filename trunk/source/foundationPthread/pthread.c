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
  * Description:    线程创建包裹函数 
  * Input:          *attr---线程属性 
  *					*start_routine---线程执行函数
  *					*agr---执行线程参数 
  * Output:         *tid---创建线程ID
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
  * Description:    获取线程ID包裹函数 
  * Input:          N/A 
  * Output:         N/A 
  * Return:         线程ID 
*************************************************/
int Pthread_self(void)
{
	return pthread_self();	
}

/*************************************************
  * Function:		Pthread_self()
  * Description:    获取线程ID包裹函数 
  * Input:          N/A 
  * Output:         N/A 
  * Return:         线程ID 
*************************************************/
int Pthread_equal(pthread_t tid1, pthread_t tid2)
{
	return pthread_equal(tid1, tid2);
}

/*************************************************
  * Function:		Pthread_cancel()
  * Description:    请求取消线程包裹函数 
  * Input:          tid---取消线程ID 
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
  * Description:    线程退出包裹函数 
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
  * Description:    等待线程退出包裹函数 
  * Input:          thread---退出线程ID
  *					value---退出线程返回状态集合 
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
  * Description:    注册线程清理处理程序包裹函数---实现为宏
  *					三种情况会调用清理函数：
  *					1.调用pthread_exit时；
  *					2.响应取消请求时(pthread_cancel);
  *					3.用非零execute调用pthread_cleanup_pop.
  * Input:          routine---线程清理处理程序
  *					arg---线程清理处理程序参数指针 
  * Output:         N/A 
  * Return:         N/A 
*************************************************/

/*************************************************
  * Function:		Pthread_cleanup_pop()
  * Description:    调用线程清理处理程序包裹函数---实现为宏	
  * Input:          execute---非0时执行Pthread_cleanup_push，
  *							  为0时不执行
  * Output:         N/A 
  * Return:         N/A 
*************************************************/

/*************************************************
  * Function:		Pthread_detach()
  * Description:    线程分离包裹函数 
  * Input:          tid---线程ID 
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
  * Description:    线程验证存在函数 
  * Input:          tid---线程任务的ID 
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
  * Description:    线程执行函数 
  * Input:          arg---线程执行函数参数
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
  * Description:    Linux下创建线程的封装函数，类似于VxWorks下的创建线程函数 
  * Input:          priority---线程优先级
  *                 stacksize---线程堆栈大小
  *                 funcptr---线程的执行函数
  *					args---线程执行函数的参数个数
  *					...---线程执行函数的参数，最多10个
  * Output:         ptid---线程ID 
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

	/*收集线程参数*/
	va_start(ap, args);
	for (i = 0; i < args; i++)
	{
		arg[i] = va_arg(ap, void *);
		//debug_info(DEBUG_LEVEL_1, "pthreadSpawn: arg[%d] = %d\n", i, (int)arg[i]);
	}
	va_end(ap);

	/*设置线程属性*/
	rval = Pthread_attr_set(&attr, priority, stacksize);
	if (rval != 0)	
	{
		return rval;
	}

	/*创建线程*/
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

	/*释放已初始化线程属性结构*/
	Pthread_attr_destroy(&attr); 

	return rval;
}

