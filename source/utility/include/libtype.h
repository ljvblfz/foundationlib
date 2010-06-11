
#ifndef _DATATYPE_H
#define _DATATYPE_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LINUX_OS
#define LINUX_OS
#endif

    /* Especial macro definition */
#ifndef FOREVER
#define FOREVER for(;;)
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif

#ifndef ERROR
#define ERROR   -1
#endif

#ifndef OK
#define OK      0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif
    
#ifndef WAIT_FOREVER
#define WAIT_FOREVER (-1)
#endif

#ifndef NO_WAIT
#define NO_WAIT         0
#endif

    /*
     *	Basic type definition
     */
    typedef char                CHAR_T;
    
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
    
    typedef enum {False, True}  BOOL;

#define min(x, y) x > y ? y:x

#ifdef __cplusplus
}
#endif

#endif                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
