/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: dateType.h 
 *  Description: 工程数据类型定义(全工程范围遵守) 
 *  Author: caoyun
 *  Create:2010-01-25 
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

#ifndef AII_FOREVER
#define AII_FOREVER for(;;)
#endif

#ifndef AII_NULL
#define AII_NULL   0
#endif

#ifndef AII_ERROR
#define AII_ERROR -1
#endif

#ifndef AII_OK
#define AII_OK  0
#endif

//用于函数返回值
#ifndef AII_TRUE
#define AII_TRUE  1
#endif

#ifndef AII_FALSE
#define AII_FALSE 0
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
	FALSE = 0,
   	TRUE = 1,
}BOOL;
#endif

#ifndef HIK_TYPE_UINT8
#define HIK_TYPE_UINT8
typedef unsigned char UINT8_T;
#endif

#ifndef HIK_TYPE_UINT16
#define HIK_TYPE_UINT16 
typedef unsigned short UINT16_T;
#endif

#ifndef HIK_TYPE_UINT32
#define HIK_TYPE_UINT32
typedef unsigned int UINT32_T;
#endif

#ifndef HIK_TYPE_UINT64
#define HIK_TYPE_UINT64
typedef unsigned long long UINT64_T;
#endif

#ifndef HIK_TYPE_INT8
#define HIK_TYPE_INT8
typedef char INT8_T;
#endif

#ifndef HIK_TYPE_INT16
#define HIK_TYPE_INT16
typedef short INT16_T;
#endif

#ifndef HIK_TYPE_INT32
#define HIK_TYPE_INT32
typedef int INT32_T;
#endif

#ifndef HIK_TYPE_INT64
#define HIK_TYPE_INT64
typedef long long INT64_T;
#endif

#ifndef HIK_TYPE_UINT
#define HIK_TYPE_UINT
typedef unsigned int UINT32_T;
#endif

#ifndef HIK_TYPE_USHORT
#define HIK_TYPE_USHORT
typedef unsigned short UINT16_T;
#endif

#ifndef HIK_TYPE_ULONG
#define HIK_TYPE_ULONG
typedef unsigned long UINT32_T;
#endif

#ifndef HIK_TYPE_INT
#define HIK_TYPE_INT
typedef int INT32_T;
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
typedef volatile UINT32_T UINTV;
#endif

#ifndef HIK_TYPE_PUINTV
#define HIK_TYPE_PUINTV
typedef volatile UINT32_T* PUINTV;
#endif

#ifdef __cplusplus
}
#endif

#endif

