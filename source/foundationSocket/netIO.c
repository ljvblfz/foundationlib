/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: netIO.c 
 *  Description: this file contains api list of network socket io operations
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

/*=======================================================
 *				基本 网络IO API　Wrapper
 * ====================================================*/

/*************************************************
* Function:       Recv()
* Description:    recv()的封装函数 
* Input:		  sockfd---套接口描述字
*                 len---套接口缓冲区长度
*                 flag---接收数据标志位
* Output:         buf---接收缓冲区
* Return:         0---success/-1---fail
*************************************************/
int Recv(int sockfd, void *buf, size_t len, int flags)
{
	int rval;

	if((rval = recv(sockfd, buf, len, flags)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "recv error!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Send()
* Description:    send()的封装函数 
* Input:		  sockfd---套接口描述字
*                 len---发送数据长度
*                 flag---发送数据标志位
* Output:         buf---发送缓冲区
* Return:         0---success/-1---fail
*************************************************/
int Send(int sockfd, void *buf, size_t len, int flags)
{
	int rval;

	if((rval = send(sockfd, buf, len, flags)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "recv error!\n");	
	}

	return rval;
}

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


/*=======================================================
 *				高级 网络IO API　Wrapper
 * ====================================================*/

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *excepfds, struct timeval *timeout)
{
	int rval;
	int sockfd;
	
	sockfd = nfds - 1;

	if(readfds != NULL)
	{
		FD_ZERO(readfds);
		FD_SET(sockfd, readfds);
	}	
	if(writefds != NULL)
	{
		FD_ZERO(writefds);
		FD_SET(sockfd, writefds);
	}	
	if(excepfds != NULL)
	{
		FD_ZERO(excepfds);
		FD_SET(sockfd, excepfds);
	}

	rval = select(nfds, readfds, writefds, excepfds, timeout);
	if(rval == -1)
	{
		debug_info(DEBUG_LEVEL_4,"select failed!\n");	
	}
	else if(rval == 0)
	{
		//debug_info("select timeout!\n");	
	}

	return rval;
}
/*************************************************
  * Function:		readn()
  * Description:    从连接的socket读取n个字符数据(超时5秒、阻塞型recv)
  * Input:          connfd---连接sock fd
  *					*vptr---读取数据缓冲区
  *					n---预定读取的字节数
  * Output:         N/A
  * Return:         读取字节数/ERROR 
*************************************************/
int readn(int connfd, void *vptr, int n)
{
	int nleft;
	int nread;
	char *ptr;
	struct timeval  select_timeout;
	fd_set rset;

	ptr = (char *)vptr;
	nleft = n;
	while (nleft > 0)
	{
		select_timeout.tv_sec = 5;
		select_timeout.tv_usec = 0;
		FD_ZERO(&rset);
		FD_SET(connfd, &rset);
		
        if(Select(connfd+1, &rset, NULL, NULL, &select_timeout) <= 0) 
		{	/* 0--timeout */
			return ERROR;
		}
		
        if((nread = recv(connfd, ptr, nleft, 0)) < 0)
		{
			//debug_info(DEBUG_LEVEL_3, "recv failed!\n");
			if(errno == EINTR)
			{
				nread = 0;
			} 
			else 
			{
				return ERROR;
			}
		} 
		else if (nread == 0) 
		{
			break;
        }
		nleft -= nread;
		ptr   += nread;
	}
	
	return(n - nleft);
}

/*************************************************
  * Function:		writen()
  * Description:    向连接的socket发送n个字符数据(阻塞型send)
  * Input:          
  *					connfd---连接sock fd
  *					*vptr---读取数据缓冲区
  *					n---写入的字节数
  * Output:         N/A
  * Return:         n个字符数据/ERROR 
*************************************************/
int writen(int connfd, void *vptr, size_t n)
{
	int nleft, nwritten;
 	char	*ptr;

	ptr = vptr;
	nleft = n;

	while(nleft>0)
	{
		if((nwritten = send(connfd, ptr, nleft, MSG_NOSIGNAL)) == ERROR)
		{
			//debug_info(DEBUG_LEVEL_3, "send failed!\n");
			if(errno == EINTR)
			{
				nwritten = 0;
			}
			else 
			{
				return ERROR;
			}
		}
		nleft -= nwritten;
		ptr   += nwritten;
	}

	return(n);
}

