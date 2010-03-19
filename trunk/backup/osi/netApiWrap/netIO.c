/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: netIO.c 
 *  Description: ����IO������װ�ļ� 
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#include "netHeader.h"

#ifdef LINUX_OS
/*=======================================================
 *				���� ����IO API��Wrapper
 * ====================================================*/
/*************************************************
* Function:       Select() 
* Description:    select()�ķ�װ����,ֻ���1���׽ӿڽ������� 
* Input:		  nfds---�׽ӿ����������
*                 readfds---�����������ּ�
*                 writefds---����д�����ּ�
*                 excepfds---�����쳣�����ּ�
*                 timeout---��ʱʱ��
* Output:         N/A 
* Return:         0---success/-1---fail
*************************************************/
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
		debug_info(DEBUG_LEVEL_1, "select failed!\n");	
	}
	else if(rval == 0)
	{
		//debug_info(DEBUG_LEVEL_1, "select timeout!\n");	
	}

	return rval;
}

/*************************************************
* Function:       Recv()
* Description:    recv()�ķ�װ���� 
* Input:		  sockfd---�׽ӿ�������
*                 len---�׽ӿڻ���������
*                 flag---�������ݱ�־λ
* Output:         buf---���ջ�����
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

/*=======================================================
 *				�߼� ����IO API��Wrapper
 * ====================================================*/
/*************************************************
  * Function:		readn()
  * Description:    �����ӵ�socket��ȡn���ַ�����(��ʱ5�롢������recv)
  * Input:          connfd---����sock fd
  *					*vptr---��ȡ���ݻ�����
  *					n---Ԥ����ȡ���ֽ���
  * Output:         N/A
  * Return:         ��ȡ�ֽ���/ERROR 
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
  * Description:    �����ӵ�socket����n���ַ�����(������send)
  * Input:          
  *					connfd---����sock fd
  *					*vptr---��ȡ���ݻ�����
  *					n---д����ֽ���
  * Output:         N/A
  * Return:         n���ַ�����/ERROR 
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

#endif



