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
	int line_num;
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
	if ((handle=CreateHandle(LIST_HEAD_TYPE))==LIST_NULL)
	{
		return NULL;
	}

	cmp_funs[0]=(void*)line_cmp_fun;
	if (Initialize(1,cmp_funs,handle)==LIST_ERROR)
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
		line_node.line_num=count++;
		if (InsertNode(handle,sizeof(line_node),&line_node)==LIST_ERROR)
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
    case LIST_ERROR:
        printf("Cannot find match!\n");
        return ERROR;
    case PERMISSION_DENY:
        printf("Permission delay!\n");
        return ERROR;
        break;
    case LIST_OK:
        return matched_index;
        break;
    default:
        break;
    }
    
    return ERROR;
}

int replace_line_kw(
                 LIST_HANDLE text_handle,
                 STRING keyword,
                 STRING line_str,
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

    if ((line_handle=CreateHandle(LIST_NODE_TYPE))==LIST_NULL)
    {
        return ERROR;
    }
    
    if (find_keyword(text_handle,keyword,line_handle,WRITE_PERMISSION_TAG)==ERROR)
    {
        printf("Find keyword error!\n");
        return ERROR;
    }

    if ((line_node_ptr=(LINE_NODE_PTR)GetData(line_handle))==LIST_NULL)
    {
        printf("Error: Empty node!\n");
        return ERROR;
    }
    
    /* Return old line for outside process */
    *old_line=line_node_ptr->line_ptr;
    line_node_ptr->line_ptr=line_str;

    ReleasePermission(line_handle,WRITE_PERMISSION_TAG);
    DeleteHandle(line_handle);

    return OK;
}


