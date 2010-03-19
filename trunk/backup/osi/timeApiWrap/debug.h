/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: ioApiWrap.h 
 *  Description: debug infomation API header 
 *  Author: caoyun
 *  Create:2010-1-25 
 *  Modification history:
 *
 *
 **************************************************************************************/ 

#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef  __cplusplus
extern "C"
{
#endif

#define    DEBUG_LEVEL_1          1    //普通调试信息，发布版本关闭
#define    DEBUG_LEVEL_2          2    //重要调试信息，程序中一直存在
#define    DEBUG_LEVEL_3          3    //轻微错误调试信息,调试时不会中断程序
#define    DEBUG_LEVEL_4          4    //致命错误调试信息，调试时会中断程序

void debug_info(int debugLevel, const char *fmt, ...);//调试信息

#ifdef __cplusplus
}
#endif

#endif



