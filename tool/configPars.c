/*
*	configPars.c
*	Config file pars C code file.
*	This code file include functions pars linux config file.
*	
*	Foundation Services, Tool Module.
*	Based on SDK5.RC16, Linux 2.6.20, ARM Host CPU.
*	
*   Copyright (c) Artificial Intelligence Infinity.
*
*	2009.07.24 Solution designed by shaobin, Programmed by shaobin.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tool.h"
#include "text.h"
#include "configPars.h"

static int pars_config_line( STRING config_line_str, STRING* param_str_ptr, STRING* value_str_ptr );

int get_config_value(LIST_HANDLE text_handle, STRING config_name, STRING* config_value_ptr)
{
    MATCH_WORD_POSITION str_posi;
    LIST_HANDLE line_handle;
    STRING line_str, param_str, value_str;
    LINE_NODE_PTR line_node_ptr;

    if (IsHandleEmpty(text_handle) || config_name==NULL || config_value_ptr==NULL)
    {
        printf("Parameter error!\n");
        return OSI_ERROR;
    }

    line_handle=CreateHandle(LIST_NODE_TYPE);
    if (line_handle == NULL)
    {
        return OSI_ERROR;
    }

    str_posi.line_node_handle=line_handle;
    if (find_keyword(text_handle,config_name,&str_posi,READ_PERMISSION_TAG)==OSI_ERROR)
    {
        printf("Cannot find keyword:%s!\n", config_name);
        goto ErrExit;
    }
    
    if ((line_node_ptr=(LINE_NODE_PTR)GetData(str_posi.line_node_handle))==NULL)
    {
        goto ErrExit;
    }

    line_str=line_node_ptr->line_ptr;

    if (pars_config_line(line_str,&param_str,&value_str)==OSI_ERROR)
    {
        printf("Pars config string error!\n");
        goto ErrExit;
    }
    
    *config_value_ptr=value_str;
    return OSI_OK;

ErrExit:
    DeleteHandle(line_handle);
    return OSI_ERROR;
}

int modify_config(LIST_HANDLE text_handle, STRING config_name, STRING config_value)
{
    STRING new_line_str;
    int new_line_len, config_name_len, config_value_len;

    if (IsHandleEmpty(text_handle) || config_name==NULL || config_value==NULL)
    {
        return OSI_ERROR;
    }
    config_name_len=strlen(config_name);
    config_value_len=strlen(config_value);
    new_line_len=config_name_len+config_value_len+1; /* For extra '=' */

    if ((new_line_str=(STRING)malloc(new_line_len+1))==NULL)    /* For extra '\0' */
    {
        return OSI_ERROR;
    }

    memcpy(new_line_str, config_name, config_name_len);
    new_line_str[config_name_len]='=';
    memcpy(new_line_str+config_name_len+1,config_value,config_value_len);
    new_line_str[new_line_len]='\0';
    
    if (replace_line_kw(text_handle,config_name,new_line_str)==OSI_ERROR)
    {
        printf("Replace line error!\n");
        goto ErrExit;
    }

    return OSI_OK;

ErrExit:
    if (new_line_str)
    {
        free(new_line_str);
    }
    return OSI_ERROR;
}

static int pars_config_line(
                     STRING config_line_str,
                     STRING* param_str_ptr,
                     STRING* value_str_ptr
                     )
{
    int deli_index, config_line_len,index;
    STRING param_str, value_str;
    char c;

    if (config_line_str==NULL || param_str_ptr==NULL || value_str_ptr==NULL)
    {
        return OSI_ERROR;
    }
    
    deli_index=findstr(config_line_str,"=");
    if (deli_index==OSI_ERROR)
    {
        printf("Cannot find delimiter of config line!\n");
        return OSI_ERROR;
    }
    
    param_str=value_str=NULL;

    if ((param_str=(STRING)malloc(deli_index+1))==NULL)
    {
        printf("Memory limited!\n");
        goto ErrExit;
    }
    
    config_line_len=strlen(config_line_str);

    if ((value_str=(STRING)malloc(config_line_len-deli_index)))
    {
        printf("Memory limited!\n");
        goto ErrExit;
    }

    while (*config_line_str==' ' || *config_line_str=='\t')
        config_line_str++;
    
    index=0;
    while ((c=*config_line_str++) != '=')
    {
        param_str[index++]=c;
    }
    param_str[index]='\0';
    if (index==0)
    {
        printf("Config line syntax error!\n");
        goto ErrExit;
    }

    index=0;
    while (*config_line_str)
    {
        if (*config_line_str==' ' || *config_line_str=='\t')
        {
            config_line_str++;
        }
        value_str[index++]=*config_line_str++;
    }

    if (index==0)
    {
        printf("Config line syntax error!\n");
        goto ErrExit;
    }

    value_str[index]='\0';

    *param_str_ptr=param_str;
    *value_str_ptr=value_str;

    return OSI_OK;

ErrExit:
    if (param_str)
    {
        free(param_str);
    }
    if (value_str)
    {
        free(value_str);
    }
    return OSI_ERROR;
}



