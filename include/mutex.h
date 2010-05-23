/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: mutex.h 
 *  Description:
 *  This library contains mutex API cross platform: linux, vxWorks, Windows.
 *  
 *  Author: Ben Shaw
 *  Create:2010-05-21
 *  
 *  Modification history:
 *  2010-05-21, created the file,           Ben Shaw
 *
 *  
 **************************************************************************************/

#ifndef _MUTEX_API_H
#define _MUTEX_API_H

#include "libtype.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void* MUTEX_HANDLE;

#ifndef WAIT_FOREVER
#define WAIT_FOREVER (-1)
#endif

/*
 *	Description:
 *  Create a mutex handle.
 *  
 *	Platform scope:
 *	Linux: supported.
 *  vxWorks: supported. This function creates a mutex semaphore. The mutual-exclusion semaphore 
 *  is a specialized version of the binary semaphore, designed to address issues 
 *  inherent in mutual exclusion, such as recursive access to resources, priority 
 *  inversion, and deletion safety. The fundamental behavior of the mutual-exclusion 
 *  semaphore is identical to the binary semaphore except for the following restrictions:
 *  - It can only be used for mutual exclusion.
 *  - It can only be given by the task that took it.
 *  - It may not be taken or given from interrupt level.
 *  win32: supported. This function creates a mutex handle in win32 platform.
 *  
 *	Parameters:
 *	none.
 *  
 *	Return value:
 *	If the function succeeds, the return value is a handle of mutex.
 *  Otherwise, it return AII_NULL.
 */
MUTEX_HANDLE mutex_create(void);

/*
 *	Description:
 *  This function delete a mutex handle.
 *  
 *	Platform scope:
 *	Linux: Support.
 *  vxWorks: Support.
 *  win32: Support.
 *  
 *	Parameters:
 *	MUTEX_HANDLE mutex_handle [input] (valid mutex handle) mutex hande.
 *  
 *	Return value:
 *	If the function succeeds, it return AII_OK.
 *  Otherwise, it return AII_ERROR.
 *  
 */
INT32_T mutex_delete(MUTEX_HANDLE mutex_handle);

/*
 *	Description:
 *  This function locks the mutex semaphore for mutual exclusion access to 
 *  critical section. It can lock the mutex semaphore for infinite long time
 *  if the millisec = WAITFOREVER or specific finite long time in millisecond unit.
 *  
 *	Platform scope:
 *	Linux: support.
 *  vxWorks: support.
 *  win32: support.
 *  
 *	Parameters:
 *	MUTEX_HANDLE mutex_handle [input] (valid mutex handle) mutex handle.
 *  INT32_T millisec [input] (WAITFOREVER, >=0) lock duration.
 *  
 *	Return value:
 *	If the function succeeds, it returns AII_OK.
 *  Otherwise, it returns AII_ERROR.
 *  
 */
INT32_T mutex_lock(MUTEX_HANDLE mutex_handle, INT32_T millisec);

/*
 *	Description:
 *  This function unlock a mutex semaphore.
 *  
 *	Platform scope:
 *	Linux: support.
 *  vxWorks: support. Note: It can only be unlocked by the thread that took it.
 *  win32: support.
 *  
 *	Parameters:
 *	MUTEX_HANDLE mutex_handle [input] (valid mutex handle) mutex handle.
 *  
 *	Return value:
 *	If the function succeeds, it returns AII_OK.
 *  Otherwise it returns AII_ERROR.
 *  
 */
INT32_T mutex_unlock(MUTEX_HANDLE mutex_handle);


#ifdef __cplusplus
}
#endif

#endif

