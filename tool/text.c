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

LIST_HANDLE read_text_line(FILE* fp)
{
	char* line_ptr;
	size_t line_len;
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

	cmp_funs[0]=(void*)line_str_cmp;
	if (Initialize(1,cmp_funs,handle)==ERROR)
	{
		DeleteHandle(handle);
		return NULL;
	}
	
	fseek(fp,0,SEEK_SET);
	line_ptr=NULL;
	line_len=0;
	count=0;
	while ((nread=getline(&line_ptr,&line_len,fp)) != ERROR)
	{
		line_node.line_ptr=line_ptr;
		line_node.line_len=nread;
		line_node.line_num=count++;
		if (InsertNode(handle,sizeof(line_node),&line_node)==ERROR)
		{
			printf("Insert text line error, maybe memory limited!\n");
			return handle;
		}
		line_ptr=NULL;
		line_len=0;
	}

	return handle;
}

static int line_str_cmp(void* node_data_ptr, void* key_word_ptr, void* userdata)
{
	LINE_NODE_PTR line_node_ptr;
	STRING key_str;
	int index, *iptr;

	line_node_ptr=(LINE_NODE_PTR)node_data_ptr;
	key_str=(STRING)key_word_ptr;
	index=findstr(line_node_ptr->line_ptr,key_word_ptr);
	if (index==ERROR)
	{
		return LIST_ERROR;
	}

	iptr=(int*)userdata;
	*iptr=index;

	return LIST_OK;
}

