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
#include "text.h"


typedef struct  
{
	int line_len;
	char* line_ptr;
}LINE_NODE,*LINE_NODE_PTR;



LIST_HANDLE read_text(STRING text_filename, void* line_cmp_fun)
{
    FILE* fp;
	char* line_ptr;
	int line_len;
	int nread,count;
	LINE_NODE line_node;
	LIST_HANDLE handle;
	void* cmp_funs[1];

    if (text_filename == NULL)
    {
        return NULL;
    }

    handle=NULL;

    fp=fopen(text_filename,"r");
    if (fp == NULL)
    {
        printf("Open text file error!\n");
        return NULL;
    }
    
	/* Create data base */
	if ((handle=CreateHandle(LIST_HEAD_TYPE))==NULL)
	{
		goto ErrExit;
	}

	cmp_funs[0]=(void*)line_cmp_fun;
	if (Initialize(1,cmp_funs,handle)==ERROR)
	{
		DeleteHandle(handle);
		goto ErrExit;
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
            fclose(fp);
			return handle;
		}
		line_ptr=NULL;
		line_len=0;
	}
    
    fclose(fp);
	return handle;

ErrExit:
    
    fclose(fp);

    return NULL;
}

int find_keyword(
              LIST_HANDLE text_handle, /* A handle represents the target text. */
              STRING keyword, /* Keyword for match */
              STRING_POSITION_PTR str_posi_ptr,
              int PermissionTag /* Permission tag */
              )
{
    void* keyword_ptr_array[1];
    int cmp_index, matched_index,retval;
    LIST_HANDLE line_node_handle; /* A handle represents matched line */

    if (str_posi_ptr==NULL || IsHandleEmpty(text_handle))
    {
        printf("Parameter error!\n");
        return ERROR;
    }
    
    line_node_handle=str_posi_ptr->line_node_handle;
    if (IsHandleEmpty(line_node_handle))
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
        line_node_handle,PermissionTag,(void*)&matched_index);

    if (retval < 0)
    {
        printf("Find keyword error!\n");
        return ERROR;
    }
    
    str_posi_ptr->line_num=retval;
    str_posi_ptr->char_index=matched_index;

    return OK;
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
        ReleasePermission(line_handle,WRITE_PERMISSION_TAG);
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
    STRING_POSITION str_posi;

    if (IsHandleEmpty(text_handle) || keyword==NULL || line_str==NULL)
    {
        printf("Parameter error!\n");
        return ERROR;
    }

    if ((line_handle=CreateHandle(LIST_NODE_TYPE))==NULL)
    {
        return ERROR;
    }
    str_posi.line_node_handle=line_handle;
    if (find_keyword(text_handle,keyword,&str_posi,WRITE_PERMISSION_TAG)==ERROR)
    {
        printf("Find keyword error!\n");
        return ERROR;
    }

    if ((line_node_ptr=(LINE_NODE_PTR)GetData(line_handle))==NULL)
    {
        ReleasePermission(line_handle,WRITE_PERMISSION_TAG);
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
    LINE_NODE_PTR line_node_ptr;
    STRING new_line;
    int cplen, retval;
    STRING_POSITION str_posi;
    
    line_node_ptr=line_handle=new_line=NULL;

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
    str_posi.line_node_handle=line_handle;
    if (find_keyword(text_handle,keyword,&str_posi,READ_PERMISSION_TAG)==ERROR)
    {
        printf("Cannot find keyword!\n");
        return ERROR;
    }
    
    if ((line_node_ptr=(LINE_NODE_PTR)malloc(sizeof(LINE_NODE)))==NULL)
    {
        ReleasePermission(line_handle,READ_PERMISSION_TAG);
        printf("Memory limited!\n");
        return ERROR;
    }

    if ((new_line=(STRING)malloc(length+3))==NULL)
    {
        printf("Memory limited!\n");
        goto ErrExit;
    }

    cplen=min(strlen(ins_line),length);

    memcpy(new_line,ins_line,cplen);

    new_line[cplen]='\r';
    new_line[cplen+1]='\n';
    new_line[cplen+2]='\0';

    line_node_ptr->line_ptr=new_line;
    line_node_ptr->line_len=cplen+3;
    
    retval=InsertNodeHandle(text_handle,line_handle,ins_posi,sizeof(LINE_NODE),(void*)line_node_ptr);
    
    if (retval == ERROR)
    {
        printf("Insert node to list error!\n");
        goto ErrExit;
    }

    ReleasePermission(line_handle,READ_PERMISSION_TAG);
    return OK;

ErrExit:

    if (line_node_ptr != NULL)
    {
        free(line_node_ptr);
    }

    if (line_node_ptr != NULL)
    {
        free(line_node_ptr);
    }
    ReleasePermission(line_handle,READ_PERMISSION_TAG);
    return ERROR;
}

int text_save(STRING config_filename, LIST_HANDLE text_handle)
{
    FILE* fp;
    LIST_HANDLE handle, handle1, handle2;
    int amount, retval;
    LINE_NODE_PTR line_node_ptr;

    if (config_filename == NULL)
    {
        return ERROR;
    }

    fp=fopen(config_filename,"w");

    if (fp==NULL)
    {
        printf("Cannot open config file!\n");
        return ERROR;
    }

    handle1=CreateHandle(LIST_NODE_TYPE);
    handle2=CreateHandle(LIST_NODE_TYPE);

    HandleCpy(handle1,text_handle);
    amount=Count(text_handle);

    while (amount>0)
    {
        retval=GetNextNode(text_handle,handle1,handle2,0,READ_PERMISSION_TAG);
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

    fclose(fp);

    return OK;
}

static int line_kw_cmp_fun(void* node_data_ptr, void* keyword, void* userdata)
{
    LINE_NODE_PTR line_node_ptr;
    STRING keywordstr;
    int index, * index_ptr;


    line_node_ptr=(LINE_NODE_PTR)node_data_ptr;
    keywordstr=(STRING) keyword;

    index=findstr(line_node_ptr->line_ptr,keywordstr);
    
    if (index == ERROR)
    {
        return ERROR;
    }

    index_ptr=(int*)userdata;

    *index_ptr=index;

    return OK;
}
