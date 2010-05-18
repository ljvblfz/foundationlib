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
#include <stdio.h>
#include <stdlib.h>

#include "libtype.h"
#include "thread.h"

#ifdef LINUX_OS
#include <pthread.h>
#endif

thread_handle thread_handle_init(void)
{
    THREAD_PROPERTY_PTR thread_property_ptr;

    thread_property_ptr = AII_NULL;

    thread_property_ptr = malloc(sizeof(THREAD_PROPERTY));
    if (thread_property_ptr == AII_NULL)
    {
        return AII_NULL;
    }
    memset(thread_property_ptr, 0x00, sizeof(THREAD_PROPERTY));

    thread_property_ptr->thread_params = 
        (THREAD_PARAM_PTR) malloc(sizeof(THREAD_PARAM));
    if (thread_property_ptr->thread_params == AII_NULL)
    {
        goto ErrExit;
    }
    memset(thread_property_ptr->thread_params, 0x00, sizeof(THREAD_PARAM));

#ifdef LINUX_OS
    thread_property_ptr->thread_attr = (void*) malloc(sizeof(pthread_attr_t));
    if (thread_property_ptr->thread_attr == AII_NULL)
    {
        goto ErrExit;
    }
    memset(thread_property_ptr->thread_attr, 0x00, sizeof(pthread_attr_t));

#endif
    
    return (thread_handle) thread_property_ptr;

ErrExit:
    if (thread_property_ptr->thread_attr)
    {
        free(thread_property_ptr->thread_attr);
        thread_property_ptr->thread_attr = AII_NULL;
    }
    
    if (thread_property_ptr->thread_params)
    {
        free(thread_property_ptr->thread_params);
        thread_property_ptr->thread_params = AII_NULL;
    }

    if (thread_property_ptr)
    {
        free(thread_property_ptr);
        thread_property_ptr = AII_NULL;
    }
    
    return AII_NULL;
}

INT32_T thread_create(
                      thread_handle thread_id, 
                      THREAD_PARAM_PTR thread_param_ptr,
                      void* route_ptr,
                      void* args  
                      )
{
    
#ifdef LINUX_OS
    
#endif

}