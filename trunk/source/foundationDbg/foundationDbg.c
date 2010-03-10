/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationDbg.c 
 *  Description: debug api function: void debug_info(int debugLevel, const char *fmt, ...)
 *               
 *  Author: caoyun
 *  Create:2010-1-25 
 *  Modification history:
 *  2010-01-25, created the file,         caoyun
 *  2010-03-09, refreshed the file,       chenwangxian
 *
 *
 **************************************************************************************/ 

#include "foundationInclude"
#include "foundationDbg.h"

/* this definition switch the debug level.
 *
 * if this definition NOT defined, this will happen:
            DEBUG_LEVEL_2 print information
            DEBUG_LEVEL_3 print information
            DEBUG_LEVEL_4 print information
 * if this definition defined, this will happen PLUS:
            DEBUG_LEVEL_1 print information
            DEBUG_LEVEL_4 exit application tasks
 */
#undef DEBUG_VERSION

#define	MAXLINE		1024	/* max text line length */

/*
 * =====================================================================
 * Function:debug_doit()
 * Description:打印错误信息的底层函数，适合可变参数列表
 * Input:  
 *		bSysCall---是否打印系统错误变量error的相关信息
 *		fmt---用户输入打印参数格式
 *		ap---可变参数列表
 * Output:  N/A
 * Return:  N/A
 *======================================================================
 */
static void debug_doit(int bSysCall, const char *fmt, va_list ap)
{
	int		errno_save, n;
	char	buf[MAXLINE + 1];

	errno_save = errno;		/* value caller might want printed */
	
	vsnprintf(buf, MAXLINE, fmt, ap);	/* safe */

	n = strlen(buf);
	if (bSysCall && (errno_save>0))
	{
		snprintf(buf + n, MAXLINE - n, ": (%d)%s", errno_save, strerror(errno_save));
	}
	strcat(buf, "\n");

	fflush(stdout);		/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(stderr);
	
	return;
}

/*
 * =====================================================================
 * Function:debug_info()
 * Description:debug API
 * Input:  
 *      debugLevel: debug level
 *		fmt---用户输入打印参数格式
 *		...---用户输入参数
 * Output:  N/A
 * Return:  N/A
 *======================================================================
 */
void debug_info(int debugLevel, const char *fmt, ...)
{
	va_list		ap;

    switch(debugLevel)
    {
        case  DEBUG_LEVEL_1://normol debug information
#ifdef DEBUG_VERSION
            va_start(ap, fmt);
            debug_doit(0, fmt, ap);
            va_end(ap);
#endif
            break;

        case  DEBUG_LEVEL_2://important debug information
            va_start(ap, fmt);
            debug_doit(0, fmt, ap);
            va_end(ap);
            break;

        case  DEBUG_LEVEL_3://slight error debug information
            va_start(ap, fmt);
            debug_doit(1, fmt, ap);
            va_end(ap);
            break;

        case  DEBUG_LEVEL_4://fatal error debug information
            va_start(ap, fmt);
            debug_doit(1, fmt, ap);
            va_end(ap);

#ifdef DEBUG_VERSION
            abort();		/* dump core and terminate */
            exit(1);		/* shouldn't get here */
#endif
            break;

        default:
            printf("mismatch debugLevel!!!\n");
    }

	return;
}


