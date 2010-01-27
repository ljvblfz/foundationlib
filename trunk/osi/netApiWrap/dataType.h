/**************************************************************************************
 *	Copyright 2003-2010 Hangzhou Hikvision Digital Technology Co., Ltd. 
 *  Filename: dateType.h 
 *  Description: 工程数据类型定义(全工程范围遵守) 
 *  Author: caoyun
 *  Create:2009-04-1 
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#ifndef _DATATYPE_H
#define _DATATYPE_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef FOREVER
#define FOREVER for(;;)
#endif

#ifndef NULL
#define NULL   0
#endif

#ifndef ERROR
#define ERROR -1
#endif

#ifndef OK
#define OK  0
#endif

//用于函数返回值
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef HIK_TYPE_ERR
#define HIK_TYPE_ERR
typedef unsigned int ERR;
#endif

/*数据类型定义*/
#ifndef HIK_TYPE_VOID
#define HIK_TYPE_VOID
typedef void VOID;
#endif

#ifndef HIK_TYPE_BYTE
#define HIK_TYPE_BYTE
typedef unsigned char BYTE;
#endif

#ifndef HIK_TYPE_UCHAR
#define HIK_TYPE_UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef HIK_TYPE_BOOL
#define HIK_TYPE_BOOL
typedef enum 
{
	False = 0,
   	True = 1,
}BOOL;
#endif

#ifndef HIK_TYPE_UINT8
#define HIK_TYPE_UINT8
typedef unsigned char UINT8;
#endif

#ifndef HIK_TYPE_UINT16
#define HIK_TYPE_UINT16 
typedef unsigned short UINT16;
#endif

#ifndef HIK_TYPE_UINT32
#define HIK_TYPE_UINT32
typedef unsigned int UINT32;
#endif

#ifndef HIK_TYPE_UINT64
#define HIK_TYPE_UINT64
typedef unsigned long long UINT64;
#endif

#ifndef HIK_TYPE_INT8
#define HIK_TYPE_INT8
typedef char INT8;
#endif

#ifndef HIK_TYPE_INT16
#define HIK_TYPE_INT16
typedef short INT16;
#endif

#ifndef HIK_TYPE_INT32
#define HIK_TYPE_INT32
typedef int INT32;
#endif

#ifndef HIK_TYPE_INT64
#define HIK_TYPE_INT64
typedef long long INT64;
#endif

#ifndef HIK_TYPE_UINT
#define HIK_TYPE_UINT
typedef unsigned int UINT;
#endif

#ifndef HIK_TYPE_USHORT
#define HIK_TYPE_USHORT
typedef unsigned short USHORT;
#endif

#ifndef HIK_TYPE_ULONG
#define HIK_TYPE_ULONG
typedef unsigned long ULONG;
#endif

#ifndef HIK_TYPE_INT
#define HIK_TYPE_INT
typedef int INT;
#endif

#ifndef HIK_TYPE_SHORT
#define HIK_TYPE_SHORT
typedef short SHORT;
#endif

#ifndef HIK_TYPE_LONG
#define HIK_TYPE_LONG
typedef long LONG;
#endif

#ifndef HIK_TYPE_PUINT
#define HIK_TYPE_PUINT
typedef unsigned int* PUINT;
#endif

#ifndef HIK_TYPE_PINT
#define HIK_TYPE_PINT
typedef int* PINT;
#endif

#ifndef HIK_TYPE_PUINT8
#define HIK_TYPE_PUINT8
typedef unsigned char* PUINT8;
#endif

#ifndef HIK_TYPE_PVOID
#define HIK_TYPE_PVOID
typedef void* PVOID;
#endif

#ifndef HIK_TYPE_UINTV
#define HIK_TYPE_UINTV
typedef volatile UINT UINTV;
#endif

#ifndef HIK_TYPE_PUINTV
#define HIK_TYPE_PUINTV
typedef volatile UINT* PUINTV;
#endif

#ifdef __cplusplus
}
#endif

#endif

