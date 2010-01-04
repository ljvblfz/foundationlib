/*
*	osi.c
*	OS Interface C code file.
*	
*	Foundation Services, Tool Module.
*	Based on SDK5.RC16, Linux 2.6.20, ARM Host CPU.
*	
*	MG3500 (MAXIM, mobilygen, Inc.) HD project
*  Copyright (c) 1992-2009,Zhejiang Dahua Technology Stock Co. Ltd.
*
*	2009.07.30 Solution designed by shaobin, Programmed by shaobin.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osi.h"

#ifdef VXWORKS_OS
#include <semLib.h>
#endif

#ifdef LINUX_OS
#include <pthread.h>
#endif



mutex_t mutex_create(void)
{
#ifdef VXWORKS_OS
	SEM_ID vx_mutex;
	
	vx_mutex =semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);
	if (vx_mutex==NULL)
	{
		return ERROR;
	}

	return (mutex_t) vx_mutex;
#endif

#ifdef LINUX_OS
	pthread_mutex_t* posix_mutex;
	
	posix_mutex=malloc(sizeof(pthread_mutex_t));
	if (posix_mutex==NULL)
	{
		return ERROR;
	}

	return (mutex_t) posix_mutex;
#endif
}

int mutex_init(mutex_t mutex, int opt)
{
#ifdef VXWORKS_OS
	/* nothing to do */

#endif

#ifdef LINUX_OS
	if (pthread_mutex_init((pthread_mutex_t*)mutex,(pthread_mutexattr_t*) opt)!=OK)
	{
		return ERROR;
	}

#endif
	
	return OK;
}

int mutex_lock(mutex_t mutex)
{
	int ret;

#ifdef VXWORKS_OS
	ret=semTake((SEM_ID)mutex, WAIT_FOREVER);
#endif
#ifdef LINUX_OS
	ret=pthread_mutex_lock((pthread_mutex_t*)mutex);
#endif

	return ret;
}

int mutex_unlock(mutex_t mutex)
{
	int ret;

#ifdef VXWORKS_OS
	ret=semGive((SEM_ID)mutex, WAIT_FOREVER);
#endif

#ifdef LINUX_OS
	ret=pthread_mutex_unlock((pthread_mutex_t*)mutex);
#endif

	return ret;
}

