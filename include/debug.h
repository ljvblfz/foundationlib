/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationDbg.h 
 *  Description: debug infomation API header 
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *  2010-01-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/ 

#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef  __cplusplus
extern "C"
{
#endif

#define    DEBUG_LEVEL_1          1    //normal debug information, only open in beta version
#define    DEBUG_LEVEL_2          2    //inportant debug information
#define    DEBUG_LEVEL_3          3    //slight error debug information, always open.
#define    DEBUG_LEVEL_4          4    //fatal error debug information, always open.

/*
 * =====================================================================
 * Function:debug_info()
 * Description:
 * this routine use 4 debug level to control 4 different debug information.
        DEBUG_LEVEL_1://normol debug information
        DEBUG_LEVEL_2://important debug information
        DEBUG_LEVEL_3://slight error debug information
        DEBUG_LEVEL_4://fatal error debug information
 *
 * Input:  
 *      debugLevel: debug level
 *		fmt---format to printf
 *		...---user enter data
 * Output:  N/A
 * Return:  N/A
 *======================================================================
 */
void debug_info(int debugLevel, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif



