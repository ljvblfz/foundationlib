/************************************************************************/
/*	
	Copyright to AI INFINITY, Inc.
	Solution designed by shaobin.
	Programmed by shaobin, wangxian.chen.

	$ Reversion 1.0 $ $Data: 2007/09/07 $
	$ Reversion 1.1 $ $Data: 2007/09/30 $
	$ Reversion 1.2 $ $Date: 2009/11/18 $
*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mgosi.h"
#include "list.h"
#include "tool.h"

typedef struct  
{
	int		HandleType;
	void*   EntryPtr;
}DS_HANDLE_ENTRY,*DS_HANDLE_ENTRY_PTR;

typedef struct  
{
	OSI_LIB_HANDLE	MutexHandle;
	int				CmpFcnAmount;
	void**			CmpFcnArray;
	unsigned int	EntryAmount;
	void*			EntriesList;
}LIST_HEAD,*LIST_HEAD_PTR;

typedef struct _Node 
{
	int				length;
	void*			data;
	int				ReadPermission;
	int				WritePermission;
	LIST_HEAD_PTR	HeadPtr;
	struct _Node*	PreviousNode;
	struct _Node*	NextNode;
}LIST_NODE,*LIST_NODE_PTR;

/*
 *	Description: 
 *  Function "CreateHandle" creates a list handle according to handle type which is 
 *  LIST_HEAD_TYPE or LIST_NODE_TYPE. This handle is a representative of the
 *  list head or a node.
 *  
 *  Parameters:
 *  1. integer HandleType [input] (LIST_HEAD_TYPE, LIST_NODE_TYPE)
 *  Return:
 *  handle pointer, a valid pointer or NULL when memory limits
 */
LIST_HANDLE CreateHandle(int HandleType)
{
	DS_HANDLE_ENTRY_PTR HandleEntryPtr;

	if ((HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)
		malloc(sizeof(DS_HANDLE_ENTRY)))==LIST_NULL)
	{
		return LIST_NULL;
	}
	memset(HandleEntryPtr,0,sizeof(DS_HANDLE_ENTRY));
	HandleEntryPtr->HandleType=HandleType;
	return (LIST_HANDLE)HandleEntryPtr;
}

/*
 *	Function "DeleteHandle" deletes a handle. Note: It just deletes the handle descriptor.
 *  It does not delete the member data which the handle represents. 
 *  
 *  Parameters:
 *  1. LIST_HANDLE handle [input] (valid pointer value)
 */
void DeleteHandle(LIST_HANDLE handle)
{
    DS_HANDLE_ENTRY_PTR HandleEntryPtr;

	if (handle != LIST_NULL)
	{
        free(handle);
        handle = LIST_NULL;
    }

    return;
}

/*
 *	Description:
 *	Function "Initialize" initializes all the members of the head structure. 
 *  It creates a comparison function table.
 *		
 *	Parameters:
 *	1. int CmpFcnAmount [input] (>0) Comparison function amount
 *  2. void** CmpFcnArray [input] (valid pointer) a pointer points to a function array 
 *      which also contains CmpFcnAmount pointers point to each comparison function.
 *	Return:
 *	Function Execution Status, (LIST_OK, LIST_ERROR)
 */
int Initialize(
				int		CmpFcnAmount,
				void**		CmpFcnArray,
				LIST_HANDLE  handle
			   )
{
	LIST_HEAD_PTR	ListHeadPtr;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;
    int         opt;

	ListHeadPtr = LIST_NULL;
    DSHandleEntryPtr = LIST_NULL;

	if ((handle == LIST_NULL)||(CmpFcnAmount == 0))
	{
		return LIST_ERROR;
	}
	/* Allocate list head */
	if ((ListHeadPtr=(LIST_HEAD_PTR)malloc(sizeof(LIST_HEAD)))==0)
	{
		return LIST_ERROR;
	}
	memset((void *)ListHeadPtr, 0, sizeof(LIST_HEAD));

	/* Allocate compare callback function array */
	ListHeadPtr->CmpFcnAmount=CmpFcnAmount;
	if ((ListHeadPtr->CmpFcnArray=(void**)
		malloc(CmpFcnAmount*sizeof(void*)))==LIST_NULL)
	{
		if (ListHeadPtr != LIST_NULL)
		{
            free(ListHeadPtr);
            ListHeadPtr = LIST_NULL;
		}
        return LIST_ERROR;
	}

	memcpy(ListHeadPtr->CmpFcnArray,CmpFcnArray,CmpFcnAmount*sizeof(void*));
	ListHeadPtr->EntryAmount=0;
	ListHeadPtr->EntriesList=LIST_NULL;
    opt=MG_SEM_Q_PRIORITY | MG_SEM_DELETE_SAFE | MG_SEM_INVERSION_SAFE;
	ListHeadPtr->MutexHandle=mutex_create(opt);

	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
	DSHandleEntryPtr->EntryPtr=(void*)ListHeadPtr;
	DSHandleEntryPtr->HandleType=LIST_HEAD_TYPE;
	return LIST_OK;
}

/*
 *	Description:
 *	Function "Count" returns the entry amount of list.
 *		
 *	Parameters:
 *	1. LIST_HANDLE handle [input] (valid pointer), the handle of list head
 *	Return:
 *	Function Execution Status, (LIST_ERROR, LIST_OK).
 */
int Count(LIST_HANDLE handle)
{
	LIST_HEAD_PTR	ListHeadPtr;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;
	int amount;
	
    ListHeadPtr = LIST_NULL;
    DSHandleEntryPtr = LIST_NULL;
    amount = 0;

	if (IsHandleEmpty(handle)==True)
	{
		return LIST_ERROR;
	}

	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
	if (DSHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		printf("List class error: DS HeadHandle type error!\n");
		return LIST_ERROR;
	}
	ListHeadPtr=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;
    if (ListHeadPtr == LIST_NULL)
    {
        return LIST_ERROR;
    }

	mutex_lock(ListHeadPtr->MutexHandle, MG_WAIT_FOREVER);
	amount=ListHeadPtr->EntryAmount;
	mutex_unlock(ListHeadPtr->MutexHandle);
	
	return amount;
}

/*
 *	Description:
 *	Check whether a list is empty or not.
 *		
 *	Parameters:
 *	1. LIST_HANDLE HeadHandle [input] (valid pointer)
 *	Return:
 *	LIST_ERROR if an error occur. False if the list is not empty, True if 
 *	the list is empty. 
 */
int IsEmpty(LIST_HANDLE HeadHandle)
{
	LIST_HEAD_PTR list;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;
	LOGICAL logi;

    list = LIST_NULL;
    DSHandleEntryPtr = LIST_NULL;
    logi = False;

	if (IsHandleEmpty(HeadHandle)==True)
	{
		return LIST_ERROR;
	}

	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	if (DSHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		return LIST_ERROR;
	}

    if (DSHandleEntryPtr->EntryPtr)
    {
        list=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;
    }
    else
    {
        return LIST_ERROR;
    }

	mutex_lock(list->MutexHandle, MG_WAIT_FOREVER);
	logi=(list->EntryAmount==0)?True:False;
	mutex_unlock(list->MutexHandle);
	
	return logi;
}

/*
 *	Description:
 *	Find a specific node with single keyword match.	
 *	Parameters:
 *	1. LIST_HANDLE HeadHandle [input] (valid pointer), handle of list
 *	2. void** KeyWordPtrArray [input] (valid pointer), a pointer points to keyword pointer array
 *	3. int CmpFcnIndex [input] (none zero integer), compare keyword mark bits.
 *	4. LIST_HANDLE NodeHandle [output] (valid pointer), a handle for returning the matched node.
 *	5. int PermissionTag [input] (READ_PERMISSION_TAG, WRITE_PERMISSION_TAG), 
 *		set the access permission for obtained node.
 *	Return:
 *	LIST_ERROR: When a error occur
 *	LIST_OK: Successfully return a matched node with the desired access permission.
 *	PERMISSION_DENY: The matched node refuses to endow the expected permission, 
 *	for example, the matched node is being writing by another process when the current process
 *	want to read or write it, or the matched node is being read by another process when the current
 *	process want to write it.
 */
int FindSW(
		   LIST_HANDLE HeadHandle,
		   void** KeyWordPtrArray,
		   int CmpFcnIndex,
		   LIST_HANDLE NodeHandle,
		   int PermissionTag,
		   void* UserData
		   )
{
	LIST_NODE_PTR CurrentNodePtr;
	int (*CmpFcnPtr)(void*,void*);
	void** CmpFcnArray;
	LIST_HEAD_PTR list;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;
	DS_HANDLE_ENTRY_PTR DSNodeHandleEntryPtr;
	int logi,i,MaskTmp;

    CurrentNodePtr = LIST_NULL;
    CmpFcnArray = LIST_NULL;
    list = LIST_NULL;
    DSHandleEntryPtr = LIST_NULL;
    DSNodeHandleEntryPtr = LIST_NULL;
    logi=i=MaskTmp=0;

	if (IsEmpty(HeadHandle) || KeyWordPtrArray==LIST_NULL || NodeHandle==LIST_NULL)
	{
		return LIST_ERROR;
	}

	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	DSNodeHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	if (DSHandleEntryPtr->HandleType!=LIST_HEAD_TYPE || DSNodeHandleEntryPtr->HandleType!=LIST_NODE_TYPE)
	{
		printf("List class error: DS handle type error!\n");
		return LIST_ERROR;
	}

    list=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;
    
    if (list->CmpFcnArray)
    {
        CmpFcnArray=list->CmpFcnArray;
    } 
    else
    {
        return LIST_ERROR;
    }

	mutex_lock(list->MutexHandle, MG_WAIT_FOREVER);

    if (list->EntriesList)
    {
        CurrentNodePtr=list->EntriesList;
    } 
    else
    {
		mutex_unlock(list->MutexHandle);
        return LIST_ERROR;
    }
	
    /* Get a temporary read permission */
	CurrentNodePtr->ReadPermission++;
	mutex_unlock(list->MutexHandle);

	while (CurrentNodePtr)
	{
		MaskTmp=CmpFcnIndex;
		/* Search the first matched tag */
		for (i=0;i<list->CmpFcnAmount;i++)
		{
			if ((MaskTmp & 0x1) && KeyWordPtrArray[i])
			{
				CmpFcnPtr=CmpFcnArray[i];
				if (((*CmpFcnPtr)(CurrentNodePtr->data,KeyWordPtrArray[i]),UserData)==0)
				{
					mutex_lock(list->MutexHandle, MG_WAIT_FOREVER);
					CurrentNodePtr->ReadPermission--;	/* Release temporary permission */
					DSNodeHandleEntryPtr->HandleType=LIST_NODE_TYPE;
					DSNodeHandleEntryPtr->EntryPtr=(void*)CurrentNodePtr;
					/* Regain the permission */
					switch(PermissionTag)
					{
					case READ_PERMISSION_TAG:
						if (CurrentNodePtr->WritePermission>0)
						{
							mutex_unlock(list->MutexHandle);
							return PERMISSION_DENY;
						}

						CurrentNodePtr->ReadPermission++;
						break;
					case WRITE_PERMISSION_TAG:
						if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)
						{
							mutex_unlock(list->MutexHandle);
							return PERMISSION_DENY;
						}

						CurrentNodePtr->WritePermission++;
					    break;
					default:
					    break;
					}
					mutex_unlock(list->MutexHandle);
					return LIST_OK;
				}
			}
			MaskTmp>>=1;
		}

		mutex_lock(list->MutexHandle, MG_WAIT_FOREVER);
		/* Release temporary read permission of previous node */
		CurrentNodePtr->ReadPermission--;
		CurrentNodePtr=CurrentNodePtr->NextNode;
		/* Get temporary read permission of current node */
		if (CurrentNodePtr)
		{
			CurrentNodePtr->ReadPermission++;
		}
		mutex_unlock(list->MutexHandle);
	}

	return LIST_ERROR;
}

/*
 *	Description:
 *	Find a specific node with multiple keyword match.	
 *	Parameters:
 *	1. LIST_HANDLE HeadHandle [input] (valid pointer), handle of list
 *	2. void** KeyWordPtrArray [input] (valid pointer), a pointer points to keyword pointer array
 *	3. int CmpFcnIndex [input] (none zero integer), compare keyword mark bits.
 *	4. LIST_HANDLE NodeHandle [output] (valid pointer), a handle for returning the matched node.
 *	5. int PermissionTag [input] (READ_PERMISSION_TAG, WRITE_PERMISSION_TAG), 
 *		set the access permission for obtained node.
 *	Return:
 *	LIST_ERROR: When a error occur
 *	LIST_OK: Successfully return a matched node with the desired access permission.
 *	PERMISSION_DENY: The matched node refuses to endow the expected permission, 
 *	for example, the matched node is being writing by another process when the current process
 *	want to read or write it, or the matched node is being read by another process when the current
 *	process want to write it.
 */
int FindMW(
		   LIST_HANDLE HeadHandle,
		   void** KeyWordPtrArray,
		   int CmpFcnIndex,
		   LIST_HANDLE NodeHandle,
		   int PermissionTag,
		   void* UserData
		   )
{
	LIST_NODE_PTR CurrentNodePtr;
	int (*CmpFcnPtr)(void*,void*);
	void** CmpFcnArray;
	LIST_HEAD_PTR list;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;
	DS_HANDLE_ENTRY_PTR DSNodeHandleEntryPtr;
	int logi,i,MaskTmp,CompareCnt;

    CurrentNodePtr = LIST_NULL;
    CmpFcnArray = LIST_NULL;
    list = LIST_NULL;
    DSHandleEntryPtr = LIST_NULL;
    DSNodeHandleEntryPtr = LIST_NULL;
    logi=i=MaskTmp=CompareCnt=0;

	if (IsHandleEmpty(HeadHandle)==True || KeyWordPtrArray==LIST_NULL || NodeHandle==LIST_NULL)
	{
		return LIST_ERROR;
	}
	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	DSNodeHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	if (DSHandleEntryPtr->HandleType!=LIST_HEAD_TYPE || DSNodeHandleEntryPtr->HandleType!=LIST_NODE_TYPE)
	{
		printf("List class error: DS handle type error!\n");
		return LIST_ERROR;
	}
	if(DSHandleEntryPtr->EntryPtr)
	{
		list=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;
	}
    else
    {
        return LIST_ERROR;
    }
    if (list->CmpFcnArray)
    {
        CmpFcnArray=list->CmpFcnArray;
    }
    else
    {
        return LIST_ERROR;
    }

	mutex_lock(list->MutexHandle, MG_WAIT_FOREVER);
    if (list->EntriesList)
    {
        CurrentNodePtr=(LIST_NODE_PTR)list->EntriesList;
    } 
    else
    {
		mutex_unlock(list->MutexHandle);
        return LIST_ERROR;
    }
	/* Get a temporary read permission */
	CurrentNodePtr->ReadPermission++;
	mutex_unlock(list->MutexHandle);
                                                                                                                                                                                                                                         
	while (CurrentNodePtr)
	{
		MaskTmp=CmpFcnIndex;
		logi=0;
		/* Compare all the key words */
		CompareCnt=0;
		for (i=0;i<list->CmpFcnAmount;i++)
		{
			if ((MaskTmp & 0x1) && KeyWordPtrArray[i])
			{
				CmpFcnPtr=CmpFcnArray[i];
				logi+=(((*CmpFcnPtr)(CurrentNodePtr->data,KeyWordPtrArray[i]),UserData)==0);
				CompareCnt++;
			}
			MaskTmp>>=1;
		}

		if (logi==CompareCnt)
		{
			mutex_lock(list->MutexHandle, MG_WAIT_FOREVER);
			CurrentNodePtr->ReadPermission--;	/* Release temporary permission */
			DSNodeHandleEntryPtr->EntryPtr=(void*)CurrentNodePtr;
			DSNodeHandleEntryPtr->HandleType=LIST_NODE_TYPE;
			/* Regain the permission */
			switch(PermissionTag)
			{
			case READ_PERMISSION_TAG:
				if (CurrentNodePtr->WritePermission>0)
				{
					mutex_unlock(list->MutexHandle);
					return PERMISSION_DENY;
				}
				CurrentNodePtr->ReadPermission++;
				break;
			case WRITE_PERMISSION_TAG:
				if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)
				{
					mutex_unlock(list->MutexHandle);
					return PERMISSION_DENY;
				}
				CurrentNodePtr->WritePermission++;
				break;
			default:
				break;
			}
			mutex_unlock(list->MutexHandle);
			return LIST_OK;
		}

		mutex_lock(list->MutexHandle, MG_WAIT_FOREVER);
		/* Release the temporary permission of previous node */
		CurrentNodePtr->ReadPermission--;
		CurrentNodePtr=CurrentNodePtr->NextNode;
		/* Get the temporary permission of current node */
		if (CurrentNodePtr)
		{
			CurrentNodePtr->ReadPermission++;
		}
		mutex_unlock(list->MutexHandle);
	}
	return LIST_ERROR;
}


int InsertNode(LIST_HANDLE handle,int length,void* DataPtr)
{
	LIST_NODE_PTR CurrentNodePtr,InsertionNodePtr;
	LIST_HEAD_PTR list;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;

    CurrentNodePtr=InsertionNodePtr=LIST_NULL;
    list = LIST_NULL;
    DSHandleEntryPtr = LIST_NULL;

	if (IsHandleEmpty(handle)
        ||(DataPtr==LIST_NULL))
	{
		return LIST_ERROR;
	}
	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;

	if (DSHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		printf("List class error: DS handle type error!\n");
		return LIST_ERROR;
	}

	if ((InsertionNodePtr=(LIST_NODE_PTR)malloc(sizeof(LIST_NODE)))==0)
	{
		return LIST_ERROR;
	}

	memset(InsertionNodePtr,0,sizeof(LIST_NODE));
	InsertionNodePtr->data=DataPtr;
	InsertionNodePtr->length=length;
	InsertionNodePtr->HeadPtr=DSHandleEntryPtr->EntryPtr;

    list=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;
	mutex_lock(list->MutexHandle, MG_WAIT_FOREVER);
	CurrentNodePtr=(LIST_NODE_PTR)list->EntriesList;
	if (CurrentNodePtr==0)	/* The first node of the list */
	{
		list->EntriesList=(void*)InsertionNodePtr;
		InsertionNodePtr->NextNode=LIST_NULL;
		InsertionNodePtr->PreviousNode=LIST_NULL;
	}
	else
	{
		InsertionNodePtr->PreviousNode=LIST_NULL;
		/* Relink */
		InsertionNodePtr->NextNode=CurrentNodePtr;
		CurrentNodePtr->PreviousNode=InsertionNodePtr;
		/* mount */
		list->EntriesList=(void*)InsertionNodePtr;
	}
	/* write back */
	list->EntryAmount++;
	mutex_unlock(list->MutexHandle);

	return LIST_OK;
}

int DeleteNodeSW(
				 LIST_HANDLE HeadHandle,
				 void** KeyWordPtrArray,
				 int CmpFcnIndex,
				 void** DelNodeDataPtr
				 )
{
	DS_HANDLE_ENTRY HandleEntry;
	DS_HANDLE_ENTRY_PTR HandleEntryPtr;
	LIST_HEAD_PTR   ListHeadPtr;
	LIST_NODE_PTR		CurrentNodePtr,PreviousNodePtr,NextNodePtr;
	LIST_HANDLE		handle;
	void*			DataPtr;
	int				retval;

    HandleEntryPtr = LIST_NULL;
    ListHeadPtr = LIST_NULL;
    CurrentNodePtr=PreviousNodePtr=NextNodePtr=LIST_NULL;
    handle = LIST_NULL;
    DataPtr = LIST_NULL;
    retval = 0;

	/* parameters check */
	if (IsHandleEmpty(HeadHandle) || KeyWordPtrArray==LIST_NULL || CmpFcnIndex==0)
	{
		return LIST_ERROR;
	}
	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	if (HandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		return LIST_ERROR;
	}

	HandleEntry.HandleType=LIST_NODE_TYPE;
	handle=(LIST_HANDLE)&HandleEntry;
	retval=FindSW(HeadHandle,KeyWordPtrArray,CmpFcnIndex,handle,WRITE_PERMISSION_TAG);
	if (retval==LIST_ERROR || retval==PERMISSION_DENY)
	{
		return retval;
	}
    
	mutex_lock(ListHeadPtr->MutexHandle, MG_WAIT_FOREVER);
	CurrentNodePtr=(LIST_NODE_PTR)HandleEntry.EntryPtr;
	if (CurrentNodePtr==0)
	{
		mutex_unlock(ListHeadPtr->MutexHandle);
		return LIST_ERROR;
	}
	CurrentNodePtr->WritePermission--;	/* Release write permission */
	if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)
	{
		mutex_unlock(ListHeadPtr->MutexHandle);
		return LIST_ERROR;
	}
	PreviousNodePtr=CurrentNodePtr->PreviousNode;
	NextNodePtr=CurrentNodePtr->NextNode;
	if (PreviousNodePtr==LIST_NULL)	/* First node */
	{
		ListHeadPtr->EntriesList=NextNodePtr;
		if (NextNodePtr!=LIST_NULL)
		{
			NextNodePtr->PreviousNode=LIST_NULL;
		}
	}
	else if (NextNodePtr==LIST_NULL)	/* Last node */
	{
		PreviousNodePtr->NextNode=NextNodePtr;
	}
	else
	{
		PreviousNodePtr->NextNode=NextNodePtr;
		NextNodePtr->PreviousNode=PreviousNodePtr;
	}
	DataPtr=CurrentNodePtr->data;
	ListHeadPtr->EntryAmount--;
	mutex_unlock(ListHeadPtr->MutexHandle);
	if (CurrentNodePtr!=LIST_NULL)
	{
		free(CurrentNodePtr);
		CurrentNodePtr=LIST_NULL;
	}

	*DelNodeDataPtr=(void*)DataPtr;

	return LIST_OK;
}

int DeleteNodeMW(
				 LIST_HANDLE HeadHandle,
				 void** KeyWordPtrArray,
				 int CmpFcnIndex,
				 void** DelNodeDataPtr
				 )
{
	DS_HANDLE_ENTRY HandleEntry;
	DS_HANDLE_ENTRY_PTR HandleEntryPtr;
	LIST_HEAD_PTR   ListHeadPtr;
	LIST_NODE_PTR		CurrentNodePtr,PreviousNodePtr,NextNodePtr;
	LIST_HANDLE		handle;
	void*			DataPtr;
	int				retval;

    HandleEntryPtr = LIST_NULL;
    ListHeadPtr = LIST_NULL;
    CurrentNodePtr=PreviousNodePtr=NextNodePtr=LIST_NULL;
    handle = LIST_NULL;
    DataPtr = LIST_NULL;
    retval = 0;

	/* parameters check */
	if (IsHandleEmpty(HeadHandle)==True || KeyWordPtrArray==LIST_NULL || CmpFcnIndex==0)
	{
		return LIST_ERROR;
	}
	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	if (HandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		return LIST_ERROR;
	}

	HandleEntry.HandleType=LIST_NODE_TYPE;
	handle=(LIST_HANDLE)&HandleEntry;
	
	retval=FindMW(HeadHandle,KeyWordPtrArray,CmpFcnIndex,handle,WRITE_PERMISSION_TAG);
	if (retval==LIST_ERROR || retval==PERMISSION_DENY)
	{
		return retval;
	}

	ListHeadPtr=HandleEntryPtr->EntryPtr;
    
	mutex_lock(ListHeadPtr->MutexHandle, MG_WAIT_FOREVER);
	CurrentNodePtr=(LIST_NODE_PTR)HandleEntry.EntryPtr;
	if (CurrentNodePtr==LIST_NULL)
	{
		mutex_unlock(ListHeadPtr->MutexHandle);
		return LIST_ERROR;
	}
	CurrentNodePtr->WritePermission--;	/* Release write permission */

	if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)
	{
		mutex_unlock(ListHeadPtr->MutexHandle);
		return LIST_ERROR;
	}
	PreviousNodePtr=CurrentNodePtr->PreviousNode;
	NextNodePtr=CurrentNodePtr->NextNode;
	if (PreviousNodePtr==LIST_NULL)	/* First node */
	{
		ListHeadPtr->EntriesList=NextNodePtr;
		if (NextNodePtr!=LIST_NULL)
		{
			NextNodePtr->PreviousNode=LIST_NULL;
		}
	}
	else if (NextNodePtr==LIST_NULL)	/* Last node */
	{
		PreviousNodePtr->NextNode=NextNodePtr;
	}
	else
	{
		PreviousNodePtr->NextNode=NextNodePtr;
		NextNodePtr->PreviousNode=PreviousNodePtr;
	}

	DataPtr=CurrentNodePtr->data;
	ListHeadPtr->EntryAmount--;
	mutex_unlock(ListHeadPtr->MutexHandle);
	
	if (CurrentNodePtr!=LIST_NULL)
	{
		free(CurrentNodePtr);
		CurrentNodePtr=LIST_NULL;
	}
	
	*DelNodeDataPtr=(void*)DataPtr;

	return LIST_OK;
}


int DeleteNodes( LIST_HANDLE handle )
{
	LIST_NODE_PTR	CurrentNodePtr,PreviousNodePtr;
	LIST_HEAD_PTR list;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;
    int EntryAmount;

    CurrentNodePtr=PreviousNodePtr=LIST_NULL;
    list = LIST_NULL;
    DSHandleEntryPtr = LIST_NULL;

	if (IsHandleEmpty(handle))
	{
		return LIST_ERROR;
	}
	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
	if (DSHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		printf("List class error: DS handle type error!\n");
		return LIST_ERROR;
	}

	list=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;
    if (list==LIST_NULL)
    {
        return LIST_ERROR;
    }

	mutex_lock(list->MutexHandle, MG_WAIT_FOREVER);
	CurrentNodePtr=PreviousNodePtr=(LIST_NODE_PTR)list->EntriesList;
	while (CurrentNodePtr)
	{
		if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)	/* Jump to next node */
		{
			PreviousNodePtr=CurrentNodePtr;
			CurrentNodePtr=CurrentNodePtr->NextNode;
		}
		else
		{
			if (CurrentNodePtr==PreviousNodePtr)	/* First node */
			{
				list->EntriesList=CurrentNodePtr->NextNode;
                if (CurrentNodePtr != LIST_NULL)
                {
                    free(CurrentNodePtr);
                    CurrentNodePtr = LIST_NULL;
                }
				list->EntryAmount--;
				CurrentNodePtr=PreviousNodePtr=(LIST_NODE_PTR)list->EntriesList;
				CurrentNodePtr->PreviousNode=LIST_NULL;
			}	
			else									/* Normal node */
			{
				PreviousNodePtr->NextNode=CurrentNodePtr->NextNode;
				CurrentNodePtr->NextNode->PreviousNode=PreviousNodePtr;
                if (CurrentNodePtr != LIST_NULL)
                {
                    free(CurrentNodePtr);
                    CurrentNodePtr = LIST_NULL;               
                }
				list->EntryAmount--;
				CurrentNodePtr=PreviousNodePtr->NextNode;
			}
		}
	}

    EntryAmount=list->EntryAmount;
	mutex_unlock(list->MutexHandle);

	return EntryAmount;
}

int Delete(LIST_HANDLE handle)
{
    int retval;
    LIST_HEAD_PTR list;
    DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;

    if ((retval=DeleteNodes(handle)) != 0)
    {
        return retval;
    }
    
    DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
    
    list=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;
    
    if (list->MutexHandle)
    {
        mutex_delete(list->MutexHandle);
        list->MutexHandle=LIST_NULL;
    }

    if (list->CmpFcnArray)
    {
        free(list->CmpFcnArray);
        list->CmpFcnArray=LIST_NULL;
    }
 
    free(list);

    DSHandleEntryPtr->EntryPtr=LIST_NULL;

    return LIST_OK;
}

int ReleasePermission(LIST_HANDLE NodeHandle,int PermissionTag)
{
	LIST_NODE_PTR			NodePtr;
	DS_HANDLE_ENTRY_PTR	HandleEntryPtr;

    NodePtr = LIST_NULL;
    HandleEntryPtr = LIST_NULL;

	if (IsHandleEmpty(NodeHandle))
	{
		return LIST_ERROR;
	}

	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	if (HandleEntryPtr->HandleType!=LIST_NODE_TYPE)
	{
		return LIST_ERROR;
	}

	if (HandleEntryPtr->EntryPtr)
	{
		NodePtr=(LIST_NODE_PTR)HandleEntryPtr->EntryPtr;
	}
	else
	{
		return LIST_ERROR;
	}

	mutex_lock(NodePtr->HeadPtr->MutexHandle, MG_WAIT_FOREVER);
	switch(PermissionTag)
	{
	case READ_PERMISSION_TAG:
		if (NodePtr->ReadPermission>0)
		{
			NodePtr->ReadPermission--;
		}
		break;
	case WRITE_PERMISSION_TAG:
		if (NodePtr->WritePermission>0)
		{
			NodePtr->WritePermission--;
		}
	    break;
	default:
	    break;
	}
	mutex_unlock(NodePtr->HeadPtr->MutexHandle);
	return LIST_OK;
}


int PrintPermission(LIST_HANDLE NodeHandle,int PermissionTag)
{
	LIST_NODE_PTR			NodePtr;
	DS_HANDLE_ENTRY_PTR	HandleEntryPtr;

    NodePtr = LIST_NULL;
    HandleEntryPtr = LIST_NULL;

	if (IsHandleEmpty(NodeHandle)==True)
	{
		return LIST_ERROR;
	}
	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	if (HandleEntryPtr->HandleType!=LIST_NODE_TYPE)
	{
		return LIST_ERROR;
	}
	NodePtr=(LIST_NODE_PTR)HandleEntryPtr->EntryPtr;
	
	mutex_lock(NodePtr->HeadPtr->MutexHandle, MG_WAIT_FOREVER);
	switch(PermissionTag)
	{
	case READ_PERMISSION_TAG:
        printf("Read Permission Tag:%d\n", NodePtr->ReadPermission);
		break;
	case WRITE_PERMISSION_TAG:
        printf("Write Permission Tag:%d\n", NodePtr->WritePermission);
		break;
	default:
		break;
	}
	mutex_unlock(NodePtr->HeadPtr->MutexHandle);
	return LIST_OK;
}


int GetPermisssion(LIST_HANDLE NodeHandle,int PermissionTag)
{
	LIST_NODE_PTR			NodePtr;
	DS_HANDLE_ENTRY_PTR	HandleEntryPtr;

    NodePtr = LIST_NULL;
    HandleEntryPtr = LIST_NULL;

	if (IsHandleEmpty(NodeHandle)==True)
	{
		return LIST_ERROR;
	}
	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	if (HandleEntryPtr->HandleType!=LIST_NODE_TYPE)
	{
		return LIST_ERROR;
	}
	NodePtr=(LIST_NODE_PTR)HandleEntryPtr->EntryPtr;

    mutex_lock(NodePtr->HeadPtr->MutexHandle, MG_WAIT_FOREVER);
	switch(PermissionTag)
	{
	case READ_PERMISSION_TAG:
		if (NodePtr->WritePermission>0)
		{
			mutex_unlock(NodePtr->HeadPtr->MutexHandle);
			return PERMISSION_DENY;
		}
		else
		{
			NodePtr->ReadPermission++;
		}
		break;
	case WRITE_PERMISSION_TAG:
		if (NodePtr->ReadPermission>0 || NodePtr->WritePermission>0)
		{
			mutex_unlock(NodePtr->HeadPtr->MutexHandle);
			return PERMISSION_DENY;
		}
		else
		{
			NodePtr->WritePermission++;
		}
		break;
	default:
		break;
	}
	mutex_unlock(NodePtr->HeadPtr->MutexHandle);
	
	return LIST_OK;
}


void* GetData( LIST_HANDLE NodeHandle )
{
	LIST_NODE_PTR			NodePtr;
	LIST_HEAD_PTR				ListHeadPtr;
	DS_HANDLE_ENTRY_PTR		HandleEntryPtr;

    NodePtr = LIST_NULL;
    ListHeadPtr = LIST_NULL;
    HandleEntryPtr = LIST_NULL;

	if (IsHandleEmpty(NodeHandle)==True)
	{
		return LIST_OK;
	}

	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	switch(HandleEntryPtr->HandleType)
	{
	case LIST_NODE_TYPE:
		NodePtr=(LIST_NODE_PTR)HandleEntryPtr->EntryPtr;
		return NodePtr->data;
	case LIST_HEAD_TYPE:
		return LIST_OK;
	default:
	    break;
	}

	return LIST_OK;
}

int GetNextNode(
				LIST_HANDLE HeadHandle,
				LIST_HANDLE handle, 
				LIST_HANDLE NextHandle,
				int PermissionTag
				)
{
	LIST_NODE_PTR		NodePtr,NextNodePtr;
	LIST_HEAD_PTR		HeadPtr;
	DS_HANDLE_ENTRY_PTR	HeadHandleEntryPtr,HandleEntryPtr,NextHandleEntryPtr;

    NodePtr=NextNodePtr=LIST_NULL;
    HeadPtr = LIST_NULL;
    HeadHandleEntryPtr=LIST_NULL;
    HeadHandleEntryPtr=LIST_NULL;
    NextHandleEntryPtr=LIST_NULL;

	if (IsHandleEmpty(HeadHandle) || IsHandleEmpty(handle) || NextHandle==LIST_NULL)
	{
		return LIST_ERROR;
	}
	
	HeadHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	if (HeadHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		return LIST_ERROR;
	}

	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
	NextHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NextHandle;
	
	HeadPtr=HeadHandleEntryPtr->EntryPtr;
    
	mutex_lock(HeadPtr->MutexHandle, MG_WAIT_FOREVER);
	if (HandleEntryPtr->HandleType==LIST_HEAD_TYPE)
	{
		HeadPtr=(LIST_HEAD_PTR)HandleEntryPtr->EntryPtr;
		NextHandleEntryPtr->HandleType=LIST_NODE_TYPE;
		NextHandleEntryPtr->EntryPtr=HeadPtr->EntriesList;
		NodePtr=(LIST_NODE_PTR)NextHandleEntryPtr->EntryPtr;
		
		if (NodePtr==LIST_NULL)
		{
			mutex_unlock(HeadPtr->MutexHandle);
			return LIST_ERROR;
		}

		switch(PermissionTag)
		{
		case READ_PERMISSION_TAG:
			if (NodePtr->WritePermission>0)
			{
				mutex_unlock(HeadPtr->MutexHandle);
				return PERMISSION_DENY;
			}
			NodePtr->ReadPermission++;

			break;
		case WRITE_PERMISSION_TAG:
			if (NodePtr->ReadPermission>0 || NodePtr->WritePermission>0)
			{
				mutex_unlock(HeadPtr->MutexHandle);
				return PERMISSION_DENY;
			}
			NodePtr->WritePermission++;
		    break;
		default:
		    break;
		}
	}
	else if (HandleEntryPtr->HandleType==LIST_NODE_TYPE)
	{
		NodePtr=HandleEntryPtr->EntryPtr;
		NextNodePtr=NodePtr->NextNode;
		if (NextNodePtr==LIST_NULL)
		{
			mutex_unlock(HeadPtr->MutexHandle);
			return LIST_ERROR;
		}
		NextHandleEntryPtr->HandleType=LIST_NODE_TYPE;
		NextHandleEntryPtr->EntryPtr=NextNodePtr;
		switch(PermissionTag)
		{
		case READ_PERMISSION_TAG:
			if (NextNodePtr->WritePermission>0)
			{
				mutex_unlock(HeadPtr->MutexHandle);
				return PERMISSION_DENY;
			}
			NextNodePtr->ReadPermission++;
			break;
		case WRITE_PERMISSION_TAG:
			if (NextNodePtr->ReadPermission>0 || NextNodePtr->WritePermission>0)
			{
				mutex_unlock(HeadPtr->MutexHandle);
				return PERMISSION_DENY;
			}
			NextNodePtr->WritePermission++;
		    break;
		default:
		    break;
		}
	}
	mutex_unlock(HeadPtr->MutexHandle);
	return LIST_OK;
}

int GetPreviousNode(
				LIST_HANDLE HeadHandle,
				LIST_HANDLE handle, 
				LIST_HANDLE PreviousHandle,
				int PermissionTag
				)
{
	LIST_NODE_PTR		NodePtr,PreviousNodePtr;
	LIST_HEAD_PTR		HeadPtr;
	DS_HANDLE_ENTRY_PTR	HeadHandleEntryPtr,HandleEntryPtr,PreHandleEntryPtr;

    NodePtr=PreviousNodePtr=LIST_NULL;
    HeadPtr = LIST_NULL;
    HeadHandleEntryPtr=LIST_NULL;
    HeadHandleEntryPtr=LIST_NULL;
    PreHandleEntryPtr=LIST_NULL;

	if (IsHandleEmpty(HeadHandle) || IsHandleEmpty(handle) || PreviousHandle==LIST_NULL)
	{
		return LIST_ERROR;
	}

	HeadHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	if (HeadHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		return LIST_ERROR;
	}
	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
	PreHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)PreviousHandle;

	HeadPtr=HeadHandleEntryPtr->EntryPtr;
    mutex_lock(HeadPtr->MutexHandle, MG_WAIT_FOREVER);

	if (HandleEntryPtr->HandleType==LIST_HEAD_TYPE)
	{
		mutex_unlock(HeadPtr->MutexHandle);
		return LIST_ERROR;
	}
	else if (HandleEntryPtr->HandleType==LIST_NODE_TYPE)
	{
		NodePtr=HandleEntryPtr->EntryPtr;
		PreviousNodePtr=NodePtr->PreviousNode;
		if (PreviousNodePtr==LIST_NULL)
		{
			mutex_unlock(HeadPtr->MutexHandle);
			return LIST_ERROR;
		}
		PreHandleEntryPtr->HandleType=LIST_NODE_TYPE;
		PreHandleEntryPtr->EntryPtr=PreviousNodePtr;
		switch(PermissionTag)
		{
		case READ_PERMISSION_TAG:
			if (PreviousNodePtr->WritePermission>0)
			{
				mutex_unlock(HeadPtr->MutexHandle);
				return PERMISSION_DENY;
			}
			PreviousNodePtr->ReadPermission++;
			break;
		case WRITE_PERMISSION_TAG:
			if (PreviousNodePtr->ReadPermission>0 || PreviousNodePtr->WritePermission>0)
			{
				mutex_unlock(HeadPtr->MutexHandle);
				return PERMISSION_DENY;
			}
			PreviousNodePtr->WritePermission++;
			break;
		default:
			break;
		}
	}
	mutex_unlock(HeadPtr->MutexHandle);
	return LIST_OK;
}

int GetDataLen(LIST_HANDLE handle)
{
	DS_HANDLE_ENTRY_PTR	HandleEntryPtr;
	LIST_NODE_PTR NodePtr;

    HandleEntryPtr = LIST_NULL;
    NodePtr = LIST_NULL;

	if (handle==LIST_NULL)
	{
		return LIST_ERROR;
	}

	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
	if (HandleEntryPtr->HandleType==LIST_HEAD_TYPE)
	{
		return LIST_ERROR;
	}

	NodePtr=(LIST_NODE_PTR)HandleEntryPtr->EntryPtr;
	if (NodePtr==LIST_NULL)
	{
		return LIST_ERROR;
	}
	return NodePtr->length;
}


LOGICAL IsHandleEmpty(LIST_HANDLE handle)
{
	DS_HANDLE_ENTRY_PTR	HandleEntryPtr;

    HandleEntryPtr = LIST_NULL;

	if (handle==LIST_NULL)
	{
		return True;
	}
	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
    return (HandleEntryPtr->EntryPtr==LIST_NULL)?True:False;
}

void HandleCpy(LIST_HANDLE DesHandle,const LIST_HANDLE SrcHandle)
{
	if (DesHandle==LIST_NULL || IsHandleEmpty(SrcHandle))
	{
		return;
	}
	memcpy(DesHandle,SrcHandle,sizeof(DS_HANDLE_ENTRY));
    return;
}
