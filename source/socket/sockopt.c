/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: sockopt.c 
 *  Description: this file contains apis of sockopt
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

/*========================================================
 *			���� sockopt API  Wrapper 
 * =====================================================*/
/*************************************************
* Function:       Getsockopt()
* Description:    getsockopt()�ķ�װ���� 
* Input:		  sockfd---�׽ӿ�������
*                 level---�׽ӿ����Լ���
*                 optname---�׽ӿ�����ѡ����
* Output:         optval---�׽ӿ�����ѡ��ֵ
*                 optlen---�׽ӿ�����ѡ���
* Return:         0---success/-1---fail
*************************************************/
int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen)
{
	int rval;

	if((rval = getsockopt(sockfd, level, optname, optval, optlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "getsockopt failed!\n");	
	}
	
	return rval;
}

/*************************************************
* Function:       Setsockopt()
* Description:    setsockopt()�ķ�װ���� 
* Input:		  sockfd---�׽ӿ�������
*                 level---�׽ӿ����Լ���
*                 optname---�׽ӿ�����ѡ����
*                 optval---�׽ӿ�����ѡ��ֵ
*                 optlen---�׽ӿ�����ѡ���
* Output:         N/A 
* Return:         0---success/-1---fail
*************************************************/
int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
	int rval;

	if((rval = setsockopt(sockfd, level, optname, optval, optlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "setsockopt failed!\n");	
	}
	
	return rval;
}


/*===========================================================
 *			�߼� sockopt API Wrapper
 * ========================================================*/
/*************************************************
* Function:		  GetsockRecvBuf()
* Description:    ��ȡ�׽ӿڽ��ջ��������� 
* Input:		  sockfd---�׽ӿ������� 
* Output:         bufsize---�׼��ڻ��������� 
* Return:         0---success/-1---fail
*************************************************/
int GetsockRecvBuf(int sockfd, int *bufsize)
{
	int rval;
	int optlen = sizeof(int);

	if(bufsize==NULL)
	{
		return -1;	
	}

	if((rval = Getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, bufsize,(socklen_t*)&optlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Getsockopt_recvbuf failed!\n");		
	}

	if(optlen != sizeof(int))
	{
		debug_info(DEBUG_LEVEL_4, "Getsockopt_recvbuf optlen error!\n");		
	}

	return rval;
}

/*************************************************
* Function:		  SetsockRecvBuf()
* Description:    �����׽ӿڽ��ջ��������� 
* Input:		  sockfd---�׽ӿ������� 
*                 bufsize---�׼��ڻ���������
* Output:         N/A 
* Return:         0---success/-1---fail
*************************************************/
int SetsockRecvBuf(int sockfd, int bufsize)
{
	int rval;
	socklen_t optlen = sizeof(int);

	if((rval = Setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char*)&bufsize, optlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Setsockopt_recvbuf failed!\n");		
	}

	return rval;
}

/*************************************************
* Function:		  GetsockSendBuf()
* Description:    ��ȡ�׽ӿڷ��ͻ��������� 
* Input:		  sockfd---�׽ӿ������� 
* Output:         bufsize---�׼��ڻ��������� 
* Return:         0---success/-1---fail
*************************************************/
int GetsockSendBuf(int sockfd, int *bufsize)
{
	int rval;
	socklen_t optlen = sizeof(int);

	if(bufsize==NULL)
	{
		return -1;	
	}

	if((rval = Getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, bufsize,(socklen_t*)&optlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Getsockopt_sendbuf failed!\n");		
	}

	if(optlen != sizeof(int))
	{
		debug_info(DEBUG_LEVEL_4, "Getsockopt_sendbuf optlen error!\n");		
	}

	return rval;
}

/*************************************************
* Function:		  SetsockSendBuf()
* Description:    �����׽ӿڷ��ͻ��������� 
* Input:		  sockfd---�׽ӿ������� 
*                 bufsize---�׼��ڻ���������
* Output:         N/A  
* Return:         0---success/-1---fail
*************************************************/
int SetsockSendBuf(int sockfd, int bufsize)
{
	int rval;
	socklen_t optlen = sizeof(int);

	if((rval = Setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char*)&bufsize, optlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Setsockopt_sendbuf failed!\n");		
	}

	return rval;
}

/*************************************************
* Function:       SetsockLingerOn()
* Description:    �����׽ӿ�ǿ�ƹر��������� 
* Input:		  sockfd---�׽ӿ������� 
* Output:         N/A
* Return:         0---success/-1---fail
*************************************************/
int SetsockLingerOn(int sockfd)
{
	struct linger clinger;
	int rval;

	clinger.l_onoff = 1;		//ǿ�ƹر�sockt����
	clinger.l_linger = 0;		//û���ӳ�
	if((rval = Setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (char *)&clinger, sizeof(clinger))) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Setsockopt_lingerOn failed!\n");		
	}

	return rval;
}

/*************************************************
* Function:       SetsockReuse()
* Description:    �����׽ӿ��������ñ��ص�ַ���� 
* Input:		  sockfd---�׽ӿ������� 
* Output:         N/A
* Return:         0---success/-1---fail
*************************************************/
int SetsockReuse(int sockfd)
{
	int optval;
	int rval;

    optval = 1;
	if((rval = Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval))) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "SetsockReuse failed!\n");		
	}

	return rval;
}

