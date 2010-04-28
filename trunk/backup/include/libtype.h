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
typedef unsigned char uint8_t;
#endif
#ifndef TYPE_UINT16
#define TYPE_UINT16
typedef unsigned short int uint16_t;
#endif

#ifndef TYPE_UINT32
#define TYPE_UINT32
typedef unsigned int uint32_t;
#endif

#ifndef TYPE_ULONG
#define TYPE_ULONG
typedef unsigned long 	uint32_t;
#endif

#ifndef TYPE_INT8
#define TYPE_INT8
typedef char 	int8_t;
#endif

#ifndef TYPE_INT16
#define TYPE_INT16
typedef short	int16_t;
#endif

#ifndef TYPE_INT32
#define TYPE_INT32
typedef int	int32_t;
#endif

#ifndef TYPE_INT
#define TYPE_INT
typedef int	int32_t;
#endif

#ifndef TYPE_UINT
#define TYPE_UINT
typedef unsigned int	uint32_t;
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
typedef unsigned short uint16_t;
#endif
typedef char * STRING;
typedef uint8_t *	USTRING;
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
	uint32_t	tv_sec;
	uint32_t	tv_usec;
}TIME_INTERVAL,*TIME_INTERVAL_PTR;


#endif
