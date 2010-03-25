/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: ioctl.c
 *  Description: ioctl() about network socket operations
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *  2010-01-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#include "foundationSocket.h"
#include "foundationDbg.h"

/*************************************************
* Function:       GetsockNReadBytes()
* Description:    获取套接口缓冲区的可读数据 
* Input:		  sockfd---套接口描述字
* Output:         N/A 
* Return:         套接口缓冲区的可读数据
*************************************************/
int GetsockNReadBytes(int sockfd)
{
    int  iBytesAvail = -1; 
    int  iRetVal;          

    iRetVal = ioctl(sockfd, FIONREAD, (long)&iBytesAvail);
	if(iRetVal==-1)
	{
		debug_info(DEBUG_LEVEL_3, "ioctl_socuBuf_datas() is error!\n");	
	}

    return (iBytesAvail);
}	

/*************************************************
* Function:       SetsockNBlock()
* Description:    设置套接口非阻塞属性
* Input:		  sockfd---套接口描述字
* Output:         N/A 
* Return:         0---success/-1---fail 
*************************************************/
int SetsockNBlock(int sockfd)
{
	int flag, rval;

	flag = 1;
	rval = ioctl(sockfd, FIONBIO, &flag);
	if(rval == -1)
	{
		debug_info(DEBUG_LEVEL_4, "SetsockNBlock() is error!\n");	
	}
	
	return rval;
}

/*************************************************
* Function:       SetsockBlock()
* Description:    设置套接口阻塞属性 
* Input:		  sockfd---套接口描述字
* Output:         N/A 
* Return:         0---success/-1---fail 
*************************************************/
int SetsockBlock(int sockfd)
{
	int flag, rval;
	
	flag = 0;
    rval = ioctl(sockfd, FIONBIO, &flag);
	if(rval == -1)
	{
		debug_info(DEBUG_LEVEL_4, "SetsockBlock() is error!\n");	
	}
	
	return rval;
}

