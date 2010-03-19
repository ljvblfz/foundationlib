/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: udpApiWrap.c 
 *  Description: UDP函数封装文件 
 *  Author: caoyun
 *  Create:2010-01-25
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#include "netHeader.h"

#ifdef LINUX_OS
/*=======================================================
 *				基本　UDP API　Wrapper
 * ====================================================*/
/*************************************************
* Function:		  Recvfrom()
* Description:    常用于UDP的接收数据函数 
* Input:          sockfd---描述字
*				  buf---读取缓冲区的地址
*				  nbytes---读取字节数
*				  flags--- 
* Output:         from---源端套接口地址结构
*				  addrlen---源端套接口地址结构长度
* Return:         读取字节数/-1 
*************************************************/
int	Recvfrom(int sockfd, void *buf, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen) 
{
	int rval;

	if((rval = recvfrom(sockfd, buf, nbytes, flags, from, addrlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "recvfrom error!\n");	
	}

	return rval;
}

/*************************************************
* Function:		  Sendto()
* Description:    常用于UDP的发送数据函数 
* Input:          sockfd---描述字
*				  buf---发送缓冲区的地址
*				  nbytes---发送字节数
*				  flags--- 
* Output:         from---宿端套接口地址结构
*				  addrlen---宿端套接口地址结构长度
* Return:         发送字节数/-1 
*************************************************/
int Sendto(int sockfd, const void *buf, size_t nbytes, int flags, const struct sockaddr *to, socklen_t addrlen)
{
	int rval;

	if((rval = sendto(sockfd, buf, nbytes, flags, to, addrlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "sendto error!\n");	
	}

	return rval;
}

/*************************************************
* Function:		  udp_socket()
* Description:    创建UDP套接口 
* Input:          bufsize---UDP套接口缓冲区大小
* Output:         N/A
* Return:         sockfd/-1 
*************************************************/
int Udp_socket(int bufsize)
{
	int sockfd;

	if((sockfd = Socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		return -1;
	}
	
	if(bufsize>0)
	{
		if(Sockopt_set_recvBuf(sockfd, bufsize) == -1)
		{
			return -1;
		}
	}

	return sockfd;
}

#endif

