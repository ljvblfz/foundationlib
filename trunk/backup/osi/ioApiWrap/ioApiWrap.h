/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: ioApiWrap.h 
 *  Description: uinx IO API封装头文件 
 *  Author: caoyun
 *  Create:2010-01-25
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#ifndef _IO_API_WRAP_H
#define _IO_API_WRAP_H

#ifdef __cplusplus
extern "C"
{
#endif

//lseek封装
int lseek_get_offset(int fd);
int lseek_head(int fd);
int lseek_end(int fd);
int lseek_set_offset(int fd, int offset);

#ifdef __cplusplus
}
#endif

#endif

