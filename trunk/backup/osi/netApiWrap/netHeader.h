/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: netHeader.h 
 *  Description: 网络头文件集合 
 *  Author: caoyun
 *  Create:2010-01-25
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#include <stdio.h>
#include <unistd.h>
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

#include "dataType.h"
#include "debug.h"
#include "netApiWrap.h"



#define	SA	struct sockaddr




