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
 *				高级 网络IO API　Wrapper
 * ====================================================*/

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
		
        if(select(connfd+1, &rset, NULL, NULL, &select_timeout) <= 0) 
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

