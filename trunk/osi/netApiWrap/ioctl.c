/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: ioctl.c
 *  Description: 网络相关ioctl操作 
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#include "netHeader.h"

#ifdef LINUX_OS
/*************************************************
* Function:       ioctl_sockBuf_datas()
* Description:    获取套接口缓冲区的可读数据 
* Input:		  sockfd---套接口描述字
* Output:         N/A 
* Return:         套接口缓冲区的可读数据
*************************************************/
int ioctl_sockBuf_datas(int sockfd)
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
* Function:       Sock_noBlock()
* Description:    设置套接口非阻塞属性
* Input:		  sockfd---套接口描述字
* Output:         N/A 
* Return:         0---success/-1---fail 
*************************************************/
int Sock_noBlock(int sockfd)
{
	int flag, rval;

	flag = 1;
	rval = ioctl(sockfd, FIONBIO, &flag);
	if(rval == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Sock_noBlock() is error!\n");	
	}
	
	return rval;
}

/*************************************************
* Function:       Sock_block()
* Description:    设置套接口阻塞属性 
* Input:		  sockfd---套接口描述字
* Output:         N/A 
* Return:         0---success/-1---fail 
*************************************************/
int Sock_block(int sockfd)
{
	int flag, rval;
	
	flag = 0;
    rval = ioctl(sockfd, FIONBIO, &flag);
	if(rval == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Sock_block() is error!\n");	
	}
	
	return rval;
}

#endif

