/************************************************************************/
/*	Copyright to AI INFINITY, Inc.
Solution designed by Ben Shaw.
Programmed by Ben Shaw.

$Reversion 1.0 $ $Date: 2007/01/19 $ 
$Reversion 1.1 $ $Date: 2009/11/18 $
*/
/************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "mgosi.h"

/************************************************************************/
/* MicroGUI Message Queue                                               */
/************************************************************************/

OSI_LIB_HANDLE msg_queue_create(
						   int MaxMSGs,			/* max messages that can be queued */
						   int MaxMsgLength,	/* max bytes in a message */
						   int options			/* Non useful */
						   )
{

#ifdef WINDOWS_OS
	MSG_HANDLE_ENTRY_PTR MsgHandleEntryPtr;

	/* Create message handle entry */
	if ((MsgHandleEntryPtr=(MSG_HANDLE_ENTRY_PTR)
		malloc(sizeof(MSG_HANDLE_ENTRY)))==NULL)
	{
		goto err;
	}
	/* Create message buffer readable event */
	if ((MsgHandleEntryPtr->ReadEnableHandle=CreateEvent(
		NULL,
		TRUE,	/* Manually settable */
		FALSE,	/* Initialized to non-signaled */
		NULL	/* No name */
		))==NULL)
	{
		printf("CreateEvent failed (%d)\n", GetLastError());
		free(MsgHandleEntryPtr);
		return NULL;
	}
	/* Create message buffer writable event */
	if ((MsgHandleEntryPtr->WriteEnableHandle=CreateEvent(
		NULL,	// default security attributes
		TRUE,	//	Manually settable
		TRUE,	// Initialized to signaled
		NULL	// No name
		))==NULL)
	{
		printf("CreateEvent failed (%d)\n", GetLastError());
		free(MsgHandleEntryPtr);
		return NULL;
	}
	MsgHandleEntryPtr->ReadTimeout=INFINITE;
	MsgHandleEntryPtr->WriteTimeout=0;
	MsgHandleEntryPtr->MsgLength=MaxMsgLength*MaxMSGs+1;
	MsgHandleEntryPtr->HeadIndex=0;
	MsgHandleEntryPtr->RearIndex=0;
	/* Allocate memory space to store message */
	if ((MsgHandleEntryPtr->MsgBuffer=
		malloc(MsgHandleEntryPtr->MsgLength))==NULL)	
	{
		goto err;
	}
	return (OSI_LIB_HANDLE)MsgHandleEntryPtr;
err:
	printf("Memory limited!\n");
	return NULL;
#endif

#ifdef VXWORKS_OS
	return (OSI_LIB_HANDLE) msgQCreate(MaxMSGs,MaxMsgLength,options);
#endif

#ifdef LINUX_OS
	printf("This function is not yet available Linux!\n");
	return ERROR;
#endif
}


LOGICAL mgIsQEmpty(OSI_LIB_HANDLE MgMsgHandle)
{
#ifdef WINDOWS_OS
	MSG_HANDLE_ENTRY_PTR MgMsgHandleEntryPtr;

	MgMsgHandleEntryPtr=(MSG_HANDLE_ENTRY_PTR)MgMsgHandle;
	return MgMsgHandleEntryPtr->HeadIndex==MgMsgHandleEntryPtr->RearIndex;
#endif

#ifdef VXWORKS_OS
	return msgQNumMsgs((MSG_Q_ID)MgMsgHandle)==0;
#endif
	
#ifdef LINUX_OS
	printf("This function is not yet available Linux!\n");
	return ERROR;
#endif

}


LOGICAL is_msg_queue_full(OSI_LIB_HANDLE MgMsgHandle)
{

#ifdef WINDOWS_OS
	MSG_HANDLE_ENTRY_PTR MgMsgHandleEntryPtr;
	UINT32 RearIndex,HeadIndex,MsgLength;

	MgMsgHandleEntryPtr=(MSG_HANDLE_ENTRY_PTR)MgMsgHandle;
	RearIndex=MgMsgHandleEntryPtr->RearIndex;
	HeadIndex=MgMsgHandleEntryPtr->HeadIndex;
	MsgLength=MgMsgHandleEntryPtr->MsgLength;
	return ((RearIndex+1)%MsgLength)==HeadIndex%MsgLength;
#endif
	
#ifdef VXWORKS_OS
	return OK;	/* No such function in vxWorks */
#endif
	
#ifdef LINUX_OS
	printf("This function is not yet available Linux!\n");
	return ERROR;
#endif
}


UINT32 msg_queue_contain(OSI_LIB_HANDLE MgMsgHandle)
{

#ifdef WINDOWS_OS
	MSG_HANDLE_ENTRY_PTR MgMsgHandleEntryPtr;
	UINT32 RearIndex,HeadIndex,MsgLength;

	MgMsgHandleEntryPtr=(MSG_HANDLE_ENTRY_PTR)MgMsgHandle;
	MsgLength=MgMsgHandleEntryPtr->MsgLength;
	RearIndex=MgMsgHandleEntryPtr->RearIndex%MsgLength;
	HeadIndex=MgMsgHandleEntryPtr->HeadIndex%MsgLength;
	if (RearIndex>=HeadIndex)
	{
		return RearIndex-HeadIndex;
	}
	else
	{
		return MsgLength-(HeadIndex-RearIndex);
	}
#endif
	
#ifdef VXWORKS_OS
	return msgQNumMsgs((MSG_Q_ID)MgMsgHandle);
#endif

#ifdef LINUX_OS
	printf("This function is not yet available Linux!\n");
	return ERROR;
#endif

}


UINT32 msg_queue_capacity(OSI_LIB_HANDLE MgMsgHandle)
{
#ifdef WINDOWS_OS
	MSG_HANDLE_ENTRY_PTR MgMsgHandleEntryPtr;
	UINT32 RearIndex,HeadIndex,MsgLength;

	MgMsgHandleEntryPtr=(MSG_HANDLE_ENTRY_PTR)MgMsgHandle;
	MsgLength=MgMsgHandleEntryPtr->MsgLength;
	RearIndex=MgMsgHandleEntryPtr->RearIndex%MsgLength;
	HeadIndex=MgMsgHandleEntryPtr->HeadIndex%MsgLength;

	if (RearIndex>=HeadIndex)
	{
		return MsgLength-1-(RearIndex-HeadIndex);
	}
	else
	{
		return (HeadIndex-RearIndex)-1;
	}
#endif

#ifdef VXWORKS_OS
	printf("This function is not yet available Linux!\n");
	return ERROR;
#endif

#ifdef LINUX_OS
	printf("This function is not yet available Linux!\n");
	return ERROR;
#endif
	
}



int msg_send(
			   OSI_LIB_HANDLE	MsgQHandle,		/* message queue handle on which to send */
			   UINT8*			buffer,			/* message buffer to send */
			   UINT32			MsgLength,		/* length of message */
			   DWORD			timeout,			/* time to wait */
			   int				priority		/* non useful in Windows platform */
			   )
{
#ifdef WINDOWS_OS
	MSG_HANDLE_ENTRY_PTR MgMsgHandleEntryPtr;
	UINT8*					msgbuffer;
	UINT32					i=0;
	DWORD					WaitReturn;
	UINT32					MsgMaxLen,capacity;

	if (MsgQHandle==NULL || buffer==NULL)
	{
		return ERROR;
	}

	MgMsgHandleEntryPtr=(MSG_HANDLE_ENTRY_PTR)MsgQHandle;
	MgMsgHandleEntryPtr->WriteTimeout=timeout;
	msgbuffer=(UINT8*)MgMsgHandleEntryPtr->MsgBuffer;
	MsgMaxLen=MgMsgHandleEntryPtr->MsgLength;

	while (MsgLength)
	{
		if (msg_queue_capacity(MsgQHandle)<MsgLength)
		{
			/* Set readable event handle to be signaled */
			SetEvent(MgMsgHandleEntryPtr->ReadEnableHandle);
			/* Set writable event handle to be non-signaled */
			ResetEvent(MgMsgHandleEntryPtr->WriteEnableHandle);
			/* Waite for writable event handle */
			WaitReturn=WaitForSingleObject(MgMsgHandleEntryPtr->WriteEnableHandle
				,MgMsgHandleEntryPtr->WriteTimeout);
			switch(WaitReturn)
			{
				/*
				*	The specified object is a mutex object that was not 
				*	released by the thread that owned the mutex object 
				*	before the owning thread terminated. Ownership of the 
				*	mutex object is granted to the calling thread, and the 
				*	mutex is set to nonsignaled.
				*/
			case WAIT_ABANDONED:	
				return ERROR;
				break;
				/*
				*	The state of the specified object is signaled.
				*/
			case WAIT_OBJECT_0:
				break;
				/*
				*	The time-out interval elapsed, and the object's state 
				*	is nonsignaled.
				*/
			case WAIT_TIMEOUT:
				return ERROR;
				break;
			default:
				break;
			}
		}
		else	/* Enough space is ready */
		{
			MgMsgHandleEntryPtr->RearIndex%=MsgMaxLen;
			/* Insert a item to the rear of  message queue */
			msgbuffer[MgMsgHandleEntryPtr->RearIndex++]=buffer[i++];
			MsgLength--;
			/* Set readable event handle to be signaled */
			SetEvent(MgMsgHandleEntryPtr->ReadEnableHandle);
		}
	}


	return OK;
#endif
	
#ifdef VXWORKS_OS
	int status;

	status=msgQSend(
		(MSG_Q_ID)MsgQHandle,
		buffer,
		MsgLength,		
		timeout,
		priority
		);
	return status;
#endif

#ifdef LINUX_OS
	printf("This function is not yet available Linux!\n");
	return ERROR;
#endif
}


int msg_receive(
				  OSI_LIB_HANDLE		MsgQHandle,	/* message queue handle on which to send */
				  UINT8*			buffer,		/* message buffer to receive */
				  UINT32			MsgLength,	/* length of message */
				  DWORD				timeout		/* time to wait in milliseconds */
				  )
{

#ifdef WINDOWS_OS
	MSG_HANDLE_ENTRY_PTR MgMsgHandleEntryPtr;
	UINT8*					msgbuffer;
	UINT32					i=0;
	DWORD					ReadReturn;
	UINT32					MsgQLen;

	if (MsgQHandle==NULL || buffer==NULL)
	{
		return ERROR;
	}
	MgMsgHandleEntryPtr=(MSG_HANDLE_ENTRY_PTR)MsgQHandle;
	MgMsgHandleEntryPtr->ReadTimeout=timeout;
	msgbuffer=(UINT8*)MgMsgHandleEntryPtr->MsgBuffer;
	MsgQLen=MgMsgHandleEntryPtr->MsgLength;
	while (MsgLength)
	{
		if (msg_queue_contain(MsgQHandle)<MsgLength)
		{
			/* Set writable event handle to be signaled */
			SetEvent(MgMsgHandleEntryPtr->WriteEnableHandle);
			/* Set readable event handle to be non-signaled */
			ResetEvent(MgMsgHandleEntryPtr->ReadEnableHandle);
			ReadReturn=WaitForSingleObject(MgMsgHandleEntryPtr->ReadEnableHandle
				,MgMsgHandleEntryPtr->ReadTimeout);
			switch(ReadReturn)
			{
				/*
				*	The specified object is a mutex object that was not 
				*	released by the thread that owned the mutex object 
				*	before the owning thread terminated. Ownership of the 
				*	mutex object is granted to the calling thread, and the 
				*	mutex is set to nonsignaled.
				*/
			case WAIT_ABANDONED:	
				return ERROR;
				break;
				/*
				*	The state of the specified object is signaled.
				*/
			case WAIT_OBJECT_0:
				break;
				/*
				*	The time-out interval elapsed, and the object's state 
				*	is nonsignaled.
				*/
			case WAIT_TIMEOUT:
				return ERROR;
				break;
			default:
				break;
			}

		}
		else
		{
			MgMsgHandleEntryPtr->HeadIndex%=MsgQLen;
			/* Read a item from the head of buffer */
			buffer[i++]=msgbuffer[MgMsgHandleEntryPtr->HeadIndex];
			MgMsgHandleEntryPtr->HeadIndex++;
			MsgLength--;
			/* Set writable event handle to be signaled */
			SetEvent(MgMsgHandleEntryPtr->WriteEnableHandle);
		}
	}

	return OK;
#endif
	
#ifdef VXWORKS_OS
	int status;

	status=msgQReceive(
		(MSG_Q_ID)MsgQHandle,
		buffer,
		MsgLength,	
		timeout
		);
	return status;
#endif

#ifdef LINUX_OS
	printf("This function is not yet available Linux!\n");
	return ERROR;
#endif
}




/************************************************************************/
/* Semaphore                                                   */
/************************************************************************/
/*
 *	Use critical section
 */
OSI_LIB_HANDLE mutex_create(
								int option	/* non useful in windows platform */
								)
{
#ifdef WINDOWS_OS
	LPCRITICAL_SECTION MgCriticalSectionHandle;

	if ((MgCriticalSectionHandle=(LPCRITICAL_SECTION)
		malloc(sizeof(CRITICAL_SECTION)))==NULL)
	{
		printf("Memory limited!\n");
		return NULL;
	}
	InitializeCriticalSection(MgCriticalSectionHandle);

	return (OSI_LIB_HANDLE)MgCriticalSectionHandle;
#endif

#ifdef VXWORKS_OS
	return (OSI_LIB_HANDLE)semMCreate(option);
#endif

#ifdef LINUX_OS
	pthread_mutex_t* posix_mutex;
	mutex_t mutex;

	posix_mutex=malloc(sizeof(pthread_mutex_t));
	if (posix_mutex==NULL)
	{
		return ERROR;
	}
	
	mutex=(mutex_t)posix_mutex;

	if (pthread_mutex_init((pthread_mutex_t*)mutex,(pthread_mutexattr_t*) option)!=OK)
	{
		return ERROR;
	}
	
	return (OSI_LIB_HANDLE)mutex;
#endif

}
void mutex_lock(
				OSI_LIB_HANDLE mutex_handle,
				DWORD timeout	/* non useful */
			   )
{
#ifdef WINDOWS_OS
	EnterCriticalSection((LPCRITICAL_SECTION)mutex_handle);
#endif

#ifdef VXWORKS_OS
	semTake((SEM_ID)mutex_handle,timeout);
#endif
	
#ifdef LINUX_OS
	pthread_mutex_lock((pthread_mutex_t*)mutex_handle);
#endif

}

void mutex_unlock(OSI_LIB_HANDLE mutex_handle)
{
#ifdef WINDOWS_OS
	LeaveCriticalSection((LPCRITICAL_SECTION)mutex_handle);
#endif

#ifdef VXWORKS_OS
	semGive((SEM_ID)mutex_handle);
#endif

#ifdef LINUX_OS
	pthread_mutex_unlock((pthread_mutex_t*)mutex_handle);
#endif
}


void mutex_delete(OSI_LIB_HANDLE mutex_handle)
{
#ifdef WINDOWS_OS
	DeleteCriticalSection((LPCRITICAL_SECTION)mutex_handle);
#endif

#ifdef VXWORKS_OS
	semDelete((SEM_ID)mutex_handle);
#endif

#ifdef LINUX_OS
	pthread_mutex_destroy((pthread_mutex_t*)mutex_handle);
#endif

}



/************************************************************************/
/* MicroGUI Task		                                                */
/************************************************************************/

/*
 *	For windows, MicroGUI is a single process which include multi-thread,
 *	so we use thread to implement MicroGUI task in windows platform.
 *	For vxWorks, MicroGUI is a multi-task system.
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
							)
{

#ifdef WINDOWS_OS
	WIN_HANDLE WinThreadHandle;

	WinThreadHandle=CreateThread(
		NULL,	/* The handle cannot be inherited */
		StackSize,
		(LPTHREAD_START_ROUTINE)FcnPtr,
		Parameters,
		Options,
		NULL
		);
	SetThreadPriority(WinThreadHandle,Priority);
	return (OSI_LIB_HANDLE)WinThreadHandle;
#endif
	
#ifdef VXWORKS_OS
	int argi,vxWorksTaskID;

	argi=(int)Parameters;
	vxWorksTaskID=taskSpawn(
		(char*)TaskId,
		Priority,
		Options,
		StackSize,
		FcnPtr,	
		argi,0,0,0,0,0,0,0,0,0
		);
	return vxWorksTaskID;
#endif

#ifdef LINUX_OS

#endif

}

/* This is the task safe delete */
int task_delete(
					 OSI_LIB_HANDLE MgTaskHandle
					 )
{
#ifdef WINDOWS_OS
	DWORD ExitCode;
	WIN_HANDLE WinThreadHandle;

	WinThreadHandle=(WIN_HANDLE)MgTaskHandle;
	if (GetExitCodeThread(WinThreadHandle,&ExitCode)==0)
	{
		return ERROR;
	}
	ExitThread(ExitCode);
	return OK;
#endif

#ifdef VXWORKS_OS
	int status;

	status=taskDelete(MgTaskHandle);
	return status;
#endif

#ifdef LINUX_OS
	
#endif
	
}

/* This function will delete a MicroGUI task in force mode */
/*
 *	mgTaskDeleteForce is a dangerous function that should only be used in 
 *	the most extreme cases. You should call mgTaskDeleteForce only if you 
 *	know exactly what the target thread is doing, and you control all of 
 *	the code that the target thread could possibly be running at the time 
 *	of the termination.
 *	
 */
int task_delete_force(OSI_LIB_HANDLE MgTaskHandle)
{
#ifdef WINDOWS_OS
	DWORD ExitCode;
	WIN_HANDLE WinThreadHandle;

	WinThreadHandle=(WIN_HANDLE)MgTaskHandle;
	if (GetExitCodeThread(WinThreadHandle,&ExitCode)==0)
	{
		return ERROR;
	}
	if (TerminateThread(WinThreadHandle,ExitCode)==0)
	{
		return ERROR;
	}
	return OK;
#endif

#ifdef VXWORKS_OS
	int status;

	status=taskDeleteForce(MgTaskHandle);
	return status;
#endif
	
#ifdef LINUX_OS

#endif

}

void task_sleep(
				 DWORD dwtime	/* 
								 *	For windows platform in milliseconds, 
								 *	For vxWorks platform in ticks.
								 */
				 )
{

#ifdef WINDOWS_OS
	Sleep(dwtime);
#endif

#ifdef VXWORKS_OS
	taskDelay(dwtime);
#endif

#ifdef LINUX_OS

#endif

}



