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


static int line_kw_cmp_fun(void* node_data_ptr, void* keyword, void* userdata);

LIST_HANDLE read_text(STRING text_filename)
{
    FILE* fp;
	char* line_ptr;
	int line_len;
	int nread,count;
	LINE_NODE line_node;
	LIST_HANDLE handle;
	void* cmp_funs[1];

    if (text_filename == AII_NULL)
    {
        return AII_NULL;
    }

    handle=AII_NULL;

    fp=fopen(text_filename,"r");
    if (fp == AII_NULL)
    {
        printf("Open text file error!\n");
        return AII_NULL;
    }
    
	/* Create data base */
	if ((handle=CreateHandle(LIST_HEAD_TYPE))==AII_NULL)
	{
		goto ErrExit;
	}

	cmp_funs[0]=(void*)line_kw_cmp_fun;
	if (Initialize(handle,1,cmp_funs)==AII_ERROR)
	{
		DeleteHandle(handle);
		goto ErrExit;
	}
	
	fseek(fp,0,SEEK_SET);
	line_ptr=AII_NULL;
	line_len=0;
	count=0;
	while ((nread=getline(&line_ptr,&line_len,fp)) != AII_ERROR)
	{
		line_node.line_ptr=line_ptr;
		line_node.line_len=nread;
		if (InsertNodeHead(handle,sizeof(line_node),&line_node)==AII_ERROR)
		{
			printf("Insert text line error, maybe memory limited!\n");
            fclose(fp);
			return handle;
		}
		line_ptr=AII_NULL;
		line_len=0;
	}
    
    fclose(fp);
	return handle;

ErrExit:
    
    fclose(fp);
    return AII_NULL;
}

int find_keyword(
              LIST_HANDLE text_handle, /* A handle represents the target text. */
              STRING keyword, /* Keyword for match */
              MATCH_WORD_POSITION_PTR str_posi_ptr,
              int permi_tag /* Permission tag */
              )
{
    void* keyword_ptr_array[1];
    int cmp_index, matched_index,retval;
    LIST_HANDLE line_node_handle; /* A handle represents matched line */

    if (str_posi_ptr==AII_NULL || IsHandleEmpty(text_handle))
    {
        printf("Parameter error!\n");
        return AII_ERROR;
    }
    
    line_node_handle=str_posi_ptr->line_node_handle;
    if (IsHandleEmpty(line_node_handle))
    {
        printf("Empty handle!\n");
        return AII_ERROR;
    }
    
    if (keyword==AII_NULL)
    {
        printf("No keyword for match!\n");
        return AII_ERROR;
    }
    
    keyword_ptr_array[0]=(void*)keyword;
    cmp_index=0x1;
    retval=FindSW(text_handle,keyword_ptr_array,cmp_index,
        line_node_handle,permi_tag,(void*)&matched_index);

    if (retval < 0)
    {
        printf("Find keyword error!\n");
        return AII_ERROR;
    }
    
    str_posi_ptr->line_num=retval;
    str_posi_ptr->word_index_begin=matched_index;
    str_posi_ptr->word_index_end=matched_index+strlen(keyword)-1;
    str_posi_ptr->permi_tag=permi_tag;
    return AII_OK;
}

int find_line_num(
                  LIST_HANDLE text_handle, /* A handle represents the target text. */
                  int line_num, /* line number */
                  LIST_HANDLE line_node, /* A handle represents matched line */
                  int permi_tag /* Permission tag */
                  )
{
    int retval;

    if (IsHandleEmpty(text_handle) || IsHandleEmpty(line_node) || line_num<0)
    {
        printf("Parameter error!\n");
        return AII_ERROR;
    }

    retval=FindNodeNum(text_handle,line_num,line_node,permi_tag);
    
    switch (retval)
    {
    case AII_ERROR:
        printf("Cannot find specific line!\n");
        return AII_ERROR;
        break;
    case LIST_PERMISSION_DENY:
        printf("Permission deny!\n");
        return AII_ERROR;
        break;
    case AII_OK:
        return AII_OK;
        break;
    default:
        break;
    }

    return AII_ERROR;
}

int replace_line_num(
                     LIST_HANDLE text_handle,
                     int line_num,
                     STRING new_line_str
                     )
{
    LIST_HANDLE line_handle;
    LINE_NODE_PTR line_node_ptr;
    int new_line_len, old_line_len;

    if (IsHandleEmpty(text_handle) || line_num < 0 || new_line_str == AII_NULL)
    {
        printf("Parameter error!\n");
        return AII_ERROR;
    }

    if ((line_handle=CreateHandle(LIST_NODE_TYPE))==AII_NULL)
    {
        return AII_ERROR;
    }

    if (find_line_num(text_handle,line_num,line_handle,WRITE_PERMISSION_TAG)==AII_ERROR)
    {
        printf("Find line with number tag error!\n");
        return AII_ERROR;
    }

    if ((line_node_ptr=(LINE_NODE_PTR)GetData(line_handle))==AII_NULL)
    {
        ReleasePermission(line_handle,WRITE_PERMISSION_TAG);
        printf("Error: Empty node!\n");
        return AII_ERROR;
    }
    
    new_line_len=strlen(new_line_str)+1;    /* For extra '\r' char */
    old_line_len=strlen(line_node_ptr->line_ptr);   /* Must already include '\r' char */

    if (new_line_len > old_line_len)
    {
        line_node_ptr->line_ptr=(STRING)realloc(line_node_ptr->line_ptr, new_line_len+1); /* For extra '\0' char */ 
    }
    
    memcpy(line_node_ptr->line_ptr,new_line_str,new_line_len-1);
    line_node_ptr->line_ptr[new_line_len-1]='\r';
    line_node_ptr->line_ptr[new_line_len]='\0';
    line_node_ptr->line_len=new_line_len;

    ReleasePermission(line_handle,WRITE_PERMISSION_TAG);
    DeleteHandle(line_handle);

    return AII_OK;
}

int replace_line_kw(
                 LIST_HANDLE text_handle,
                 STRING keyword,
                 STRING new_line_str
                 )
{
    LIST_HANDLE line_handle;
    LINE_NODE_PTR line_node_ptr;
    MATCH_WORD_POSITION str_posi;
	STRING old_line_str;
    int retval, old_line_len, new_line_len;

    if (IsHandleEmpty(text_handle) || new_line_str==AII_NULL)
    {
        return AII_ERROR;
    }

    line_handle=CreateHandle(LIST_NODE_TYPE);
    if (line_handle==AII_NULL)
    {
        return AII_ERROR;
    }

    str_posi.line_node_handle=line_handle;
    retval=find_keyword(text_handle,keyword,&str_posi,WRITE_PERMISSION_TAG);
    if (retval==AII_ERROR)
    {
        printf("Cannot find keyword!\n");
        DeleteHandle(line_handle);
        return AII_ERROR;
    }
    
    if ((line_node_ptr=(LINE_NODE_PTR)GetData(line_handle))==AII_NULL)
    {
        printf("Error: Empty node!\n");
        DeleteHandle(line_handle);
        return AII_ERROR;
    }
    
    old_line_str=line_node_ptr->line_ptr;
    old_line_len=line_node_ptr->line_len;

    new_line_len=strlen(new_line_str)+1;    /* For extra '\r' char */
    if (new_line_len > old_line_len)
    {
        line_node_ptr->line_ptr=(STRING) realloc(line_node_ptr->line_ptr,new_line_len+1); /* For extra '\0' char */
    }
    
    memcpy(line_node_ptr->line_ptr,new_line_str, new_line_len-1);
    line_node_ptr->line_ptr[new_line_len-1]='\r';
    line_node_ptr->line_ptr[new_line_len]='\0';
    line_node_ptr->line_len=new_line_len;

    ReleasePermission(line_handle,WRITE_PERMISSION_TAG);
    DeleteHandle(line_handle);
    return AII_OK;
}

int replace_kw(
               LIST_HANDLE text_handle,
               STRING keyword,
               STRING new_string
               )
{
    LIST_HANDLE line_handle;
    LINE_NODE_PTR line_node_ptr;
    MATCH_WORD_POSITION str_posi;
    int retval, new_line_length,wib,wie,old_line_len,new_string_len;
    STRING old_line_str,new_line_str;

    if (IsHandleEmpty(text_handle) || new_string==AII_NULL)
    {
        return AII_ERROR;
    }


    line_handle=CreateHandle(LIST_NODE_TYPE);
    if (line_handle==AII_NULL)
    {
        return AII_ERROR;
    }

    str_posi.line_node_handle=line_handle;
    retval=find_keyword(text_handle,keyword,&str_posi,WRITE_PERMISSION_TAG);
    if (retval==AII_ERROR)
    {
        printf("Cannot find keyword!\n");
        DeleteHandle(line_handle);
        return AII_ERROR;
    }

    if ((line_node_ptr=(LINE_NODE_PTR)GetData(line_handle))==AII_NULL)
    {
        printf("Error: Empty node!\n");
        DeleteHandle(line_handle);
        return AII_ERROR;
    }

    old_line_str=line_node_ptr->line_ptr;
    old_line_len=line_node_ptr->line_len;
    wib=str_posi.word_index_begin;
    wie=str_posi.word_index_end;
    
    new_string_len=strlen(new_string);
    new_line_length=old_line_len-(wie-wib+1)+new_string_len;
    if (new_line_length > old_line_len)
    {
        if (realloc(line_node_ptr->line_ptr,new_line_length)==AII_NULL)
        {
            printf("Cannot reallocate enough space for new line!\n");
            DeleteHandle(line_handle);
            return AII_ERROR;
        }
    }
    new_line_str=line_node_ptr->line_ptr;
    memcpy(new_line_str+wib,new_string,new_string_len);
    memcpy(new_line_str+wib+new_string_len,old_line_str+wie+1,old_line_len-wie-1);
    new_line_str[new_line_length]='\0';
    
    line_node_ptr->line_len=new_line_length;
    
    DeleteHandle(line_handle);
    return AII_OK;
}

int insert_line(
             LIST_HANDLE text_handle,
             STRING keyword,
             STRING ins_line,
             int ins_posi
             )
{
    LIST_HANDLE line_handle;
    LINE_NODE_PTR line_node_ptr;
    STRING new_line;
    int retval, ins_length;
    MATCH_WORD_POSITION str_posi;
    
    line_node_ptr=line_handle=new_line=AII_NULL;

    if (IsHandleEmpty(text_handle) || 
        keyword==AII_NULL || ins_line==AII_NULL)
    {
        printf("Parameters error!\n");
        return AII_ERROR;
    }

    line_handle=CreateHandle(LIST_NODE_TYPE);

    if (line_handle==AII_NULL)
    {
        return AII_ERROR;
    }
    str_posi.line_node_handle=line_handle;
    if (find_keyword(text_handle,keyword,&str_posi,READ_PERMISSION_TAG)==AII_ERROR)
    {
        printf("Cannot find keyword!\n");
        return AII_ERROR;
    }
    
    if ((line_node_ptr=(LINE_NODE_PTR)malloc(sizeof(LINE_NODE)))==AII_NULL)
    {
        ReleasePermission(line_handle,READ_PERMISSION_TAG);
        printf("Memory limited!\n");
        return AII_ERROR;
    }
    
    ins_length=strlen(ins_line);

    if ((new_line=(STRING)malloc(ins_length+2))==AII_NULL) /* For extra '\r' '\0' chars */
    {
        printf("Memory limited!\n");
        goto ErrExit;
    }

    memcpy(new_line,ins_line,ins_length);

    new_line[ins_length]='\r';
    new_line[ins_length+1]='\0';

    line_node_ptr->line_ptr=new_line;
    line_node_ptr->line_len=ins_length+1;
    
    retval=InsertNodeHandle(text_handle,line_handle,ins_posi,sizeof(LINE_NODE),(void*)line_node_ptr);
    
    if (retval == AII_ERROR)
    {
        printf("Insert node to list error!\n");
        goto ErrExit;
    }

    ReleasePermission(line_handle,READ_PERMISSION_TAG);
    return AII_OK;

ErrExit:

    if (line_node_ptr != AII_NULL)
    {
        free(line_node_ptr);
    }

    if (line_node_ptr != AII_NULL)
    {
        free(line_node_ptr);
    }
    ReleasePermission(line_handle,READ_PERMISSION_TAG);
    return AII_ERROR;
}

int text_save(STRING config_filename, LIST_HANDLE text_handle)
{
    FILE* fp;
    LIST_HANDLE handle1, handle2;
    int amount, retval;
    LINE_NODE_PTR line_node_ptr;

    if (config_filename == AII_NULL)
    {
        return AII_ERROR;
    }

    fp=fopen(config_filename,"w");

    if (fp==AII_NULL)
    {
        printf("Cannot open config file!\n");
        return AII_ERROR;
    }

    handle1=CreateHandle(LIST_NODE_TYPE);
    handle2=CreateHandle(LIST_NODE_TYPE);

    HandleCpy(handle1,text_handle);
    amount=Count(text_handle);

    while (amount>0)
    {
        retval=GetNextNode(text_handle,handle1,handle2,0,READ_PERMISSION_TAG);
        if (retval == AII_ERROR || retval == LIST_PERMISSION_DENY)
        {
            break;
        }
        
        ReleasePermission(handle1,READ_PERMISSION_TAG);
        
        if ((line_node_ptr=(LINE_NODE_PTR)GetData(handle2))==AII_NULL)
        {
            printf("Empty line!\n");
            HandleCpy(handle1,handle2);
            amount--;
            continue;
        }

        if (line_node_ptr->line_ptr != AII_NULL)
        {
            /* Write text */
            if (fwrite(line_node_ptr->line_ptr,line_node_ptr->line_len,1,fp)<0)
            {
                printf("Write text error!\n");
                break;
            }
            
            if (fwrite("\n",1,1,fp) < 0)
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

    return AII_OK;
}

static int line_kw_cmp_fun(void* node_data_ptr, void* keyword, void* userdata)
{
    LINE_NODE_PTR line_node_ptr;
    STRING keywordstr;
    int index, * index_ptr;


    line_node_ptr=(LINE_NODE_PTR)node_data_ptr;
    keywordstr=(STRING) keyword;

    index=findstr(line_node_ptr->line_ptr,keywordstr);
    
    if (index == AII_ERROR)
    {
        return AII_ERROR;
    }
	
	if (userdata)
	{
		index_ptr=(int*)userdata;
		*index_ptr=index;
	}
    return AII_OK;
}
