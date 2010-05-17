/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: thread.c
 *  Description: thread api from linux/vxworks/win32
 *  Author: Ben Shaw
 *  Create:2010-05-17
 *  Modification history:
 *  2010-05-17, created the file,         Ben Shaw
 *
 *
 **************************************************************************************/ 

#include "libtype.h"
#include "thread.h"

INT32_T thread_create(
                      thread_handle* thread_id, 
                      THREAD_PARAM_PTR thread_param_ptr,
                      void* route_ptr,
                      void* args  
                      )
{
    
#ifdef LINUX_OS
    
#endif
}