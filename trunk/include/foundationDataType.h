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

#if 1
#ifndef WIN32
#define WIN32
#endif
#endif

#if 0
#ifndef LINUX_OS
#define LINUX_OS
#endif
#endif

#if 0
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
     *	Basic type definition, compliant with ISO (POSIX)
     */
#ifndef char_t
    typedef char                char_t;
#endif
    
#ifndef int8_t
    typedef signed char         int8_t;
#endif

#ifndef uint8_t
    typedef unsigned char       uint8_t;
#endif

#ifndef int16_t
    typedef signed short        int16_t;
#endif

#ifndef uint16_t
    typedef unsigned short      uint16_t;
#endif

#ifndef int32_t
    typedef signed int          int32_t;
#endif

#ifndef uint32_t
    typedef unsigned int        uint32_t;
#endif

#ifndef int64_t
    typedef signed long long    int64_t;
#endif

#ifndef uint64_t
    typedef unsigned long long  uint64_t;
#endif

#ifndef float32_t
    typedef float               float32_t;
#endif

#ifndef float64_t
    typedef double              float64_t;
#endif

#ifndef float128_t
    typedef long double         float128_t;
#endif
    
    /* Convenient type definition */
#ifndef STRING
    typedef char * STRING;
#endif

#ifndef USTRING
    typedef unsigned char * USTRING;
#endif

#ifndef BOOL
    typedef enum {False, True}BOOL;
#endif

#ifdef __cplusplus
}
#endif

#endif

