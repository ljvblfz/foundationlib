/*
*	configPars.c
*	Config file pars C code file.
*	This code file include functions pars linux config file.
*	
*	Foundation Services, Tool Module.
*	Based on SDK5.RC16, Linux 2.6.20, ARM Host CPU.
*	
*	MG3500 (MAXIM, mobilygen, Inc.) HD project
*   Copyright (c) Artificial Intelligence Infinity.
*
*	2009.07.24 Solution designed by shaobin, Programmed by shaobin.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tool.h"
#include "list.h"

#define MAX_CONFIG_CHAR_NUM		128

static config_parameter_cmp(void* node_data_ptr, void* key_word_ptr);


LIST_HANDLE read_config_file(STRING filename)
{
	char* line_ptr;
	size_t line_len;
	int nread,count,i;
	LIST_HANDLE handle;
    
    
    
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
	int deli_index,*deli_index_ptr;

	cfentry_ptr=(LINUX_CONFIG_ENTRY_PTR)node_data_ptr;
	param_str=(STRING)key_word_ptr;
	deli_index_ptr=(int*)userdata;

	deli_index=findstr(cfentry_ptr->config_parameter,param_str);
	if (deli_index==ERROR)
	{
		return ERROR;
	}

	*deli_index_ptr=deli_index;

	return OK;

}


