/*
*	configPars.c
*	Config file pars C code file.
*	This code file include functions pars linux config file.
*	
*	Foundation Services, Tool Module.
*	Based on SDK5.RC16, Linux 2.6.20, ARM Host CPU.
*	
*	MG3500 (MAXIM, mobilygen, Inc.) HD project
*  Copyright (c) 1992-2009,Zhejiang Dahua Technology Stock Co. Ltd.
*
*	2009.07.24 Solution designed by shaobin, Programmed by shaobin.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tool.h"
#include "list.h"

#define MAX_CONFIG_CHAR_NUM		128

typedef struct  
{
	int		line_num;
	char	config_parameter[MAX_CONFIG_CHAR_NUM];
	char	config_value[MAX_CONFIG_CHAR_NUM];
}LINUX_CONFIG_ENTRY,*LINUX_CONFIG_ENTRY_PTR;

static config_parameter_cmp(void* node_data_ptr, void* key_word_ptr);


LIST_HANDLE read_config_file(FILE* rfp)
{
	char* line_ptr;
	size_t line_len;
	int nread,count,i;
	LINUX_CONFIG_ENTRY config_entry;
	LIST_HANDLE handle;
	void* cmp_funs[1];

	if (rfp==NULL)
	{
		return NULL;
	}
	/* Create data base */
	if ((handle=CreateHandle(LIST_HEAD_TYPE))==NULL)
	{
		return NULL;
	}

	cmp_funs[0]=(void*)config_parameter_cmp;
	if (Initialize(1,cmp_funs,handle)==ERROR)
	{
		DeleteHandle(handle);
		return NULL;
	}

	fseek(rfp,0,SEEK_SET);
	line_ptr=NULL;
	line_len=0;
	count=1;
	while ((nread=getline(&line_ptr,&line_len,rfp)) != ERROR)
	{
		i=0;
		
		while (line_ptr[i]==' ' || line_ptr[i]=='\t' && i++ < line_len);
		
		if (line_ptr[i]=='\n' || line_ptr[i]=='#')
		{
			free(line_ptr);
			continue;
		}

		if (pars_line(line_ptr,"=",&config_entry)==ERROR)
		{
			printf("Warning: line %d configure format error!\n")
		}
		
		if (InsertNode(handle,sizeof(config_entry),&config_entry)==ERROR)
		{
			printf("Insert text line error, maybe memory limited!\n");
			free(line_ptr);
			return handle;
		}

		free(line_ptr);
		line_ptr=NULL;
		line_len=0;
	}

	return handle;
}

/*
 *	Function: pars_line
 *	Description: Pars one line string of Linux config file.
 *	Input:
 *			line_str: line string
 *			keyword: match keyword
 *	Output:
 *			cfentry_ptr: configuration entry pointer.
 *	Return:
 *			OK, ERROR.
 */
static int pars_line(
					 STRING line_str, /* [in] Line string*/
					 STRING keyword,  /* [in] */
					 LINUX_CONFIG_ENTRY_PTR cfentry_ptr
					 )
{
	int deli_index;	/* Delimiter index */
	int cplen;

	if (line_str==NULL || keyword==NULL || cfentry_ptr==NULL)
	{
		printf("Input parameter error!\n");
		return ERROR;
	}

	deli_index=findstr(line_str,keyword);
	if (deli_index==ERROR)
	{
		printf("Cannot find keyword!\n");
		return ERROR;
	}
	cplen=min(deli_index+1,MAX_CONFIG_CHAR_NUM-1);
	memcpy(cfentry_ptr->config_parameter,line_str,cplen);
	cfentry_ptr->config_parameter[cplen]='\0';
	cplen=strlen(line_str)-deli_index-strlen(keyword);
	cplen=min(cplen,MAX_CONFIG_CHAR_NUM-1);
	memcpy(cfentry_ptr->config_value,line_str+deli_index+strlen(keyword),cplen);
	cfentry_ptr->config_value[cplen]='\0';

	return OK;
}

static config_parameter_cmp(void* node_data_ptr, void* key_word_ptr, void* userdata)
{
	LINUX_CONFIG_ENTRY_PTR cfentry_ptr;
	STRING param_str;

	cfentry_ptr=(LINUX_CONFIG_ENTRY_PTR)node_data_ptr;
	param_str=(STRING)key_word_ptr;

	return strcmplow(cfentry_ptr->config_parameter,param_str);
}
