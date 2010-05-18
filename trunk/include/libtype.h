/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: libtype.h
 *  Description: this file contains basic types of foundation lib
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *  2010-01-25, created the file,           caoyun
 *  2010-03-09, modify the file,         chenwangxian
 *  2010-04-28, modify the file,            Ben Shaw
 *
 *
 **************************************************************************************/ 
#ifndef _DATATYPE_H
#define _DATATYPE_H

#ifdef __cplusplus
extern "C"
{
#endif

#if 0
#ifndef WIN32
#define WIN32
#endif
#endif

#ifndef LINUX_OS
#define LINUX_OS
#endif

#if 1
#ifndef VXWORKS_OS
#define VXWORKS_OS
#endif
#endif


    /* Especial macro definition */
    #define AII_FOREVER for(;;)

    #define AII_NULL    ((void*) 0)

    #define AII_ERROR   -1

    #define AII_OK      0

    #define AII_TRUE    1

    #define AII_FALSE   0

    /*
     *	Basic type definition
     */
    typedef char                CHART_T;
    
    typedef signed char         INT8_T;

    typedef unsigned char       UINT8_T;

    typedef signed short        INT16_T;

    typedef unsigned short      UINT16_T;

    typedef signed int          INT32_T;

    typedef unsigned int        UINT32_T;

    typedef signed long long    INT64_T;

    typedef unsigned long long  UINT64_T;

    typedef float               FLOAT32_T;

    typedef double              FLOAT64_T;

    /* Convenient type definition */
    typedef char *              STRING;

    typedef unsigned char *     USTRING;

    typedef enum {FALSE, TRUE}  BOOL;

#ifdef __cplusplus
}
#endif

#endif                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
