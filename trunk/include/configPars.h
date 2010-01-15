/*
*	configPars.h
*	Config file pars head file code file.
*	This code file include functions pars linux config file.
*	
*	Foundation Services, Tool Module.
*	Based on SDK5.RC16, Linux 2.6.20, ARM Host CPU.
*	
*   Copyright (c) Artificial Intelligence Infinity.
*
*	2009.07.24 Solution designed by shaobin, Programmed by shaobin.
*/

#include "libtype.h"
#include "list.h"




int get_config_value(LIST_HANDLE text_handle, STRING config_name, STRING* config_value_ptr);
int modify_config(LIST_HANDLE text_handle, STRING config_name, STRING config_value);
