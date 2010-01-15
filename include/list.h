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

#ifndef _LIST_METHOD
#define _LIST_METHOD

#include "libtype.h"

#define LIST_HEAD_TYPE	1
#define LIST_NODE_TYPE 2

#define READ_PERMISSION_TAG		0
#define WRITE_PERMISSION_TAG	1

#define LIST_PERMISSION_DENY			-2

#ifndef OSI_ERROR
#define OSI_ERROR              -1
#endif

#ifndef OSI_OK
#define OSI_OK                 0
#endif

#ifndef NULL
#define NULL               ((void*) 0)
#endif

typedef void*   LIST_HANDLE;
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
LIST_HANDLE CreateHandle(int HandleType);
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
void DeleteHandle(LIST_HANDLE handle);
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
*	Function Execution Status, (OSI_OK, OSI_ERROR)
*/
int Initialize(
               LIST_HANDLE handle,
               int	CmpFcnAmount,
               void** CmpFcnArray
               );
/*
*	Description:
*	Function "Count" returns the entry amount of list.
*		
*	Parameters:
*	1. LIST_HANDLE handle [input] (non-empty handle), the handle of list head
*	Return:
*	Function Execution Status, (OSI_ERROR, OSI_OK).
*/
int Count(LIST_HANDLE handle);

/*
*	Description:
*	Check whether a list is empty or not.
*		
*	Parameters:
*	1. LIST_HANDLE HeadHandle [input] (valid pointer)
*
*	Return:
*	OSI_ERROR if an error occur. False if the list is not empty, True if 
*	the list is empty. 
*/
int IsEmpty(LIST_HANDLE HeadHandle);

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
*	1. OSI_ERROR: When a error occur
*	2. LIST_PERMISSION_DENY: The matched node refuses to endow the expected permission, 
*	for example, the matched node is being writing by another process when the current process
*	want to read or write it, or the matched node is being read by another process when the current
*	process want to write it.
*  3. >0 Successfully find specific node and return the index of node.
*/
int FindSW(
           LIST_HANDLE HeadHandle,
           void** KeyWordPtrArray,
           int CmpFcnIndex,
           LIST_HANDLE NodeHandle,
           int PermissionTag,
           void* UserData
           );

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
*	1. OSI_ERROR: When an error occur
*	2. PERMISSION_DENY: The matched node refuses to endow the expected permission, 
*	for example, the matched node is being writing by another process when the current process
*	want to read or write it, or the matched node is being read by another process when the current
*	process want to write it.
*  3. >0 Successfully find specific node and return the index of node.
*/
int FindMW(
           LIST_HANDLE HeadHandle,
           void** KeyWordPtrArray,
           int CmpFcnIndex,
           LIST_HANDLE NodeHandle,
           int PermissionTag,
           void** UserDataArray
           );

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
*	OSI_ERROR: When an error occur
*	OSI_OK: Successfully return a matched node with the desired access permission.
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
                );

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
 *  OSI_ERROR: When an error occur.
 *  OSI_OK: Successfully inserted.
 *  
 */
int InsertNodeHead(LIST_HANDLE handle,int DataLength,void* DataPtr);

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
*	OSI_ERROR: When an error occurs.
*  OSI_OK: When successfully inserted a node.
*/
#define INSERT_BEFORE_NODE  0
#define INSERT_AFTER_NODE   1
int InsertNodeHandle( LIST_HANDLE HeadHandle, 
                     LIST_HANDLE PosiNodeHandle, 
                     int InsertPosition, 
                     int DataLength, void* DataPtr );
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
*	OSI_ERROR: When an error occur or inexist expected node.
*  OSI_OK: Successfully delete the expected node.
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
                 );

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
*	OSI_ERROR: When an error occur or inexist expected node.
*  OSI_OK: Successfully delete the expected node.
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
                 );

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
*	OSI_ERROR: When an error occur
*  EntryAmount: left nodes amount.
*/
int DeleteNodes( LIST_HANDLE handle );

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
*	OSI_ERROR: When an error occurs.
*  Left nodes amount: left nodes amount of list.
*  OSI_OK: When this routine successfully deletes all the nodes and list head.
*/

int Delete(LIST_HANDLE handle);

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
*	OSI_ERROR: When an error occurs.
*  OSI_OK: Successfully release specific permission tag of the node.
*/
int ReleasePermission(LIST_HANDLE NodeHandle,int PermissionTag);

/*
*	Description:
*	Print permission tags.
*	
*	Parameters:
*	LIST_HANDLE NodeHandle [input] (non-empty handle), handle of node.
*	
*	Return:
*	OSI_ERROR: When an error occurs.
*  OSI_OK: Successfully printed the permission tag.
*/
int PrintPermission(LIST_HANDLE NodeHandle);

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
*	OSI_ERROR: When an error occurs.
*  OSI_OK: Successfully obtained specific permission.
*  LIST_PERMISSION_DENY: The specific node refuses to endow the expected permission, 
*	for example, the specific node is being writing by another process when the current process
*	want to get read or write permission, or the specific node is being read 
*  by another process when the current process want to get write permission.
*  However current process can successfully get read permission when the node
*  has been obtained read permission by another process.
*	
*/
int GetPermisssion(LIST_HANDLE NodeHandle,int PermissionTag);

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
void* GetData( LIST_HANDLE NodeHandle );

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
*  1. OSI_ERROR: When an error occurs.
*  2. OSI_OK: successfully get next node with specific permission.
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
                );
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
*  1. OSI_ERROR: When an error occurs.
*  2. OSI_OK: successfully get previous node with specific permission.
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
                    );
/*
*	Description:
*	Get data length of the data block stored in a node.
*	
*	Parameters:
*	LIST_HANDLE handle [input] (non-empty handle), handle of a node.
*	
*	Return:
*	OSI_ERROR: When an error occurs.
*  Data length: When the node includes valid data block.
*/
int GetDataLen(LIST_HANDLE handle);

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
LOGICAL IsHandleEmpty(LIST_HANDLE handle);

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
void HandleCpy(LIST_HANDLE DesHandle,const LIST_HANDLE SrcHandle);

#endif

