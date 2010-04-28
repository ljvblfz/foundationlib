#ifndef _DATATYPE
#define _DATATYPE

/* OS definition */
/*
#ifndef	LINUX_OS
#define LINUX_OS
#endif
*/

#ifndef WINDOWS_OS
#define WINDOWS_OS
#endif

/*
#ifndef VXWORKS_OS
#define VXWORKS_OS
#endif
*/

#ifndef OSI_ERROR
#define OSI_ERROR						-1
#endif

#ifndef OSI_OK
#define OSI_OK							0
#endif

/* variable alias define */
#ifndef TYPE_UINT8
#define TYPE_UINT8
typedef unsigned char UINT8_T;
#endif
#ifndef TYPE_UINT16
#define TYPE_UINT16
typedef unsigned short int UINT16_T;
#endif

#ifndef TYPE_UINT32
#define TYPE_UINT32
typedef unsigned int UINT32_T;
#endif

#ifndef TYPE_ULONG
#define TYPE_ULONG
typedef unsigned long 	UINT32_T;
#endif

#ifndef TYPE_INT8
#define TYPE_INT8
typedef char 	INT8_T;
#endif

#ifndef TYPE_INT16
#define TYPE_INT16
typedef short	INT16_T;
#endif

#ifndef TYPE_INT32
#define TYPE_INT32
typedef int	INT32_T;
#endif

#ifndef TYPE_INT
#define TYPE_INT
typedef int	INT32_T;
#endif

#ifndef TYPE_UINT
#define TYPE_UINT
typedef unsigned int	UINT32_T;
#endif

#ifndef TYPE_PUINT8
#define TYPE_PUINT8
typedef unsigned char*	PUINT8;
#endif

#ifndef TYPE_PUINT
#define TYPE_PUINT
typedef unsigned int*	PUINT;
#endif

#ifndef TYPE_PINT
#define TYPE_PINT
typedef int*	PINT;
#endif

#ifndef TYPE_PVOID
#define TYPE_PVOID
typedef void*	PVOID;
#endif

#ifndef TYPE_USHORT
#define TYPE_USHORT
typedef unsigned short UINT16_T;
#endif
typedef char * STRING;
typedef UINT8_T *	USTRING;
typedef enum {False,True} LOGICAL;


typedef struct  
{
	int year;
	int month;
	int day;
	int week;
	int hour;
	int minute;
	int second;
}DATE_TIME_REP,*DATE_TIME_REP_PTR;

typedef struct  
{
	DATE_TIME_REP StartDateTime;					
	DATE_TIME_REP StopDateTime;
}DATE_TIME_SEGMENT,*DATE_TIME_SEGMENT_PTR;


typedef struct  
{
	UINT32_T	tv_sec;
	UINT32_T	tv_usec;
}TIME_INTERVAL,*TIME_INTERVAL_PTR;


#endif
