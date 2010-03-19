/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: sysTest.h 
 *  Description: 系统测试头文件 
 *  Author: caoyun
 *  Create:2010-01-25
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#ifndef _SYSTEST_H
#define _SYSTEST_H

#ifdef __cplusplus
extern "C"
{
#endif

int testHostByteOrder(const char* recvBuf, char*sendBuf);
int testDefSockopts(const char* recvBuf, char* sendBuf);

#ifdef __cplusplus
}
#endif

#endif

