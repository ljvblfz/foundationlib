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
 *  1. integer HandleType [input] (LIST_HEAD_TYPE, LIST_NODE_TYPE), it indicates the type of handle
 *  Return:
 *  handle pointer, a valid pointer or NULL when memory limits
 */
LIST_HANDLE CreateHandle(int HandleType)
{
	DS_HANDLE_ENTRY_PTR HandleEntryPtr;

	if ((HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)
		malloc(sizeof(DS_HANDLE_ENTRY)))==NULL)
	{
		return NULL;
	}
	memset(HandleEntryPtr,0,sizeof(DS_HANDLE_ENTRY));
	HandleEntryPtr->HandleType=HandleType;
	return (LIST_HANDLE)HandleEntryPtr;
}

/*
 *  Description:
 *	Function "DeleteHandle" deletes a handle. Note: It just deletes the handle descriptor.
 *  It does not delete the member data which the handle represents. 
 *  
 *  Parameters:
 *  1. LIST_HANDLE handle [input] (valid pointer value)
 *
 *  Return:
 *  No expected return value.
 */
void DeleteHandle(LIST_HANDLE handle)
{
    DS_HANDLE_ENTRY_PTR HandleEntryPtr;

	if (handle != NULL)
	{
        free(handle);
        handle = NULL;
    }

    return;
}

/*
 *	Description:
 *	Function "Initialize" initializes all the members of the head structure. 
 *  It creates a comparison function table.
 *		
 *	Parameters:
 *	1. LIST_HANDLE handle [input] (valid pointer), a handle represents the list.
 *  2. int CmpFcnAmount [input] (>0), Comparison function amount
 *  3. void** CmpFcnArray [input] (valid pointer), a pointer points to a function array 
 *      which also contains CmpFcnAmount pointers point to each comparison function.
 *  
 *	Return:
 *	Function Execution Status, (OK, ERROR)
 */
int Initialize(
               LIST_HANDLE handle,
				int	CmpFcnAmount,
				void** CmpFcnArray
			   )
{
	LIST_HEAD_PTR	ListHeadPtr;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;
    int         opt;

	ListHeadPtr = NULL;
    DSHandleEntryPtr = NULL;

	if ((handle == NULL)||(CmpFcnAmount == 0))
	{
		return ERROR;
	}
	/* Allocate list head */
	if ((ListHeadPtr=(LIST_HEAD_PTR)malloc(sizeof(LIST_HEAD)))==0)
	{
		return ERROR;
	}
	memset((void *)ListHeadPtr, 0, sizeof(LIST_HEAD));

	/* Allocate compare callback function array */
	ListHeadPtr->CmpFcnAmount=CmpFcnAmount;
	if ((ListHeadPtr->CmpFcnArray=(void**)
		malloc(CmpFcnAmount*sizeof(void*)))==NULL)
	{
		if (ListHeadPtr != NULL)
		{
            free(ListHeadPtr);
            ListHeadPtr = NULL;
		}
        return ERROR;
	}

	memcpy(ListHeadPtr->CmpFcnArray,CmpFcnArray,CmpFcnAmount*sizeof(void*));
	ListHeadPtr->EntryAmount=0;
	ListHeadPtr->EntriesList=NULL;
    opt=OSI_SEM_Q_PRIORITY | OSI_SEM_DELETE_SAFE | OSI_SEM_INVERSION_SAFE;
	ListHeadPtr->MutexHandle=mutex_create(opt);

	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
	DSHandleEntryPtr->EntryPtr=(void*)ListHeadPtr;
	DSHandleEntryPtr->HandleType=LIST_HEAD_TYPE;
	return OK;
}

/*
 *	Description:
 *	Function "Count" returns the entry amount of list.
 *		
 *	Parameters:
 *	1. LIST_HANDLE handle [input] (non-empty handle), the handle of list head
 *	Return:
 *	Function Execution Status, (ERROR, OK).
 */
int Count(LIST_HANDLE handle)
{
	LIST_HEAD_PTR	ListHeadPtr;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;
	int amount;
	
    ListHeadPtr = NULL;
    DSHandleEntryPtr = NULL;
    amount = 0;

	if (IsHandleEmpty(handle)==True)
	{
		return ERROR;
	}

	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
	if (DSHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		printf("List class error: DS HeadHandle type error!\n");
		return ERROR;
	}
	ListHeadPtr=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;
    if (ListHeadPtr == NULL)
    {
        return ERROR;
    }

	mutex_lock(ListHeadPtr->MutexHandle, OSI_WAIT_FOREVER);
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
 *
 *	Return:
 *	ERROR if an error occur. False if the list is not empty, True if 
 *	the list is empty. 
 */
int IsEmpty(LIST_HANDLE HeadHandle)
{
	LIST_HEAD_PTR list;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;
	LOGICAL logi;

    list = NULL;
    DSHandleEntryPtr = NULL;
    logi = False;

	if (IsHandleEmpty(HeadHandle)==True)
	{
		return ERROR;
	}

	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	if (DSHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		return ERROR;
	}

    if (DSHandleEntryPtr->EntryPtr)
    {
        list=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;
    }
    else
    {
        return ERROR;
    }

	mutex_lock(list->MutexHandle, OSI_WAIT_FOREVER);
	logi=(list->EntryAmount==0)?True:False;
	mutex_unlock(list->MutexHandle);
	
	return logi;
}

/*
 *	Description:
 *	Find a specific node with single keyword match.	
 *	Parameters:
 *	1. LIST_HANDLE HeadHandle [input] (non-empty handle), handle of list
 *	2. void** KeyWordPtrArray [input] (valid pointer), a pointer points to keyword pointer array
 *	3. int CmpFcnIndex [input] (none zero integer), compare keyword mark bits.
 *	4. LIST_HANDLE NodeHandle [output] (non-empty handle), a handle for returning the matched node.
 *	5. int PermissionTag [input] (READ_PERMISSION_TAG, WRITE_PERMISSION_TAG), 
 *		set the access permission for obtained node.
 *  6. void* UserData [output] (valid pointer), a pointer points to extra user data. In some cases
 *      user may want to transfer some extra information during finding action.
 *	Return:
 *	ERROR: When a error occur
 *	OK: Successfully return a matched node with the desired access permission.
 *	LIST_PERMISSION_DENY: The matched node refuses to endow the expected permission, 
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
	DS_HANDLE_ENTRY_PTR DSHeadHandleEntryPtr;
	DS_HANDLE_ENTRY_PTR DSNodeHandleEntryPtr;
	int logi,i,MaskTmp;

    CurrentNodePtr = NULL;
    CmpFcnArray = NULL;
    list = NULL;
    DSHeadHandleEntryPtr = NULL;
    DSNodeHandleEntryPtr = NULL;
    logi=i=MaskTmp=0;

	if (IsEmpty(HeadHandle) || KeyWordPtrArray==NULL || NodeHandle==NULL)
	{
		return ERROR;
	}

	DSHeadHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	DSNodeHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	if (DSHeadHandleEntryPtr->HandleType!=LIST_HEAD_TYPE || DSNodeHandleEntryPtr->HandleType!=LIST_NODE_TYPE)
	{
		printf("List class error: DS handle type error!\n");
		return ERROR;
	}

    list=(LIST_HEAD_PTR)DSHeadHandleEntryPtr->EntryPtr;
    
    if (list->CmpFcnArray)
    {
        CmpFcnArray=list->CmpFcnArray;
    } 
    else
    {
        return ERROR;
    }

	mutex_lock(list->MutexHandle, OSI_WAIT_FOREVER);

    if (list->EntriesList)
    {
        CurrentNodePtr=list->EntriesList;
    } 
    else
    {
		mutex_unlock(list->MutexHandle);
        return ERROR;
    }
	
    /* Get a temporary read permission */
	CurrentNodePtr->ReadPermission++;
	mutex_unlock(list->MutexHandle);

	while (1)
	{
		MaskTmp=CmpFcnIndex;
		/* Search the first matched tag */
		for (i=0;i<list->CmpFcnAmount;i++)
		{
			if ((MaskTmp & 0x1) && KeyWordPtrArray[i])
			{
				CmpFcnPtr=CmpFcnArray[i];
				if (((*CmpFcnPtr)(CurrentNodePtr->data,KeyWordPtrArray[i]),UserData)==OK)
				{
					mutex_lock(list->MutexHandle, OSI_WAIT_FOREVER);
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
							return LIST_PERMISSION_DENY;
						}

						CurrentNodePtr->ReadPermission++;
						break;
					case WRITE_PERMISSION_TAG:
						if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)
						{
							mutex_unlock(list->MutexHandle);
							return LIST_PERMISSION_DENY;
						}

						CurrentNodePtr->WritePermission++;
					    break;
					default:
					    break;
					}
					mutex_unlock(list->MutexHandle);
					return OK;
				}
			}
			MaskTmp>>=1;
		}

		mutex_lock(list->MutexHandle, OSI_WAIT_FOREVER);
		/* Release temporary read permission of previous node */
		CurrentNodePtr->ReadPermission--;
		CurrentNodePtr=CurrentNodePtr->NextNode;
		/* List rear judgment */
        if (CurrentNodePtr == list->EntriesList)
		{
            mutex_unlock(list->MutexHandle);
            break;
		}

        /* Get temporary read permission of current node */
        CurrentNodePtr->ReadPermission++;
		mutex_unlock(list->MutexHandle);
	}

	return ERROR;
}

/*
 *	Description:
 *	Find a specific node with multiple keyword match.	
 *
 *	Parameters:
 *	1. LIST_HANDLE HeadHandle [input] (non-empty handle), handle of list
 *	2. void** KeyWordPtrArray [input] (valid pointer), a pointer points to keyword pointer array
 *	3. int CmpFcnIndex [input] (none zero integer), comparison keyword mark bits.
 *	4. LIST_HANDLE NodeHandle [output] (non-empty handle), a handle for returning the matched node.
 *	5. int PermissionTag [input] (READ_PERMISSION_TAG, WRITE_PERMISSION_TAG), 
 *		set the access permission for obtained node.
 *  6. void* UserDataArray [output] (valid pointer), a pointer points to extra user data array. In some cases
 *      user may want to transfer some extra information during finding action. And since FindMW uses multiple
 *      keywords matching, user can define user data for each keyword matching.
 *
 *	Return:
 *	ERROR: When an error occur
 *	OK: Successfully return a matched node with the desired access permission.
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
		   void** UserDataArray
		   )
{
	LIST_NODE_PTR CurrentNodePtr;
	int (*CmpFcnPtr)(void*,void*);
	void** CmpFcnArray;
	LIST_HEAD_PTR list;
	DS_HANDLE_ENTRY_PTR DSHeadHandleEntryPtr;
	DS_HANDLE_ENTRY_PTR DSNodeHandleEntryPtr;
	int logi,i,MaskTmp,CompareCnt;

    CurrentNodePtr = NULL;
    CmpFcnArray = NULL;
    list = NULL;
    DSHeadHandleEntryPtr = NULL;
    DSNodeHandleEntryPtr = NULL;
    logi=i=MaskTmp=CompareCnt=0;

	if (
        IsHandleEmpty(HeadHandle)==True || IsHandleEmpty(NodeHandle)==True 
        || KeyWordPtrArray==NULL || UserDataArray==NULL
        )
	{
		return ERROR;
	}
	DSHeadHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	DSNodeHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	if (DSHeadHandleEntryPtr->HandleType!=LIST_HEAD_TYPE || DSNodeHandleEntryPtr->HandleType!=LIST_NODE_TYPE)
	{
		printf("List class error: DS handle type error!\n");
		return ERROR;
	}

	list=(LIST_HEAD_PTR)DSHeadHandleEntryPtr->EntryPtr;

    if (list->CmpFcnArray)
    {
        CmpFcnArray=list->CmpFcnArray;
    }
    else
    {
        return ERROR;
    }

	mutex_lock(list->MutexHandle, OSI_WAIT_FOREVER);
    if (list->EntriesList)
    {
        CurrentNodePtr=(LIST_NODE_PTR)list->EntriesList;
    } 
    else
    {
		mutex_unlock(list->MutexHandle);
        return ERROR;
    }
	/* Get a temporary read permission */
	CurrentNodePtr->ReadPermission++;
	mutex_unlock(list->MutexHandle);
                                                                                                                                                                                                                                         
	while (1)
	{
		MaskTmp=CmpFcnIndex;
		logi=0;
		/* Compare all the key words */
		CompareCnt=0;
		for (i=0;i<list->CmpFcnAmount;i++)
		{
			if ((MaskTmp & 0x1) && KeyWordPtrArray[i] && UserDataArray[i])
			{
				CmpFcnPtr=CmpFcnArray[i];
				logi+=(((*CmpFcnPtr)(CurrentNodePtr->data,KeyWordPtrArray[i]),UserDataArray[i])==OK);
				CompareCnt++;
			}
			MaskTmp>>=1;
		}

		if (logi==CompareCnt)
		{
			mutex_lock(list->MutexHandle, OSI_WAIT_FOREVER);
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
					return LIST_PERMISSION_DENY;
				}
				CurrentNodePtr->ReadPermission++;
				break;
			case WRITE_PERMISSION_TAG:
				if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)
				{
					mutex_unlock(list->MutexHandle);
					return LIST_PERMISSION_DENY;
				}
				CurrentNodePtr->WritePermission++;
				break;
			default:
				break;
			}
			mutex_unlock(list->MutexHandle);
			return OK;
		}

		mutex_lock(list->MutexHandle, OSI_WAIT_FOREVER);
		/* Release the temporary permission of previous node */
		CurrentNodePtr->ReadPermission--;
		CurrentNodePtr=CurrentNodePtr->NextNode;
		/* list rear judgment */
        if (CurrentNodePtr == list->EntriesList)
		{
            mutex_unlock(list->MutexHandle);
            break;
		}
        /* Get the temporary permission of current node */
        CurrentNodePtr->ReadPermission++;
		mutex_unlock(list->MutexHandle);
	}
	return ERROR;
}

/*
 *	Description:
 *  Find a node with index number which means find 1st, 2nd, 3rd or any number node.
 *
 *	Parameters:
 *  1. LIST_HANDLE HeadHandle [input] (non-empty handle), handle of list
 *  2. int INdexNum [input] (>=0), index number of node
 *  3. LIST_HANDLE NodeHandle [output] (non-empty handle), handle of returned node
 *  4. int PermissionTag [input] (READ_PERMISSION_TAG, WRITE_PERMISSION_TAG), Permission tag of matched node
 *	
 *  Return: 
 *	ERROR: When an error occur
 *	OK: Successfully return a matched node with the desired access permission.
 *	LIST_PERMISSION_DENY: The matched node refuses to endow the expected permission, 
 *	for example, the matched node is being writing by another process when the current process
 *	want to read or write it, or the matched node is being read by another process when the current
 *	process want to write it.
 */
int FindNodeNum(
                LIST_HANDLE HeadHandle,
                int IndexNum,
                LIST_HANDLE NodeHandle,
                int PermissionTag
                )
{
    LIST_NODE_PTR CurrentNodePtr;
    LIST_HEAD_PTR list;
    DS_HANDLE_ENTRY_PTR DSHeadHandleEntryPtr;
    DS_HANDLE_ENTRY_PTR DSNodeHandleEntryPtr;

    CurrentNodePtr=list=NULL;
    DSHeadHandleEntryPtr=DSNodeHandleEntryPtr=NULL;

    if (IsHandleEmpty(HeadHandle)==True || IsHandleEmpty(NodeHandle)==True || IndexNum<0)
    {
        printf("Parameter error!\n");
        return ERROR;
    }

    DSHeadHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
    DSNodeHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;

    if (DSHeadHandleEntryPtr->HandleType!=LIST_HEAD_TYPE || 
        DSNodeHandleEntryPtr->HandleType!=LIST_NODE_TYPE)
    {
        printf("Handle type error!\n");
        return ERROR;
    }

    list=(LIST_HEAD_PTR)DSHeadHandleEntryPtr->EntryPtr;

    mutex_lock(list->MutexHandle,OSI_WAIT_FOREVER);
    if (list->EntriesList)
    {
        CurrentNodePtr=(LIST_NODE_PTR)list->EntriesList;
    }
    else
    {
        mutex_unlock(list->MutexHandle);
        return ERROR;
    }
    /* Get a temporary read permission */
    CurrentNodePtr->ReadPermission++;
    mutex_unlock(list->MutexHandle);

    while (IndexNum--)
    {
        mutex_lock(list->MutexHandle,OSI_WAIT_FOREVER);
        /* Release the temporary read permission of current node */
        CurrentNodePtr->ReadPermission--;
        /* Pointer to the next node */
        CurrentNodePtr=CurrentNodePtr->NextNode;
        /* List rear judgment */
        if (CurrentNodePtr == list->EntriesList)
        {
            mutex_unlock(list->MutexHandle);
            break;
        }
        
        /* Get a temporary read permission of the next node */
        CurrentNodePtr->ReadPermission++;
        mutex_unlock(list->MutexHandle,OSI_WAIT_FOREVER);
    }

    if (CurrentNodePtr == list->EntriesList)
    {
        return ERROR;
    }

    mutex_lock(list->MutexHandle,OSI_WAIT_FOREVER);
    /* Release temporary read permission of current node */
    CurrentNodePtr->ReadPermission--;
    DSNodeHandleEntryPtr->EntryPtr=(void*)CurrentNodePtr;
    DSNodeHandleEntryPtr->HandleType=LIST_NODE_TYPE;
    switch (PermissionTag)
    {
    case READ_PERMISSION_TAG:
        if (CurrentNodePtr->WritePermission>0)
        {
            mutex_unlock(list->MutexHandle,OSI_WAIT_FOREVER);
            return LIST_PERMISSION_DENY;
        }
        CurrentNodePtr->ReadPermission++;
    	break;
    case WRITE_PERMISSION_TAG:
        if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)
        {
            mutex_unlock(list->MutexHandle,OSI_WAIT_FOREVER);
            return LIST_PERMISSION_DENY;
        }
        CurrentNodePtr->WritePermission++;
        break;
    default:
        break;
    }
    mutex_unlock(list->MutexHandle);

    return OK;
}

/*
 *	Description:
 *	Insert a node to the list.
 *	
 *  Parameters:
 *	1. LIST_HANDLE handle [input] (non-empty handle), handle of list.
 *  2. int DataLength [input] (>0), length of node data.
 *  3. void* DataPtr [input] (valid pointer), pointer to node data.
 * 
 *	Return:
 *  ERROR: When an error occur.
 *  OK: Successfully inserted.
 *  
 */
int InsertNodeHead(LIST_HANDLE handle,int DataLength,void* DataPtr)
{
	LIST_NODE_PTR CurrentNodePtr,InsertionNodePtr;
	LIST_HEAD_PTR list;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;

    CurrentNodePtr=InsertionNodePtr=NULL;
    list = NULL;
    DSHandleEntryPtr = NULL;

	if (IsHandleEmpty(handle)
        ||(DataPtr==NULL))
	{
		return ERROR;
	}
	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;

	if (DSHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		printf("List class error: DS handle type error!\n");
		return ERROR;
	}

	if ((InsertionNodePtr=(LIST_NODE_PTR)malloc(sizeof(LIST_NODE)))==0)
	{
		return ERROR;
	}

	memset(InsertionNodePtr,0,sizeof(LIST_NODE));
	InsertionNodePtr->data=DataPtr;
	InsertionNodePtr->length=DataLength;
	InsertionNodePtr->HeadPtr=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;

    list=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;
	mutex_lock(list->MutexHandle, OSI_WAIT_FOREVER);
	CurrentNodePtr=(LIST_NODE_PTR)list->EntriesList;

	if (CurrentNodePtr==0)	/* The first node of the list */
	{
		list->EntriesList=(void*)InsertionNodePtr;
		InsertionNodePtr->NextNode=InsertionNodePtr;
		InsertionNodePtr->PreviousNode=InsertionNodePtr;
	}
	else
	{
		InsertionNodePtr->PreviousNode=CurrentNodePtr->PreviousNode;
        InsertionNodePtr->PreviousNode->NextNode=InsertionNodePtr;
		/* Relink */
		InsertionNodePtr->NextNode=CurrentNodePtr;
		CurrentNodePtr->PreviousNode=InsertionNodePtr;
		/* mount */
		list->EntriesList=(void*)InsertionNodePtr;
	}
	/* write back */
	list->EntryAmount++;
	mutex_unlock(list->MutexHandle);

	return OK;
}

/*
 *	Description:
 *	Insert a node to list before or after a specific node which is indicated
 *  by parameter PosiNodeHandle.
 *	
 *	Parameters:
 *	1. LIST_HANDLE HeadHandle [input] (non-empty handle), handle of list.
 *  2. LIST_HANDLE PosiNodeHandle [input] (non-empty handle), handle of position node.
 *  3. int InsertPosition [input] (INSERT_BEFORE_NODE, INSERT_AFTER_NODE), insert position.
 *  4. int DataLength [input] (>0) Insertion node data length.
 *  5. void* DataPtr [input] (valid data pointer) node data.
 *	
 *	Return:
 *	ERROR: When an error occurs.
 *  OK: When successfully inserted a node.
 */
int InsertNodeHandle(
                     LIST_HANDLE HeadHandle, 
                     LIST_HANDLE PosiNodeHandle, 
                     int InsertPosition,
                     int DataLength, 
                     void* DataPtr
                     )
{
    LIST_HEAD_PTR ListHeadPtr;
    LIST_NODE_PTR ListNodePtr, CurrentNodePtr, InsertNodePtr;
    DS_HANDLE_ENTRY_PTR HandleEntryPtr1, HandleEntryPtr2;

    if (
        IsHandleEmpty(HeadHandle) || 
        IsHandleEmpty(PosiNodeHandle) || 
        DataLength<0 || 
        DataPtr==NULL
        )
    {
        printf("Parameters check fail!\n");
        return ERROR;
    }
    
    HandleEntryPtr1=(DS_HANDLE_ENTRY_PTR)HeadHandle;
    HandleEntryPtr2=(DS_HANDLE_ENTRY_PTR)PosiNodeHandle;

    if (HandleEntryPtr1->HandleType != LIST_HEAD_TYPE || 
        HandleEntryPtr2->HandleType != LIST_NODE_TYPE)
    {
        printf("Handle type error!\n");
        return ERROR;
    }

    ListHeadPtr=(LIST_HEAD_PTR)HandleEntryPtr1->EntryPtr;
    ListNodePtr=(LIST_NODE_PTR)HandleEntryPtr2->EntryPtr;
    
    if ((InsertNodePtr=(LIST_NODE_PTR)malloc(sizeof(LIST_NODE)))==NULL)
    {
        printf("Memory limited!\n");
        return ERROR;
    }

    memset(InsertNodePtr,0,sizeof(LIST_NODE));

    InsertNodePtr->data=DataPtr;
    InsertNodePtr->length=DataLength;
    InsertNodePtr->HeadPtr=ListHeadPtr;
    mutex_lock(ListHeadPtr->MutexHandle,OSI_WAIT_FOREVER);
    switch (InsertPosition)
    {
    case INSERT_BEFORE_NODE:
        if (ListNodePtr == ListHeadPtr->EntriesList)    /* Head position */
        {
            ListHeadPtr->EntriesList=(void*) InsertNodePtr;
            InsertNodePtr->PreviousNode=ListNodePtr->PreviousNode;
            InsertNodePtr->PreviousNode->NextNode=InsertNodePtr;
            InsertNodePtr->NextNode=ListNodePtr;
            ListNodePtr->PreviousNode=InsertNodePtr;
        }
        else /* Normal position */
        {
            CurrentNodePtr=ListNodePtr->PreviousNode;
            CurrentNodePtr->NextNode=InsertNodePtr;
            InsertNodePtr->PreviousNode=CurrentNodePtr;
            InsertNodePtr->NextNode=ListNodePtr;
            ListNodePtr->PreviousNode=InsertNodePtr;

        }
        break;
    case INSERT_AFTER_NODE:
        if (ListNodePtr->NextNode == ListHeadPtr->EntriesList) /* Rear position */
        {
            InsertNodePtr->PreviousNode=ListNodePtr;
            InsertNodePtr->NextNode=ListNodePtr->NextNode;
            InsertNodePtr->NextNode->PreviousNode=InsertNodePtr;

            ListNodePtr->NextNode=InsertNodePtr;
            
        }
        else /* Normal position */
        {
            CurrentNodePtr=ListNodePtr->NextNode;

            ListNodePtr->NextNode=InsertNodePtr;
            InsertNodePtr->PreviousNode=ListNodePtr;
            InsertNodePtr->NextNode=CurrentNodePtr;
            CurrentNodePtr->PreviousNode=InsertNodePtr;
        }

        break;
    default:
        printf("Insert position error!\n");
        mutex_unlock(ListHeadPtr->MutexHandle);
        free(InsertNodePtr);
        return ERROR;
        break;
    }

    ListHeadPtr->EntryAmount++;
    mutex_unlock(ListHeadPtr->MutexHandle);

    return OK;
}



/*
 *	Description:
 *	Delete a node from list with single word matching.
 *	
 *	Parameters:
 *	1. LIST_HANDLE HeadHandle [input] (non-empty handle), handle of list.
 *  2. void** KeyWordPtrArray [input] (valid pointer), pointer to keywords array.
 *	3. int CmpFcnIndex [input] (non-zero integer), comparison keyword mark bits.
 *  4. void** DelNodeDataPtr [output] (valid pointer), pointer points to deleted node.
 *      output for user processing.
 *  
 *	Return:
 *	ERROR: When an error occur or inexist expected node.
 *  OK: Successfully delete the expected node.
 *  LIST_PERMISSION_DENY: The matched node refuses to endow the expected permission, 
 *	for example, the matched node is being writing by another process when the current process
 *	want to read or write it, or the matched node is being read by another process when the current
 *	process want to write it.
 */
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

    HandleEntryPtr = NULL;
    ListHeadPtr = NULL;
    CurrentNodePtr=PreviousNodePtr=NextNodePtr=NULL;
    handle = NULL;
    DataPtr = NULL;
    retval = 0;

	/* parameters check */
	if (IsHandleEmpty(HeadHandle) || KeyWordPtrArray==NULL || CmpFcnIndex==0)
	{
		return ERROR;
	}
	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	if (HandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		return ERROR;
	}
    
    ListHeadPtr = HandleEntryPtr->EntryPtr;

	HandleEntry.HandleType=LIST_NODE_TYPE;
	handle=(LIST_HANDLE)&HandleEntry;

	retval=FindSW(HeadHandle,KeyWordPtrArray,CmpFcnIndex,handle,WRITE_PERMISSION_TAG);
	if (retval==ERROR || retval==LIST_PERMISSION_DENY)
	{
		return retval;
	}
    
	mutex_lock(ListHeadPtr->MutexHandle, OSI_WAIT_FOREVER);
	CurrentNodePtr=(LIST_NODE_PTR)HandleEntry.EntryPtr;

	if (CurrentNodePtr==NULL)
	{
		mutex_unlock(ListHeadPtr->MutexHandle);
		return ERROR;
	}

	CurrentNodePtr->WritePermission--;	/* Release write permission */
	if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)
	{
		mutex_unlock(ListHeadPtr->MutexHandle);
		return ERROR;
	}

	PreviousNodePtr=CurrentNodePtr->PreviousNode;
	NextNodePtr=CurrentNodePtr->NextNode;
	if (CurrentNodePtr == ListHeadPtr->EntriesList)	/* First node */
	{
        if (CurrentNodePtr->NextNode == CurrentNodePtr) /* Only one node */
        {
            ListHeadPtr->EntriesList=NULL;
        }
        else
        {
            ListHeadPtr->EntriesList=CurrentNodePtr->NextNode;
            CurrentNodePtr->NextNode->PreviousNode=CurrentNodePtr->PreviousNode;
            CurrentNodePtr->PreviousNode->NextNode=CurrentNodePtr->NextNode;
        }
		
	}
	else
	{
        CurrentNodePtr->PreviousNode->NextNode=CurrentNodePtr->NextNode;
        CurrentNodePtr->NextNode->PreviousNode=CurrentNodePtr->PreviousNode;
	}

	DataPtr=CurrentNodePtr->data;
	ListHeadPtr->EntryAmount--;
	mutex_unlock(ListHeadPtr->MutexHandle);
	
    if (CurrentNodePtr!=NULL)
	{
		free(CurrentNodePtr);
		CurrentNodePtr=NULL;
	}

	*DelNodeDataPtr=(void*)DataPtr;

	return OK;
}

/*
 *	Description:
 *	Delete a node from list with multiple word matching.
 *	
 *	Parameters:
 *	1. LIST_HANDLE HeadHandle [input] (non-empty handle), handle of list.
 *  2. void** KeyWordPtrArray [input] (valid pointer), pointer to keywords array.
 *	3. int CmpFcnIndex [input] (non-zero integer), comparison keyword mark bits.
 *  4. void** DelNodeDataPtr [output] (valid pointer), pointer points to deleted node.
 *      output for user processing.
 *  
 *	Return:
 *	ERROR: When an error occur or inexist expected node.
 *  OK: Successfully delete the expected node.
 *  LIST_PERMISSION_DENY: The matched node refuses to endow the expected permission, 
 *	for example, the matched node is being writing by another process when the current process
 *	want to read or write it, or the matched node is being read by another process when the current
 *	process want to write it.
 */
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

    HandleEntryPtr = NULL;
    ListHeadPtr = NULL;
    CurrentNodePtr=PreviousNodePtr=NextNodePtr=NULL;
    handle = NULL;
    DataPtr = NULL;
    retval = 0;

    /* parameters check */
    if (IsHandleEmpty(HeadHandle) || KeyWordPtrArray==NULL || CmpFcnIndex==0)
    {
        return ERROR;
    }
    HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
    if (HandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
    {
        return ERROR;
    }

    ListHeadPtr = HandleEntryPtr->EntryPtr;

    HandleEntry.HandleType=LIST_NODE_TYPE;
    handle=(LIST_HANDLE)&HandleEntry;

    retval=FindMW(HeadHandle,KeyWordPtrArray,CmpFcnIndex,handle,WRITE_PERMISSION_TAG);
    if (retval==ERROR || retval==LIST_PERMISSION_DENY)
    {
        return retval;
    }

    mutex_lock(ListHeadPtr->MutexHandle, OSI_WAIT_FOREVER);
    CurrentNodePtr=(LIST_NODE_PTR)HandleEntry.EntryPtr;

    if (CurrentNodePtr==NULL)
    {
        mutex_unlock(ListHeadPtr->MutexHandle);
        return ERROR;
    }

    CurrentNodePtr->WritePermission--;	/* Release write permission */
    if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)
    {
        mutex_unlock(ListHeadPtr->MutexHandle);
        return ERROR;
    }

    PreviousNodePtr=CurrentNodePtr->PreviousNode;
    NextNodePtr=CurrentNodePtr->NextNode;
    if (CurrentNodePtr == ListHeadPtr->EntriesList)	/* First node */
    {
        if (CurrentNodePtr->NextNode == CurrentNodePtr) /* Only one node */
        {
            ListHeadPtr->EntriesList=NULL;
        }
        else
        {
            ListHeadPtr->EntriesList=CurrentNodePtr->NextNode;
            CurrentNodePtr->NextNode->PreviousNode=CurrentNodePtr->PreviousNode;
            CurrentNodePtr->PreviousNode->NextNode=CurrentNodePtr->NextNode;
        }

    }
    else
    {
        CurrentNodePtr->PreviousNode->NextNode=CurrentNodePtr->NextNode;
        CurrentNodePtr->NextNode->PreviousNode=CurrentNodePtr->PreviousNode;
    }

    DataPtr=CurrentNodePtr->data;
    ListHeadPtr->EntryAmount--;
    mutex_unlock(ListHeadPtr->MutexHandle);

    if (CurrentNodePtr!=NULL)
    {
        free(CurrentNodePtr);
        CurrentNodePtr=NULL;
    }

    *DelNodeDataPtr=(void*)DataPtr;

    return OK;
}

/*
 *	Description:
 *	Delete all the free nodes of the list. When the routine meets a permission obtained
 *  node, it will skip to the next free node for deletion. It will not delete list head.
 *	
 *	Parameters:
 *  LIST_HANDLE handle [input] (non-empty handle), handle of list.	
 *
 *	
 *	Return:
 *	ERROR: When an error occur
 *  EntryAmount: left nodes amount.
 */
int DeleteNodes( LIST_HANDLE handle )
{
	LIST_NODE_PTR	CurrentNodePtr,PreviousNodePtr,LastNodePtr;
	LIST_HEAD_PTR list;
	DS_HANDLE_ENTRY_PTR DSHandleEntryPtr;
    int EntryAmount;

    CurrentNodePtr=PreviousNodePtr=NULL;
    list = NULL;
    DSHandleEntryPtr = NULL;

	if (IsHandleEmpty(handle))
	{
		return ERROR;
	}
	DSHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
	if (DSHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		printf("List class error: DS handle type error!\n");
		return ERROR;
	}

	list=(LIST_HEAD_PTR)DSHandleEntryPtr->EntryPtr;

	mutex_lock(list->MutexHandle, OSI_WAIT_FOREVER);

    if (list->EntriesList == NULL || list->EntryAmount <=0 )    /* Empty or invalid list */
    {
        mutex_unlock(list->MutexHandle);
        return list->EntryAmount;
    }

	CurrentNodePtr=PreviousNodePtr=(LIST_NODE_PTR)list->EntriesList;
    LastNodePtr=CurrentNodePtr->PreviousNode;

	while (1)
	{
        if (CurrentNodePtr==LastNodePtr)    /* Last node */
        {
            if (CurrentNodePtr->ReadPermission==0 && CurrentNodePtr->WritePermission==0) /* Delete last node */
            {
                if (CurrentNodePtr->NextNode == CurrentNodePtr) /* Last unique node */
                {
                    free(CurrentNodePtr);
                    list->EntriesList=NULL;
                    list->EntryAmount--;
                }
                else
                {
                    CurrentNodePtr->PreviousNode->NextNode=CurrentNodePtr->NextNode;
                    CurrentNodePtr->NextNode->PreviousNode=CurrentNodePtr->PreviousNode;
                    free(CurrentNodePtr);
                    list->EntryAmount--;
                }
            }
            break;
        }

        if (CurrentNodePtr->ReadPermission>0 || CurrentNodePtr->WritePermission>0)	/* Jump to next node */
        {
            PreviousNodePtr=CurrentNodePtr;
            CurrentNodePtr=CurrentNodePtr->NextNode;
        }
        else /* Delete current node */ 
        {
            if (CurrentNodePtr==list->EntriesList)	/* First node */
            {
                list->EntriesList=CurrentNodePtr->NextNode;
                CurrentNodePtr->NextNode->PreviousNode=CurrentNodePtr->PreviousNode;
                CurrentNodePtr->PreviousNode->NextNode=CurrentNodePtr->NextNode;
                free(CurrentNodePtr);
                list->EntryAmount--;
                CurrentNodePtr=PreviousNodePtr=list->EntriesList; /* Reset pointers */
            }	
            else									/* Normal node */
            {

                PreviousNodePtr->NextNode=CurrentNodePtr->NextNode;
                CurrentNodePtr->NextNode->PreviousNode=PreviousNodePtr;
                free(CurrentNodePtr);
                list->EntryAmount--;
                PreviousNodePtr=CurrentNodePtr;
                CurrentNodePtr=PreviousNodePtr->NextNode;
            }
        }
	}
    
    EntryAmount=list->EntryAmount;
	mutex_unlock(list->MutexHandle);

	return EntryAmount;
}

/*
 *	Description:
 *	This routine attempts to delete entire list including list head. It will
 *  first invoke "DeleteNodes" routine to attempt delete all the nodes. If 
 *  this attempt fail, it will return immediately with left nodes amount. It it
 *  successfully delete all the nodes, it will delete list head.
 *	
 *	Parameters:
 *	1. LIST_HANDLE handle [input] (non-empty handle), handle of list.
 *	
 *	Return:
 *	ERROR: When an error occurs.
 *  Left nodes amount: left nodes amount of list.
 *  OK: When this routine successfully deletes all the nodes and list head.
 */

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
        list->MutexHandle=NULL;
    }

    if (list->CmpFcnArray)
    {
        free(list->CmpFcnArray);
        list->CmpFcnArray=NULL;
    }
 
    free(list);

    DSHandleEntryPtr->EntryPtr=NULL;

    return OK;
}

/*
 *	Description:
 *	Release specific permission of a node. Actually just reduce 1 count of specific 
 *  permission tag.
 *	
 *	Parameters:
 *	LIST_HANDLE NodeHandle [input] (non-empty handle), handle of list.
 *  int PermissionTag [input] (READ_PERMISSION_TAG, WRITE_PERMISSION_TAG), permission tag of node.
 *	
 *	Return:
 *	ERROR: When an error occurs.
 *  OK: Successfully release specific permission tag of the node.
 */
int ReleasePermission(LIST_HANDLE NodeHandle,int PermissionTag)
{
	LIST_NODE_PTR			NodePtr;
	DS_HANDLE_ENTRY_PTR	HandleEntryPtr;

    NodePtr = NULL;
    HandleEntryPtr = NULL;

	if (IsHandleEmpty(NodeHandle))
	{
		return ERROR;
	}

	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	if (HandleEntryPtr->HandleType!=LIST_NODE_TYPE)
	{
		return ERROR;
	}

	if (HandleEntryPtr->EntryPtr)
	{
		NodePtr=(LIST_NODE_PTR)HandleEntryPtr->EntryPtr;
	}
	else
	{
		return ERROR;
	}

	mutex_lock(NodePtr->HeadPtr->MutexHandle, OSI_WAIT_FOREVER);
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
	return OK;
}

/*
 *	Description:
 *	Print permission tags.
 *	
 *	Parameters:
 *	LIST_HANDLE NodeHandle [input] (non-empty handle), handle of node.
 *	
 *	Return:
 *	ERROR: When an error occurs.
 *  OK: Successfully printed the permission tag.
 */
int PrintPermission(LIST_HANDLE NodeHandle)
{
	LIST_NODE_PTR			NodePtr;
	DS_HANDLE_ENTRY_PTR	HandleEntryPtr;

    NodePtr = NULL;
    HandleEntryPtr = NULL;

	if (IsHandleEmpty(NodeHandle)==True)
	{
		return ERROR;
	}
	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	if (HandleEntryPtr->HandleType!=LIST_NODE_TYPE)
	{
		return ERROR;
	}
	NodePtr=(LIST_NODE_PTR)HandleEntryPtr->EntryPtr;
	
	mutex_lock(NodePtr->HeadPtr->MutexHandle, OSI_WAIT_FOREVER);
    printf("Read Permission Tag:%d\n", NodePtr->ReadPermission);
    printf("Write Permission Tag:%d\n", NodePtr->WritePermission);
    mutex_unlock(NodePtr->HeadPtr->MutexHandle);

	return OK;
}

/*
 *	Description:
 *	Get the specific permission tag of one node. Actually just count plus 1
 *  on specific permission tag.
 *	
 *	Parameters:
 *	1. LIST_HANDLE NodeHandle [input] (non-empty handle), handle of a node.
 *  2. int PermissionTag [input] (READ_PERMISSION_TAG, WRITE_PERMISSION_TAG), permission tag of a node
 *	
 *	Return:
 *	ERROR: When an error occurs.
 *  OK: Successfully obtained specific permission.
 *  LIST_PERMISSION_DENY: The specific node refuses to endow the expected permission, 
 *	for example, the specific node is being writing by another process when the current process
 *	want to get read or write permission, or the specific node is being read 
 *  by another process when the current process want to get write permission.
 *  However current process can successfully get read permission when the node
 *  has been obtained read permission by another process.
 *	
 */
int GetPermisssion(LIST_HANDLE NodeHandle,int PermissionTag)
{
	LIST_NODE_PTR			NodePtr;
	DS_HANDLE_ENTRY_PTR	HandleEntryPtr;

    NodePtr = NULL;
    HandleEntryPtr = NULL;

	if (IsHandleEmpty(NodeHandle)==True)
	{
		return ERROR;
	}
	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	if (HandleEntryPtr->HandleType!=LIST_NODE_TYPE)
	{
		return ERROR;
	}
	NodePtr=(LIST_NODE_PTR)HandleEntryPtr->EntryPtr;

    mutex_lock(NodePtr->HeadPtr->MutexHandle, OSI_WAIT_FOREVER);
	switch(PermissionTag)
	{
	case READ_PERMISSION_TAG:
		if (NodePtr->WritePermission>0)
		{
			mutex_unlock(NodePtr->HeadPtr->MutexHandle);
			return LIST_PERMISSION_DENY;
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
			return LIST_PERMISSION_DENY;
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
	
	return OK;
}

/*
 *	Description:
 *	Get real node data of a handle.
 *	
 *	Parameters:
 *	LIST_HANDLE NodeHandle [input] (non-empty handle), handle of node.
 *	
 *	Return:
 *  LIST_NULL: When the handle is invalid, empty or handle type error.
 *  Valid pointer: pointer to node data.
 */
void* GetData( LIST_HANDLE NodeHandle )
{
	LIST_NODE_PTR			NodePtr;
	LIST_HEAD_PTR				ListHeadPtr;
	DS_HANDLE_ENTRY_PTR		HandleEntryPtr;

    NodePtr = NULL;
    ListHeadPtr = NULL;
    HandleEntryPtr = NULL;

	if (IsHandleEmpty(NodeHandle)==True)
	{
		return NULL;
	}

	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NodeHandle;
	switch(HandleEntryPtr->HandleType)
	{
	case LIST_NODE_TYPE:
		NodePtr=(LIST_NODE_PTR)HandleEntryPtr->EntryPtr;
		return NodePtr->data;
	case LIST_HEAD_TYPE:
		return NULL;
	default:
	    break;
	}

	return NULL;
}

/*
 *	Description:
 *	Get next node from current node. Currently if the next node's critical permission
 *  is obtained by another process, this routine just return permission deny. This
 *  routine will return the next node with specific permission obtained. User can 
 *  freely access the returned next node with specific permission. 
 *	
 *	Parameters:
 *	1. LIST_HANDLE HeadHandle [input] (non-empty handle), handle of list.
 *  2. LIST_HANDLE CurrentHandle [input] (non-empty handle), handle of current node.
 *  3. LIST_HANDLE NextHandle [output] (valid empty handle), handle of next node.
 *  4. int SkipTag [input] (0, 1), Skip tag, "0" indicates routine will return permission deny when
 *      the next node's permission failed to be obtained, "1" indicates routine will skip to the next node when
 *      permission deny happen.
 *	5. int PermissionTag [input] (READ_PERMISSION_TAG, WRITE_PERMISSION_TAG), permission tag.
 *
 *	Return:
 *  1. ERROR: When an error occurs.
 *  2. OK: successfully get next node with specific permission.
 *  3. LIST_PERMISSION_DENY: The specific node refuses to endow the expected permission. 
 *	For example, the next node is being writing by another process when the current process
 *	want to get read or write permission, or the next node is being read 
 *  by another process when the current process want to get write permission.
 *  However current process can successfully get read permission when the next node
 *  has been obtained read permission by another process.
 *		
 */
int GetNextNode(
				LIST_HANDLE HeadHandle,
				LIST_HANDLE CurrentHandle, 
				LIST_HANDLE NextHandle,
                int SkipTag,
				int PermissionTag
				)
{
	LIST_NODE_PTR		NextNodePtr, CurrentNodePtr;
	LIST_HEAD_PTR		HeadPtr;
	DS_HANDLE_ENTRY_PTR	HeadHandleEntryPtr,CurrentHandleEntryPtr,NextHandleEntryPtr;

    CurrentNodePtr=NextNodePtr=NULL;
    HeadPtr = NULL;
    HeadHandleEntryPtr=NULL;
    HeadHandleEntryPtr=NULL;
    NextHandleEntryPtr=NULL;

	if (IsHandleEmpty(HeadHandle) || IsHandleEmpty(CurrentHandle) || NextHandle==NULL)
	{
		return ERROR;
	}
	
	HeadHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
	if (HeadHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
	{
		return ERROR;
	}

    HeadPtr= (LIST_HEAD_PTR) HeadHandleEntryPtr->EntryPtr;
    mutex_lock(HeadPtr->MutexHandle, OSI_WAIT_FOREVER);
    if (HeadPtr->EntriesList == NULL) /* Empty list */
    {
        mutex_unlock(HeadPtr->MutexHandle);
        return ERROR;
    }

	CurrentHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)CurrentHandle;
	NextHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)NextHandle;
    /* Get Next node */
    if (CurrentHandleEntryPtr->HandleType == LIST_HEAD_TYPE && 
        CurrentHandleEntryPtr->EntryPtr == HeadHandleEntryPtr->EntryPtr) /* Current node is the head of the list */
    {
        CurrentNodePtr=(LIST_NODE_PTR)HeadPtr->EntriesList;
        NextNodePtr=(LIST_NODE_PTR)HeadPtr->EntriesList;
    }
    else if (CurrentHandleEntryPtr->HandleType == LIST_NODE_TYPE)
    {
        CurrentNodePtr=(LIST_NODE_PTR)CurrentHandleEntryPtr->EntryPtr;
        NextNodePtr=CurrentNodePtr->NextNode;
    }
    
    do
    {
        if (PermissionTag == READ_PERMISSION_TAG)
        {
            if (NextNodePtr->WritePermission == 0) /* Successfully obtained next node permission */
            {
                NextNodePtr->ReadPermission++;
                NextHandleEntryPtr->HandleType=LIST_NODE_TYPE;
                NextHandleEntryPtr->EntryPtr=NextNodePtr;
                mutex_unlock(HeadPtr->MutexHandle);
                return OK;
            }
            else /* Pointer to next node */
            {
                if (SkipTag == 1)
                {
                    NextNodePtr=NextNodePtr->NextNode;
                }
                else
                {
                    mutex_unlock(HeadPtr->MutexHandle);
                    return LIST_PERMISSION_DENY;
                }
            }
        }
        else if (PermissionTag == WRITE_PERMISSION_TAG)
        {
            if (NextNodePtr->ReadPermission == 0 && NextNodePtr->WritePermission == 0)
            {
                NextNodePtr->WritePermission++;
                NextHandleEntryPtr->HandleType=LIST_NODE_TYPE;
                NextHandleEntryPtr->EntryPtr=NextNodePtr;
                mutex_unlock(HeadPtr->MutexHandle);
                return OK;
            }
            else
            {
                if (SkipTag==1)
                {
                    NextNodePtr=NextNodePtr->NextNode; /* Jump to the next node */
                }
                else
                {
                    mutex_unlock(HeadPtr->MutexHandle);
                    return LIST_PERMISSION_DENY;
                }
            }
        }
        else
        {
            printf("Permission tag error!\n");
            mutex_unlock(HeadPtr->MutexHandle);
            return ERROR;
        }

    }while (NextNodePtr != CurrentNodePtr);
	
	mutex_unlock(HeadPtr->MutexHandle);
	return ERROR;
}

/*
 *	Description:
 *	Get previous node from current node. Currently if the previous node's critical permission
 *  is obtained by another process, this routine just return permission deny. This
 *  routine will return the previous node with specific permission obtained. User can 
 *  freely access the returned previous node with specific permission. 
 *	
 *	Parameters:
 *	1. LIST_HANDLE HeadHandle [input] (non-empty handle), handle of list.
 *  2. LIST_HANDLE CurrentHandle [input] (non-empty handle), handle of current node.
 *  3. LIST_HANDLE PreviousHandle [output] (valid empty handle), handle of next node.
 *  4. int SkipTag [input] (0, 1), Skip tag, "0" indicates routine will return permission deny when
 *      the previous node's permission failed to be obtained, "1" indicates routine will skip to the previous node when
 *      permission deny happen.
 *	5. int PermissionTag [input] (READ_PERMISSION_TAG, WRITE_PERMISSION_TAG), permission tag.
 *
 *	Return:
 *  1. ERROR: When an error occurs.
 *  2. OK: successfully get previous node with specific permission.
 *  3. LIST_PERMISSION_DENY: The specific node refuses to endow the expected permission. 
 *	For example, the previous node is being writing by another process when the current process
 *	want to get read or write permission, or the previous node is being read 
 *  by another process when the current process want to get write permission.
 *  However current process can successfully get read permission when the previous node
 *  has been obtained read permission by another process.
 * 		
 */
int GetPreviousNode(
				LIST_HANDLE HeadHandle,
				LIST_HANDLE CurrentHandle, 
				LIST_HANDLE PreviousHandle,
                int SkipTag,
				int PermissionTag
				)
{
    LIST_NODE_PTR		PreviousNodePtr, CurrentNodePtr;
    LIST_HEAD_PTR		HeadPtr;
    DS_HANDLE_ENTRY_PTR	HeadHandleEntryPtr,CurrentHandleEntryPtr,PreviousHandleEntryPtr;

    CurrentNodePtr=PreviousNodePtr=NULL;
    HeadPtr = NULL;
    HeadHandleEntryPtr=NULL;
    HeadHandleEntryPtr=NULL;
    PreviousHandleEntryPtr=NULL;

    if (IsHandleEmpty(HeadHandle) || IsHandleEmpty(CurrentHandle) || PreviousHandle==NULL)
    {
        return ERROR;
    }

    HeadHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)HeadHandle;
    if (HeadHandleEntryPtr->HandleType!=LIST_HEAD_TYPE)
    {
        return ERROR;
    }
    
    HeadPtr= (LIST_HEAD_PTR) HeadHandleEntryPtr->EntryPtr;
    mutex_lock(HeadPtr->MutexHandle, OSI_WAIT_FOREVER);
    if (HeadPtr->EntriesList == NULL) /* Empty list */
    {
        mutex_unlock(HeadPtr->MutexHandle);
        return ERROR;
    }
    
    CurrentHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)CurrentHandle;
    PreviousHandleEntryPtr=(DS_HANDLE_ENTRY_PTR)PreviousHandle;
    /* Get previous node */
    if (CurrentHandleEntryPtr->HandleType == LIST_HEAD_TYPE && 
        CurrentHandleEntryPtr->EntryPtr == HeadHandleEntryPtr->EntryPtr) /* Current node is the head of the list */
    {
        CurrentNodePtr=(LIST_NODE_PTR)HeadPtr->EntriesList;
        PreviousNodePtr=(LIST_NODE_PTR)HeadPtr->EntriesList;
    }
    else if (CurrentHandleEntryPtr->HandleType == LIST_NODE_TYPE)
    {
        CurrentNodePtr=(LIST_NODE_PTR)CurrentHandleEntryPtr->EntryPtr;
        PreviousNodePtr=CurrentNodePtr->PreviousNode;
    }

    do
    {
        if (PermissionTag == READ_PERMISSION_TAG)
        {
            if (PreviousNodePtr->WritePermission == 0) /* Successfully obtained previous node permission */
            {
                PreviousNodePtr->ReadPermission++;
                PreviousHandleEntryPtr->HandleType=LIST_NODE_TYPE;
                PreviousHandleEntryPtr->EntryPtr=PreviousNodePtr;
                mutex_unlock(HeadPtr->MutexHandle);
                return OK;
            }
            else /* Pointer to previous node */
            {
                if (SkipTag == 1)
                {
                    PreviousNodePtr=PreviousNodePtr->PreviousNode;
                }
                else
                {
                    mutex_unlock(HeadPtr->MutexHandle);
                    return LIST_PERMISSION_DENY;
                }
            }
        }
        else if (PermissionTag == WRITE_PERMISSION_TAG)
        {
            if (PreviousNodePtr->ReadPermission == 0 && PreviousNodePtr->WritePermission == 0)
            {
                PreviousNodePtr->WritePermission++;
                PreviousHandleEntryPtr->HandleType=LIST_NODE_TYPE;
                PreviousHandleEntryPtr->EntryPtr=PreviousNodePtr;
                mutex_unlock(HeadPtr->MutexHandle);
                return OK;
            }
            else
            {
                if (SkipTag==1)
                {
                    PreviousNodePtr=PreviousNodePtr->PreviousNode; /* Jump to the previous node */
                }
                else
                {
                    mutex_unlock(HeadPtr->MutexHandle);
                    return LIST_PERMISSION_DENY;
                }
            }
        }
        else
        {
            printf("Permission tag error!\n");
            mutex_unlock(HeadPtr->MutexHandle);
            return ERROR;
        }

    }while (PreviousNodePtr != CurrentNodePtr);

    mutex_unlock(HeadPtr->MutexHandle);
    return ERROR;
}


/*
 *	Description:
 *	Get data length of the data block stored in a node.
 *	
 *	Parameters:
 *	LIST_HANDLE handle [input] (non-empty handle), handle of a node.
 *	
 *	Return:
 *	ERROR: When an error occurs.
 *  Data length: When the node includes valid data block.
 */
int GetDataLen(LIST_HANDLE handle)
{
	DS_HANDLE_ENTRY_PTR	HandleEntryPtr;
	LIST_NODE_PTR NodePtr;

    HandleEntryPtr = NULL;
    NodePtr = NULL;

	if (handle==NULL)
	{
		return ERROR;
	}

	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
	if (HandleEntryPtr->HandleType==LIST_HEAD_TYPE)
	{
		return ERROR;
	}

	NodePtr=(LIST_NODE_PTR)HandleEntryPtr->EntryPtr;
	if (NodePtr==NULL)
	{
		return ERROR;
	}
	return NodePtr->length;
}

/*
 *	Description:
 *	Judge whether a handle includes valid node or not.
 *	
 *	Parameters:
 *	LIST_HANDLE handle [input] (valid pointer), handle of a head or node.
 *	
 *	Return:
 *	True: when current handle pointer is null or handle is empty.
 *  False: when current handle includes valid node data.
 */
LOGICAL IsHandleEmpty(LIST_HANDLE handle)
{
	DS_HANDLE_ENTRY_PTR	HandleEntryPtr;

    HandleEntryPtr = NULL;

	if (handle==NULL)
	{
		return True;
	}
	HandleEntryPtr=(DS_HANDLE_ENTRY_PTR)handle;
    return (HandleEntryPtr->EntryPtr==NULL)?True:False;
}

/*
 *	Description:
 *	handle data copy, this routine just copy handle data, it does not copy 
 *  the entry data which is represented by the handle.
 *	
 *	Parameters:
 *	LIST_HANDLE DesHandle [input] (valid pointer), handle of destination entry.
 *  const LIST_HANDLE SrcHandle [input] (non-empty handle), handle of source entry.
 *  
 *	
 *	Return:
 *	void	
 */
void HandleCpy(LIST_HANDLE DesHandle,const LIST_HANDLE SrcHandle)
{
	if (DesHandle==NULL || IsHandleEmpty(SrcHandle))
	{
		return;
	}
	memcpy(DesHandle,SrcHandle,sizeof(DS_HANDLE_ENTRY));
    return;
}
