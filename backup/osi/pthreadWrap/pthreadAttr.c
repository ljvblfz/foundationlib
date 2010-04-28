/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: pthreadAttr.c 
 *  Description: �߳�����ԭ���װ��
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
  * Description:    �߳����Գ�ʼ���������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *attr---Ϊ���Զ�����䶯̬�ڴ� 
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
  * Description:    �߳���������������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *attr---�ͷ����Զ����ڴ� 
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
  * Description:    ��ȡ�̷߳������԰������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *detachstate---�̷߳������� 
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
  * Description:    �����̷߳������԰������� 
  * Input:          *attr---�߳����Խṹ 
  *					detachstate---�̷߳�������
					PTHEARD_CREATE_DETACHED:����״̬
					PTHEARD_CREATE_JOINABLE:Ĭ��״̬
  * Output:			*attr---�߳����Խṹ 
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
  * Description:    ��ȡ�̵߳��Ȳ������԰������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *policy---�̵߳��Ȳ������� 
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
  * Description:    �����̵߳��Ȳ������԰������� 
  * Input:          *attr---�߳����Խṹ 
  *					policy---�̵߳��Ȳ�������
  					SCHED_OTHER:��ʵʱ����ͨ
					SCHED_FIFO:ʵʱ���Ƚ��ȳ�
					SCHED_RR: ʵʱ��ʱ��Ƭ��ת
  * Output:			*attr---�߳����Խṹ 
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
  * Description:    ��ȡ�̵߳��Ȳ����������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *param---�̵߳��Ȳ��� 
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
  * Description:    �����̵߳��Ȳ����������� 
  * Input:          *attr---�߳����Խṹ 
  *					*param---�̵߳��Ȳ���
  * Output:			*attr---�߳����Խṹ 
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
  * Description:    ��ȡ�̼̳߳й�ϵ�������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *inheritsched---�̼̳߳й�ϵ���� 
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
  * Description:    �����̼̳߳����԰������� 
  * Input:          *attr---�߳����Խṹ 
  *					inheritsched--�̼̳߳й�ϵ
  					PTHREAD_INHERIT_SCHED:��ʾ���߳̽��̳д����̵߳ĵ��Ȳ��Ժ����ȼ�
  				    PTHREAD_EXPLICIT_SCHED:��ʾʹ����schedpolicy��schedparam��������ʽ���õĵ��Ȳ��Ժ����ȼ�
					!!!Ҫʹ����schedpolicy��schedparam��������ʽ���õĵ�����Ϣ,����������֮ǰ��inheritsched
						��������ΪPTHREAD_EXPLICIT_SCHED.
  * Output:			*attr---�߳����Խṹ 
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
  * Description:    ��ȡ�߳����ȼ��������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *priority---�߳����ȼ����� 
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
  * Description:    ��ȡ���Ȳ������ȼ���Χ�������� 
  * Input:          policy---���Ȳ��� 
  * Output:         *minPriority---�õ��Ȳ��Զ�Ӧ����С�߳����ȼ� 
  					*maxPriority---�õ��Ȳ��Զ�Ӧ������߳����ȼ�
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
  * Description:    �����߳����ȼ��������� 
  * Input:          *attr---�߳����Խṹ 
  					inheritsched---�߳����ȼ�����
  * Output:         *attr---�߳����Խṹ
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

	/*����û����õ����ȼ��Ϸ���*/
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

	/*��ȡ�߳�Ĭ�ϵ��Ȳ���*/
	rval = Pthread_attr_getschedparam(attr, &params);
	if (rval != 0)
	{
		return rval;
	}

	/*�޸��߳����ȼ��������̵߳�������*/
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
  * Description:    ��ȡ�߳����ȼ�������������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *scope--�߳����ȼ������� 
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
  * Description:    �����߳����ȼ�������������� 
  * Input:          *attr---�߳����Խṹ 
  					scope---�߳����ȼ�������
					PTHREAD_SCOPE_SYSTEM:ϵͳ��Χ������CPU
					PTHREAD_SCOPE_PROCESS:ͬһ���̷�Χ������CPU(linux pthread ��֧��!!!)
  * Output:         *attr---�߳����Խṹ
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
  * Description:    ��ȡ�߳�ջ��С�������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *stacksize--�߳�ջ��С(�ֽ���)
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
  * Description:    �����߳�ջ��С�������� 
  * Input:          *attr---�߳����Խṹ 
  					stacksize---�߳�ջ��С(�ֽ���)
					�߳�ջ��СΪPTHREAD_STACK_MIN=16k
  * Output:         *attr---�߳����Խṹ
  * Return:         0/errno 
*************************************************/
int Pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
	int rval;
	
	if(attr==AII_NULL)
	{
		return -1;	
	}

	/*����û����õ��߳�ջ�ĺϷ���*/
	if (stacksize < PTHREAD_STACK_MIN)
	{
		stacksize = PTHREAD_STACK_MIN;
	}

	/*�����߳�ջ����*/
	rval = pthread_attr_setstacksize(attr, stacksize);
	if (rval != 0)
	{
		debug_info(DEBUG_LEVEL_4,"Pthread_attr_setstacksize() failed!\n");
	}
	
	return rval;
}

/*************************************************
  * Function:		Pthread_attr_getstackaddr()
  * Description:    ��ȡ�߳�ջ��͵�ַ�������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *stackaddr--�߳�ջ��͵�ַ
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
  * Description:    �����߳�ջ����͵�ַ��������(Ӧ��ϣ���Լ������߳�ջ�ķ�������)
  * Input:          *attr---�߳����Խṹ 
  					stackaddr---�߳�ջ�ڴ浥Ԫ��͵�ַ
  * Output:         *attr---�߳����Խṹ
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
  * Description:    ��ȡ�߳�ջ���仺������С�������� 
  * Input:          *attr---�߳����Խṹ 
  * Output:         *guardsize--�߳�ջ���仺������С
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
  * Description:    �����߳�ջ�ľ��仺������С��������
  * Input:          *attr---�߳����Խṹ 
  					guardsize---�߳�ջ���仺������С
  * Output:         *attr---�߳����Խṹ
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
  * Description:    �����߳����԰������� 
  *					Ĭ���߳����ԣ�
  *					1.���̳д����̵߳ĵ��Ȳ��ԺͲ���,ʹ�����õĲ���
  *					2.ϵͳ��Χ������CPU
  *					3.�̲߳���SCHED_RR�����㷨
  *					4.�̷߳�������
  *					5.�߳����ȼ�:�ڵ��Ȳ��Ե����ȼ�minPriority~maxPriority��Χ��
  *					6.�߳�ջ��С���ԣ���СΪ16k
  * Input:          *attr---�߳����Խṹ
  *					priority---�߳����ȼ�
  *					stacksize---�߳�ջ��С 
  * Output:         *attr---�߳����Խṹ
  * Return:         0/error 
*************************************************/
int Pthread_attr_set(pthread_attr_t *attr, int priority, size_t stacksize)
{
	int rval;
	
	if(attr == AII_NULL)
	{
		return -1;	
	}

	/*��ʼ���߳����Խṹ*/
	rval = Pthread_attr_init(attr);
	if (rval != 0)
	{
		return rval;
	}

	/*���̳д����̵߳ĵ��Ȳ��ԺͲ���*/
	rval = Pthread_attr_setinheritsched(attr, PTHREAD_EXPLICIT_SCHED);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	/*ϵͳ��Χ������CPU*/
	rval = Pthread_attr_setscope(attr, PTHREAD_SCOPE_SYSTEM);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	/*�̲߳���SCHED_RR�����㷨*/
	rval = Pthread_attr_setschedpolicy(attr, SCHED_RR);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	/*�����̷߳�������:�߳̽�������ϵͳ�Զ�������Դ*/
	rval = Pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	/*�����߳����ȼ�����*/
	rval = Pthread_attr_setschedpriority(attr, priority);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	/*�����߳�ջ��С����*/
	rval = Pthread_attr_setstacksize(attr, stacksize);
	if (rval != 0)
	{
		Pthread_attr_destroy(attr);
		return rval;
	}

	return 0;
}

#endif
