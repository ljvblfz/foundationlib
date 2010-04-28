/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: pthreadAttr.c 
 *  Description: 线程属性原语封装库
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *
 *
 **************************************************************************************/
#include "pthreadHead.h"

#ifdef LINUX_OS

/*************************************************
  * Function:		Pthread_attr_init()
  * Description:    线程属性初始化包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *attr---为属性对象分配动态内存 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_init(pthread_attr_t *attr)
{
	int rval;

	if(attr==AII_NULL)
	{
		return -1;	
	}

	if((rval = pthread_attr_init(attr)) != 0)
	{
		debug_info(DEBUG_LEVEL_4, "Pthread_attr_init() failed!\n");
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_destroy()
  * Description:    线程属性清理包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *attr---释放属性对象内存 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_destroy(pthread_attr_t *attr)
{
	int rval;

	if(attr==AII_NULL)
	{
		return -1;	
	}

	if((rval = pthread_attr_destroy(attr)) != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_destroy() failed!\n");
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_getdetachstate()
  * Description:    获取线程分离属性包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *detachstate---线程分离属性 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate)
{
	int rval;

	if(attr==AII_NULL || detachstate==AII_NULL)
	{
		return -1;	
	}

	if((rval = pthread_attr_getdetachstate(attr, detachstate)) != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_getdetacgstate() failed!\n");
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_setdetachstate()
  * Description:    设置线程分离属性包裹函数 
  * Input:          *attr---线程属性结构 
  *					detachstate---线程分离属性
					PTHEARD_CREATE_DETACHED:分离状态
					PTHEARD_CREATE_JOINABLE:默认状态
  * Output:			*attr---线程属性结构 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
	int rval;

	if(attr==AII_NULL)
	{
		return -1;	
	}

	if((rval = pthread_attr_setdetachstate(attr, detachstate)) != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_setdetacgstate() failed!\n");
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_getschedpolicy()
  * Description:    获取线程调度策略属性包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *policy---线程调度策略属性 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy)
{
	int rval;

	if(attr==AII_NULL || policy==AII_NULL)
	{
		return -1;	
	}

	if((rval = pthread_attr_getschedpolicy(attr, policy)) != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_getschedpolicy() failed!\n");
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_setschedpolicy()
  * Description:    设置线程调度策略属性包裹函数 
  * Input:          *attr---线程属性结构 
  *					policy---线程调度策略属性
  					SCHED_OTHER:非实时、普通
					SCHED_FIFO:实时、先进先出
					SCHED_RR: 实时、时间片轮转
  * Output:			*attr---线程属性结构 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
	int rval;

	if(attr==AII_NULL)
	{
		return -1;	
	}

	if((rval = pthread_attr_setschedpolicy(attr, policy)) != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_setschedpolicy() failed!\n");
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_getschedparam()
  * Description:    获取线程调度参数包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *param---线程调度参数 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param * param)
{
	int rval;

	if(attr==AII_NULL || param==AII_NULL)
	{
		return -1;	
	}

	if((rval = pthread_attr_getschedparam(attr, param)) != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_getschedparam() failed!\n");
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_setschedparam()
  * Description:    设置线程调度参数包裹函数 
  * Input:          *attr---线程属性结构 
  *					*param---线程调度参数
  * Output:			*attr---线程属性结构 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param)
{
	int rval;

	if(attr==AII_NULL || param==AII_NULL)
	{
		return -1;	
	}

	if((rval = pthread_attr_setschedparam(attr, param)) != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_setschedparam() failed!\n");
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_getinheritsched()
  * Description:    获取线程继承关系包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *inheritsched---线程继承关系参数 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched)
{
	int rval;

	if(attr==AII_NULL || inheritsched==AII_NULL)
	{
		return -1;	
	}

	if((rval = pthread_attr_getinheritsched(attr, inheritsched)) != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_getinheritsched() failed!\n");
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_setinheritsched()
  * Description:    设置线程继承属性包裹函数 
  * Input:          *attr---线程属性结构 
  *					inheritsched--线程继承关系
  					PTHREAD_INHERIT_SCHED:表示新线程将继承创建线程的调度策略和优先级
  				    PTHREAD_EXPLICIT_SCHED:表示使用在schedpolicy和schedparam属性中显式设置的调度策略和优先级
					!!!要使用在schedpolicy和schedparam属性中显式设置的调度信息,必须在设置之前将inheritsched
						属性设置为PTHREAD_EXPLICIT_SCHED.
  * Output:			*attr---线程属性结构 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched)
{
	int rval;

	if(attr==AII_NULL)
	{
		return -1;	
	}

	if((rval = pthread_attr_setinheritsched(attr, inheritsched)) != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_setinheritsched() failed!\n");
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_getschedpriority()
  * Description:    获取线程优先级包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *priority---线程优先级参数 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_getschedpriority(const pthread_attr_t *attr, int *priority)
{
	struct sched_param	params;
	int rval;

	if(attr==AII_NULL || priority==AII_NULL)
	{
		return -1;	
	}

	rval = Pthread_attr_getschedparam(attr, &params);
	if (rval != 0)
	{
		return rval;
	}

	*priority = params.sched_priority;

	return rval;
}

/*************************************************
  * Function:		Pthread_get_priorityscope()
  * Description:    获取调度策略优先级范围包裹函数 
  * Input:          policy---调度策略 
  * Output:         *minPriority---该调度策略对应的最小线程优先级 
  					*maxPriority---该调度策略对应的最大线程优先级
  * Return:         0/errno 
*************************************************/
static int  Pthread_get_priorityscope(int policy , int *minPriority, int *maxPriority)
{
	int priority;

	if(maxPriority==AII_NULL || minPriority==AII_NULL)
	{
		return -1;	
	}

	if((priority = Sched_get_priority_min(policy)) == -1)
	{
		return -1;	
	}
	else
	{
		*minPriority = priority;
	}
	
	if((priority = Sched_get_priority_max(policy)) == -1)
	{
		return -1;
	}
	else
	{
		*maxPriority = priority;
	}

	return 0;
}

/*************************************************
  * Function:		Pthread_attr_setschedpriority()
  * Description:    设置线程优先级包裹函数 
  * Input:          *attr---线程属性结构 
  					inheritsched---线程优先级参数
  * Output:         *attr---线程属性结构
  * Return:         0/errno 
*************************************************/
int Pthread_attr_setschedpriority(pthread_attr_t *attr, int priority)
{
	struct sched_param	params;
	int maxPriority, minPriority;
	int rval;
	
	if(attr==AII_NULL)
	{
		return -1;	
	}

	/*检查用户设置的优先级合法性*/
	rval = Pthread_get_priorityscope(SCHED_RR, &minPriority, &maxPriority);
	if (rval != 0)
	{
		return rval;
	}	
	//debug_info(DEBUG_LEVEL_1,"pthread sched policy range is %d---%d\n", minPriority, maxPriority);
	if (priority < minPriority)
	{
		priority = minPriority;
	}
	else if (priority > maxPriority)
	{
		priority = maxPriority;
	}

	/*获取线程默认调度参数*/
	rval = Pthread_attr_getschedparam(attr, &params);
	if (rval != 0)
	{
		return rval;
	}

	/*修改线程优先级，设置线程调度属性*/
	params.sched_priority = priority;
	rval = Pthread_attr_setschedparam(attr, &params);
	if (rval != 0)
	{
		return rval;
	}

	return rval;
}

/*************************************************
  * Function:		Pthread_attr_getscope()
  * Description:    获取线程优先级作用域包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *scope--线程优先级作用域 
  * Return:         0/errno 
*************************************************/
int Pthread_attr_getscope(const pthread_attr_t *attr, int *scope)
{
	int rval;
	
	if(attr==AII_NULL || scope==AII_NULL)
	{
		return -1;	
	}

	rval = pthread_attr_getscope(attr, scope);
	if (rval != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_getcope() failed!\n");
	}
	
	return rval;
}

/*************************************************
  * Function:		Pthread_attr_setscope()
  * Description:    设置线程优先级作用域包裹函数 
  * Input:          *attr---线程属性结构 
  					scope---线程优先级作用域
					PTHREAD_SCOPE_SYSTEM:系统范围内争夺CPU
					PTHREAD_SCOPE_PROCESS:同一进程范围内争夺CPU(linux pthread 不支持!!!)
  * Output:         *attr---线程属性结构
  * Return:         0/errno 
*************************************************/
int Pthread_attr_setscope(pthread_attr_t *attr, int scope)
{
	int rval;
	
	if(attr==AII_NULL)
	{
		return -1;	
	}

	rval = pthread_attr_setscope(attr, scope);
	if (rval != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_setcope() failed!\n");
	}
	
	return rval;
}

/*************************************************
  * Function:		Pthread_attr_getstacksize()
  * Description:    获取线程栈大小包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *stacksize--线程栈大小(字节数)
  * Return:         0/errno 
*************************************************/
int Pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize)
{
	int rval;
	
	if(attr==AII_NULL || stacksize==AII_NULL)
	{
		return -1;	
	}

	rval = pthread_attr_getstacksize(attr, stacksize);
	if (rval != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_getstacksize() failed!\n");
	}
	
	return rval;
}

/*************************************************
  * Function:		Pthread_attr_setstacksize()
  * Description:    设置线程栈大小包裹函数 
  * Input:          *attr---线程属性结构 
  					stacksize---线程栈大小(字节数)
					线程栈最小为PTHREAD_STACK_MIN=16k
  * Output:         *attr---线程属性结构
  * Return:         0/errno 
*************************************************/
int Pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
	int rval;
	
	if(attr==AII_NULL)
	{
		return -1;	
	}

	/*检查用户设置的线程栈的合法性*/
	if (stacksize < PTHREAD_STACK_MIN)
	{
		stacksize = PTHREAD_STACK_MIN;
	}

	/*设置线程栈属性*/
	rval = pthread_attr_setstacksize(attr, stacksize);
	if (rval != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_setstacksize() failed!\n");
	}
	
	return rval;
}

/*************************************************
  * Function:		Pthread_attr_getstackaddr()
  * Description:    获取线程栈最低地址包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *stackaddr--线程栈最低地址
  * Return:         0/errno 
*************************************************/
int Pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr)
{
	int rval;
	
	if(attr==AII_NULL || stackaddr==AII_NULL)
	{
		return -1;	
	}

	rval = pthread_attr_getstackaddr(attr, stackaddr);
	if (rval != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_getstackaddr() failed!\n");
	}
	
	return rval;
}

/*************************************************
  * Function:		Pthread_attr_setstackaddr()
  * Description:    设置线程栈的最低地址包裹函数(应用希望自己处理线程栈的分配问题)
  * Input:          *attr---线程属性结构 
  					stackaddr---线程栈内存单元最低地址
  * Output:         *attr---线程属性结构
  * Return:         0/errno 
*************************************************/
int Pthread_attr_setstackaddr(pthread_attr_t *attr,  void *stackaddr)
{
	int rval;
	
	if(attr==AII_NULL)
	{
		return -1;	
	}

	rval = pthread_attr_setstackaddr(attr, stackaddr);
	if (rval != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_setstackaddr() failed!\n");
	}
	
	return rval;
}

/*************************************************
  * Function:		Pthread_attr_getguradsize()
  * Description:    获取线程栈警戒缓冲区大小包裹函数 
  * Input:          *attr---线程属性结构 
  * Output:         *guardsize--线程栈警戒缓冲区大小
  * Return:         0/errno 
*************************************************/
int Pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize)
{
	int rval;
	
	if(attr==AII_NULL || guardsize==AII_NULL)
	{
		return -1;	
	}

	rval = pthread_attr_getguardsize(attr, guardsize);
	if (rval != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_getguardsize() failed!\n");
	}
	
	return rval;
}

/*************************************************
  * Function:		Pthread_attr_setguardsize()
  * Description:    设置线程栈的警戒缓冲区大小包裹函数
  * Input:          *attr---线程属性结构 
  					guardsize---线程栈警戒缓冲区大小
  * Output:         *attr---线程属性结构
  * Return:         0/errno 
*************************************************/
int Pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize)
{
	int rval;
	
	if(attr==AII_NULL)
	{
		return -1;	
	}

	rval = pthread_attr_setguardsize(attr, guardsize);
	if (rval != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_setguardsize() failed!\n");
	}
	
	return rval;
}

/*************************************************
  * Function:		Pthread_attr_set()
  * Description:    设置线程属性包裹函数 
  *					默认线程属性：
  *					1.不继承创建线程的调度策略和参数,使用设置的参数
  *					2.系统范围内争抢CPU
  *					3.线程采用SCHED_RR调度算法
  *					4.线程分离属性
  *					5.线程优先级:在调度策略的优先级minPriority~maxPriority范围内
  *					6.线程栈大小属性：最小为16k
  * Input:          *attr---线程属性结构
  *					priority---线程优先级
  *					stacksize---线程栈大小 
  * Output:         *attr---线程属性结构
  * Return:         0/error 
*************************************************/
int Pthread_attr_set(pthread_attr_t *attr, int priority, size_t stacksize)
{
	int rval;
	
	if(attr == AII_NULL)
	{
		return -1;	
	}

	/*初始化线程属性结构*/
	rval = Pthread_attr_init(attr);
	if (rval != 0)
	{
		return rval;
	}

	/*不继承创建线程的调度策略和参数*/
	rval = Pthread_attr_setinheritsched(attr, PTHREAD_EXPLICIT_SCHED);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	/*系统范围内争抢CPU*/
	rval = Pthread_attr_setscope(attr, PTHREAD_SCOPE_SYSTEM);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	/*线程采用SCHED_RR调度算法*/
	rval = Pthread_attr_setschedpolicy(attr, SCHED_RR);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	/*设置线程分离属性:线程结束后，由系统自动回收资源*/
	rval = Pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	/*设置线程优先级属性*/
	rval = Pthread_attr_setschedpriority(attr, priority);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	/*设置线程栈大小属性*/
	rval = Pthread_attr_setstacksize(attr, stacksize);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	return 0;
}

#endif
