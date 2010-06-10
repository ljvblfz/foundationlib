/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationInclude.h 
 *  Description: this file include system *.h that to be needed
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *  2010-01-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/ 

#ifndef _INCLUDE_H
#define _INCLUDE_H

#ifdef  __cplusplus
extern "C"
{
#endif

#ifdef LINUX_OS

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>

#include <limits.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <sched.h>
#include <mqueue.h>
#include <semaphore.h>

#elif VXWORKS_OS
/* TODO need collect */

#endif


#ifdef __cplusplus
}
#endif

#endif






