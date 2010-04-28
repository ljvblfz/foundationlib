/************************************************************************/
/*	Copyright to AI INFINITY, Inc.
	Solution designed by Ben Shaw.
	Programmed by Ben Shaw.
	$Reversion 1.0 $ $Date: 2007/1/19 $ 
	$Reversion 1.1 $ $Date: 2009/12/31 $
*/
/************************************************************************/
#ifndef _OSI_HEAD_FILE
#define _OSI_HEAD_FILE

#include "libtype.h"

typedef void*	OSI_LIB_HANDLE;
/*
*	MicroGUI Message Queue ID Data Structure
*/

#define OSI_SEM_Q_PRIORITY				(0x1)
#define OSI_SEM_Q_FIFO					(0x0)
#define OSI_SEM_DELETE_SAFE				(0x4)
#define OSI_SEM_INVERSION_SAFE			(0x8)
#define OSI_SEM_EVENTSEND_ERR_NOTIFY	(0x10)
#define OSI_WAIT_FOREVER				(-1)

#define OSI_MSG_Q_FIFO					(0x00)
#define OSI_MSG_Q_PRIORITY				(0x01)
#define OSI_MSG_Q_EVENTSEND_ERR_NOTIFY	(0x02) 
#define OSI_NO_WAIT						(0) 
#define OSI_MSG_PRI_NORMAL				(0)
#define OSI_MSG_PRI_URGENT				(1)

#ifdef WINDOWS_OS
#include <Windows.h>
typedef HANDLE WIN_HANDLE;
typedef struct  
{
	WIN_HANDLE		ReadEnableHandle;
	WIN_HANDLE		WriteEnableHandle;
	DWORD			ReadTimeout;		/* Read wait Time-out interval, in milliseconds.  */
	DWORD			WriteTimeout;		/* Write wait Time-out interval, in milliseconds.  */

	uint32_t			HeadIndex;
	uint32_t			RearIndex;
	uint32_t			MsgLength;
	void*			MsgBuffer;
}MSG_HANDLE_ENTRY,*MSG_HANDLE_ENTRY_PTR;

/*
 *	Semaphore options
 */
#endif

#ifdef VXWORKS_OS
#include <msgQLib.h>
#include <semLib.h>
#include <taskLib.h>
#endif

#ifdef LINUX_OS
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#endif

OSI_LIB_HANDLE msg_queue_create(
								int MaxMSGs,			/* max messages that can be queued */
								int MaxMsgLength,	/* max bytes in a message */
								int options
							   );

int msg_send(
			OSI_LIB_HANDLE	MsgQHandle,		/* message queue handle on which to send */
			uint8_t*			buffer,			/* message buffer */
			uint32_t			MsgLength,		/* length of message */
			DWORD			timeout,		/* time to wait in milliseconds */
			int				priority		/* non useful in Windows platform */
			);

int msg_receive(
				OSI_LIB_HANDLE	MsgQHandle,	/* message queue handle on which to send */
				uint8_t*			buffer,		/* message buffer */
				uint32_t			MsgLength,	/* length of message */
				DWORD			timeout		/* time to wait in milliseconds */
			   );

void msg_queue_delete(
					  OSI_LIB_HANDLE	MsgQHandle		/* message queue to delete */
				     );

uint32_t msg_queue_contain(OSI_LIB_HANDLE MgMsgHandle);

uint32_t msg_queue_capacity(OSI_LIB_HANDLE MgMsgHandle);

/*
*	Use critical section
*/
OSI_LIB_HANDLE mutex_create(
							int option	/* non useful in windows platform */
						   );
void mutex_lock(
							OSI_LIB_HANDLE MgCriticalSectionHandle,
							DWORD timeout	/* non useful  */
							);
void mutex_unlock(
							OSI_LIB_HANDLE MgCriticalSectionHandle
							);
void mutex_delete(OSI_LIB_HANDLE MgCriticalSectionHandle);


/*
 *	For Windows, a task means one thread;
 *	For Linux, a task means one posix thread;
 *	For vxWorks, the process schedule element is a task which similar with 
 *	thread in Windows or Linux.
 */
OSI_LIB_HANDLE task_create(
							/*
							*	[out] For windows, TaskID will return the pointer
							*	to a variable that receives the thread 
							*	identifier. If this parameter is NULL, 
							*	the thread identifier is not returned. 
							*	[in] For vxWorks, TaskID implemented as task name
							*/
							void*	TaskId,
							int		Priority,	/* Task priority */
							/*
							*	For windows, Options implemented as dwCreationFlags.
							*	[in] Flags that control the creation of the thread. 
							*	If the CREATE_SUSPENDED flag is specified, 
							*	the thread is created in a suspended state, 
							*	and will not run until the ResumeThread function 
							*	is called. If this value is zero, the thread runs 
							*	immediately after creation. If the STACK_SIZE_PARAM_IS_A_RESERVATION 
							*	flag is specified, the dwStackSize parameter 
							*	specifies the initial reserve size of the stack. 
							*	Otherwise, dwStackSize specifies the commit size.
							*	For vxWorks, Options implemented as task option word  
							*/
							int		Options,	
							int		StackSize,	/* Stack size in bytes */
							void*	FcnPtr,		/* Application function start address */
							void*	Parameters	/* Point to parameters */
							);
/* This is the task safe delete */
int task_delete(
				 OSI_LIB_HANDLE MgTaskHandle
				 );
/* This function will delete a task in force mode */
/*
*	For Windows Platform
*	mgTaskDeleteForce is a dangerous function that should only be used in 
*	the most extreme cases. You should call mgTaskDeleteForce only if you 
*	know exactly what the target thread is doing, and you control all of 
*	the code that the target thread could possibly be running at the time 
*	of the termination.
*	For vxWorks Platform
*	This routine is intended as a debugging aid, and is generally 
*	inappropriate for applications. Disregarding a task's deletion 
*	protection could leave the the system in an unstable state or lead
*	to system deadlock.
*/
int task_delete_force(OSI_LIB_HANDLE MgTaskHandle);


void task_sleep(
				 DWORD dwtime	/* 
								 *	For windows platform in milliseconds; 
								 *	For Linux platform in milliseconds;
								 *	For vxWorks platform in ticks.
								 */
				);
#endif

