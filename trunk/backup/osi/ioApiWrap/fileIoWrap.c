/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: fileIoWrap.c 
 *  Description: unix file IO API函数封装文件 
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#include "ioHeader.h"

#ifdef LINUX_OS
/*=======================================================
 *					lseek封装	
 * ====================================================*/
/*************************************************
* Function:		  lseek_get_offset()
* Description:    get file current offset 
* Input:          fd 
* Output:         
* Return:         current offset 
*************************************************/
int lseek_get_offset(int fd)
{
	int rval;

	if((rval = lseek(fd, 0, SEEK_CUR)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d: getFileCurrOffset error!\n", __FILE__, __LINE__);	
	}

	return rval;
}

/*************************************************
* Function:		  lseek_set_offset()
* Description:    set file offset 
* Input:          fd 
* Output:         
* Return:         new offset 
*************************************************/
int lseek_set_offset(int fd, int offset)
{
	int rval;

	if((rval = lseek(fd, offset, SEEK_SET)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d: locateFileOffset error!\n", __FILE__, __LINE__);	
	}

	return rval;
}

/*************************************************
* Function:		  lseek_head()
* Description:    lseek to file header 
* Input:          fd 
* Output:         
* Return:         0 
*************************************************/
int lseek_head(int fd)
{
	int rval;

	if((rval = lseek(fd, 0, SEEK_SET)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d: locateFileHeader error!\n", __FILE__, __LINE__);	
	}

	return rval;
}

/*************************************************
* Function:		  lseek_end()
* Description:    lseek to file end 
* Input:          fd 
* Output:         
* Return:         file length 
*************************************************/
int lseek_end(int fd)
{
	int rval;

	if((rval = lseek(fd, 0, SEEK_END)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d: locateFileEnd error!\n", __FILE__, __LINE__);	
	}

	return rval;
}

#endif

