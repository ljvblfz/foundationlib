/************************************************************************/
/*	Copyright to HIKVISION.Inc
Solution designed by Ben Shaw.
Programmed by Ben Shaw.
$Reversion 1.0 $ $Data: 2007/6/19 $ 
*/
/************************************************************************/

#ifndef _LIST_METHOD
#define _LIST_METHOD

#include "libtype.h"

#define LIST_HEAD_TYPE	1
#define LIST_NODE_TYPE 2

#define READ_PERMISSION_TAG		0
#define WRITE_PERMISSION_TAG	1

#define PERMISSION_DENY			-2

#define LIST_ERROR              -1
#define LIST_OK                 0
#define LIST_NULL               ((void*) 0)

typedef void*   LIST_HANDLE;
/*
*	Create a handle for specific handle type
*	The handle is just for representing the head or a common node
*/
LIST_HANDLE CreateHandle(int HandleType);
/*
*	Release the allocated memory block for storing the handle.
*/
void DeleteHandle(LIST_HANDLE handle);

/*
*	Initialize the data structure
*/
int Initialize( 
               int CmpFcnAmount, /* Keyword compare functions amount [in] */
               void** CmpFcnArray, /* Keyword compare function array [in] */
               LIST_HANDLE handle	/* The handle of the data structure [in] */
               );
/* Count the node amount of the data structure */
int Count(LIST_HANDLE handle);
/* Judge whether the data structure is empty or not */
int IsEmpty(LIST_HANDLE handle);
/* Find a specific node with single keyword match */
int FindSW( 
           LIST_HANDLE HeadHandle, /* The handle of the data structure head [in] */
           void** KeyWordPtrArray,	/* The keyword array [in] */
           int CmpFcnIndex,		/* Compare function tag [in] */
           LIST_HANDLE NodeHandle, /* The handle of matched node [out] */
           int PermissionTag /* Permission Tag [in] */
           );

/* Find a specific node with multiple keyword match */
int FindMW( 
           LIST_HANDLE HeadHandle, /* The handle of the data structure head [in] */
           void** KeyWordPtrArray, /* The keyword array [in] */
           int CmpFcnIndex, /* Compare function tag [in] */
           LIST_HANDLE NodeHandle, /* The handle of matched node [out] */
           int PermissionTag /* Permission Tag [in] */
           );

int InsertNode(
               LIST_HANDLE HeadHandle, /* The handle of the head [in] */
               int length,	/* The length of the user data [in] */
               void* DataPtr /* The point to data block [in] */
               );

/* Delete node with single keyword match */
int DeleteNodeSW(
                 LIST_HANDLE HeadHandle,	/* The handle of the head [in] */
                 void** KeyWordPtrArray,	/* keyword array [in] */
                 int CmpFcnIndex,			/* Compare function index tag [in] */
                 void** DelNodePtr		/* Deletion node pointer [out] */
                 );
/* Delete node with multiple keyword match */
int DeleteNodeMW(
                 LIST_HANDLE HeadHandle,	/* The handle of the head [in] */
                 void** KeyWordPtrArray,	/* keyword array [in] */
                 int CmpFcnIndex,			/* Compare function index tag [in] */
                 void** DelNodePtr		/* Deletion node pointer [out] */
                 );
/* Delete the entire data structure */
int DeleteNodes(LIST_HANDLE handle);
/* Release the specific permission of a node */
int ReleasePermission(LIST_HANDLE NodeHandle,int PermissionTag);
/* Get the specific permission of a node */
int GetPermisssion(LIST_HANDLE NodeHandle,int PermissionTag);
/* Get the pointer to the user data block */
void* GetData( LIST_HANDLE NodeHandle );

/* Get the next node from current node */
int GetNextNode( 
                LIST_HANDLE HeadHandle, /* Head handle [in] */
                LIST_HANDLE handle,	/* Current node handle [in] */
                LIST_HANDLE NextHandle, /* The next node handle [out] */
                int PermissionTag /* Permission tag [in] */
                );
int GetPreviousNode( 
                    LIST_HANDLE HeadHandle, 
                    LIST_HANDLE handle, 
                    LIST_HANDLE PreviousHandle, 
                    int PermissionTag 
                    );

/* Judge whether the handle is empty or not */
LOGICAL IsHandleEmpty(LIST_HANDLE handle);

/* Handle copy */
void HandleCpy(
               LIST_HANDLE DesHandle,	/* Destination handle */
               const LIST_HANDLE SrcHandle	/* Source handle */
               );
int GetDataLen(LIST_HANDLE handle);
int PrintPermission(LIST_HANDLE NodeHandle,int PermissionTag);

int Delete(LIST_HANDLE handle);

#endif

