/*
*	text.c
*	text modification tool C code file.
*	
*	Infrastructure, Text Analysis Module.
*	Based on SDK5.RC16, Linux 2.6.20, ARM Host CPU.
*	
*	MG3500 (MAXIM, mobilygen, Inc.) HD project
*   Copyright (c) Artificial Intelligence Infinity
*
*	2009.08.06 Solution designed by shaobin, Programmed by shaobin.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tool.h"
#include "list.h"


typedef struct  
{
	int line_len;
	char* line_ptr;
}LINE_NODE,*LINE_NODE_PTR;


LIST_HANDLE read_text(FILE* fp, void* line_cmp_fun)
{
	char* line_ptr;
	int line_len;
	int nread,count;
	LINE_NODE line_node;
	LIST_HANDLE handle;
	void* cmp_funs[1];

	if (fp==NULL)
	{
		return NULL;
	}
	/* Create data base */
	if ((handle=CreateHandle(LIST_HEAD_TYPE))==NULL)
	{
		return NULL;
	}

	cmp_funs[0]=(void*)line_cmp_fun;
	if (Initialize(1,cmp_funs,handle)==ERROR)
	{
		DeleteHandle(handle);
		return NULL;
	}
	
	fseek(fp,0,SEEK_SET);
	line_ptr=NULL;
	line_len=0;
	count=0;
	while ((nread=getline_all(&line_ptr,&line_len,fp)) != ERROR)
	{
		line_node.line_ptr=line_ptr;
		line_node.line_len=nread;
		if (InsertNodeHead(handle,sizeof(line_node),&line_node)==ERROR)
		{
			printf("Insert text line error, maybe memory limited!\n");
			return handle;
		}
		line_ptr=NULL;
		line_len=0;
	}

	return handle;
}

static int find_keyword(
              LIST_HANDLE text_handle, /* A handle represents the target text. */
              STRING keyword, /* Keyword for match */
              LIST_HANDLE line_node, /* A handle represents matched line */
              int PermissionTag /* Permission tag */
              )
{
    void* keyword_ptr_array[1];
    int cmp_index, matched_index,retval;

    if (IsHandleEmpty(text_handle) || IsHandleEmpty(line_node))
    {
        printf("Empty handle!\n");
        return ERROR;
    }

    if (keyword==NULL)
    {
        printf("No keyword for match!\n");
        return ERROR;
    }
    
    keyword_ptr_array[0]=(void*)keyword;
    cmp_index=0x1;
    retval=FindSW(text_handle,keyword_ptr_array,cmp_index,
        line_node,PermissionTag,(void*)&matched_index);

    switch (retval)
    {
    case ERROR:
        printf("Cannot find match!\n");
        return ERROR;
    case LIST_PERMISSION_DENY:
        printf("Permission deny!\n");
        return ERROR;
        break;
    case OK:
        return matched_index;
        break;
    default:
        break;
    }
    
    return ERROR;
}

int find_line_num(
                  LIST_HANDLE text_handle, /* A handle represents the target text. */
                  int line_num, /* line number */
                  LIST_HANDLE line_node, /* A handle represents matched line */
                  int PermissionTag /* Permission tag */
                  )
{
    int retval;

    if (IsHandleEmpty(text_handle) || IsHandleEmpty(line_node) || line_num<0)
    {
        printf("Parameter error!\n");
        return ERROR;
    }

    retval=FindNodeNum(text_handle,line_num,line_node,PermissionTag);
    
    switch (retval)
    {
    case ERROR:
        printf("Cannot find specific line!\n");
        return ERROR;
        break;
    case LIST_PERMISSION_DENY:
        printf("Permission deny!\n");
        return ERROR;
        break;
    case OK:
        return OK;
        break;
    default:
        break;
    }

    return ERROR;
}

int replace_line_num(
                     LIST_HANDLE text_handle,
                     int line_num,
                     STRING line_str,
                     int line_len,
                     STRING* old_line
                     )
{
    LIST_HANDLE line_handle;
    LINE_NODE_PTR line_node_ptr;

    if (IsHandleEmpty(text_handle) || line_num<0 || line_str=NULL)
    {
        printf("Parameter error!\n");
        return ERROR;
    }

    if ((line_handle=CreateHandle(LIST_NODE_TYPE))==NULL)
    {
        return ERROR;
    }

    if (find_line_num(text_handle,line_num,line_handle,WRITE_PERMISSION_TAG)==ERROR)
    {
        printf("Find line with number tag error!\n");
        return ERROR;
    }

    if ((line_node_ptr=(LINE_NODE_PTR)GetData(line_handle))==NULL)
    {
        printf("Error: Empty node!\n");
        return ERROR;
    }
    
    /* Return old line for outside process */

    *old_line=line_node_ptr->line_ptr;
    line_node_ptr->line_ptr=line_str;
    line_node_ptr->line_len=line_len;

    ReleasePermission(line_handle,WRITE_PERMISSION_TAG);
    DeleteHandle(line_handle);

    return OK;
}

int replace_line_kw(
                 LIST_HANDLE text_handle,
                 STRING keyword,
                 STRING line_str,
                 int line_len,
                 STRING* old_line
                 )
{
    LIST_HANDLE line_handle;
    LINE_NODE_PTR line_node_ptr;

    if (IsHandleEmpty(text_handle) || keyword==NULL || line_str==NULL)
    {
        printf("Parameter error!\n");
        return ERROR;
    }

    if ((line_handle=CreateHandle(LIST_NODE_TYPE))==NULL)
    {
        return ERROR;
    }
    
    if (find_keyword(text_handle,keyword,line_handle,WRITE_PERMISSION_TAG)==ERROR)
    {
        printf("Find keyword error!\n");
        return ERROR;
    }

    if ((line_node_ptr=(LINE_NODE_PTR)GetData(line_handle))==NULL)
    {
        printf("Error: Empty node!\n");
        return ERROR;
    }
    
    /* Return old line for outside process */
    *old_line=line_node_ptr->line_ptr;
    line_node_ptr->line_ptr=line_str;
    line_node_ptr->line_len=line_len;

    ReleasePermission(line_handle,WRITE_PERMISSION_TAG);
    DeleteHandle(line_handle);

    return OK;
}

int ins_line(
             LIST_HANDLE text_handle,
             STRING keyword,
             STRING ins_line,
             int length,
             int ins_posi
             )
{
    LIST_HANDLE line_handle;

    if (IsHandleEmpty(text_handle) || 
        keyword==NULL || ins_line==NULL || length < 0)
    {
        printf("Parameters error!\n");
        return ERROR;
    }

    line_handle=CreateHandle(LIST_NODE_TYPE);

    if (line_handle==NULL)
    {
        return ERROR;
    }

    if (find_keyword(text_handle,keyword,line_handle,READ_PERMISSION_TAG)==ERROR)
    {
        printf("Cannot find keyword!\n");
        return ERROR;
    }


}

int text_save(FILE* fp, LIST_HANDLE text_handle)
{
    LIST_HANDLE handle, handle1, handle2;
    int amount, retval;
    LINE_NODE_PTR line_node_ptr;

    handle1=CreateHandle(LIST_NODE_TYPE);
    handle2=CreateHandle(LIST_NODE_TYPE);

    HandleCpy(handle1,text_handle);
    amount=Count(text_handle);

    while (amount>0)
    {
        retval=GetNextNode(text_handle,handle1,handle2,READ_PERMISSION_TAG);
        if (retval == ERROR || retval == LIST_PERMISSION_DENY)
        {
            break;
        }
        
        ReleasePermission(handle1,READ_PERMISSION_TAG);
        
        if ((line_node_ptr=GetData(handle2))==ERROR)
        {
            printf("Empty line!\n");
            HandleCpy(handle1,handle2);
            amount--;
            continue;
        }

        if (line_node_ptr->line_ptr != NULL)
        {
            /* Write text */
            if (fwrite(line_node_ptr->line_ptr,line_node_ptr->line_len,1,fp)<0)
            {
                printf("Write text error!\n");
                break;
            }
        }
        amount--;
        HandleCpy(handle1,handle2);
    }
    ReleasePermission(handle1,READ_PERMISSION_TAG);

    DeleteHandle(handle1);
    DeleteHandle(handle2);

    return OK;
}

