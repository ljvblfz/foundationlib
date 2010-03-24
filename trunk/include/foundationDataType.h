/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationDataType.h
 *  Description: this file contains basic types of foundation lib
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *  2010-01-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
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

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TYPE_ERR
#define TYPE_ERR
typedef unsigned int ERR;
#endif

#ifndef TYPE_VOID
#define TYPE_VOID
typedef void VOID;
#endif

#ifndef TYPE_BYTE
#define TYPE_BYTE
typedef unsigned char BYTE;
#endif

#ifndef TYPE_UCHAR
#define TYPE_UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef TYPE_BOOL
#define TYPE_BOOL
typedef enum {False, True}BOOL;
#endif

#ifndef TYPE_UINT8
#define TYPE_UINT8
typedef unsigned char UINT8;
#endif

#ifndef TYPE_UINT16
#define TYPE_UINT16 
typedef unsigned short UINT16;
#endif

#ifndef TYPE_UINT32
#define TYPE_UINT32
typedef unsigned int UINT32;
#endif

#ifndef TYPE_UINT64
#define TYPE_UINT64
typedef unsigned long long UINT64;
#endif

#ifndef TYPE_INT8
#define TYPE_INT8
typedef char INT8;
#endif

#ifndef TYPE_INT16
#define TYPE_INT16
typedef short INT16;
#endif

#ifndef TYPE_INT32
#define TYPE_INT32
typedef int INT32;
#endif

#ifndef TYPE_INT64
#define TYPE_INT64
typedef long long INT64;
#endif

#ifndef TYPE_UINT
#define TYPE_UINT
typedef unsigned int UINT;
#endif

#ifndef TYPE_USHORT
#define TYPE_USHORT
typedef unsigned short USHORT;
#endif

#ifndef TYPE_ULONG
#define TYPE_ULONG
typedef unsigned long ULONG;
#endif

#ifndef TYPE_INT
#define TYPE_INT
typedef int INT;
#endif

#ifndef TYPE_SHORT
#define TYPE_SHORT
typedef short SHORT;
#endif

#ifndef TYPE_LONG
#define TYPE_LONG
typedef long LONG;
#endif

#ifndef TYPE_PUINT
#define TYPE_PUINT
typedef unsigned int* PUINT;
#endif

#ifndef TYPE_PINT
#define TYPE_PINT
typedef int* PINT;
#endif

#ifndef TYPE_PUINT8
#define TYPE_PUINT8
typedef unsigned char* PUINT8;
#endif

#ifndef TYPE_PVOID
#define TYPE_PVOID
typedef void* PVOID;
#endif

#ifndef TYPE_UINTV
#define TYPE_UINTV
typedef volatile UINT UINTV;
#endif

#ifndef TYPE_PUINTV
#define TYPE_PUINTV
typedef volatile UINT* PUINTV;
#endif

#ifndef TYPE_STRING
#define TYPE_STRING
typedef char * STRING;
#endif

#ifndef TYPE_USTRING
#define TYPE_USTRING
typedef UINT8 *	USTRING;
#endif

#ifndef TYPE_SA
#define TYPE_SA
#define	SA	struct sockaddr
#endif

#ifdef __cplusplus
}
#endif

#endif

