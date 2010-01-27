/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: udpApiWrap.c 
 *  Description: UDP������װ�ļ� 
 *  Author: caoyun
 *  Create:2010-01-25
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#include "netHeader.h"

#ifdef LINUX_OS
/*=======================================================
 *				������UDP API��Wrapper
 * ====================================================*/
/*************************************************
* Function:		  Recvfrom()
* Description:    ������UDP�Ľ������ݺ��� 
* Input:          sockfd---������
*				  buf---��ȡ�������ĵ�ַ
*				  nbytes---��ȡ�ֽ���
*				  flags--- 
* Output:         from---Դ���׽ӿڵ�ַ�ṹ
*				  addrlen---Դ���׽ӿڵ�ַ�ṹ����
* Return:         ��ȡ�ֽ���/-1 
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
* Description:    ������UDP�ķ������ݺ��� 
* Input:          sockfd---������
*				  buf---���ͻ������ĵ�ַ
*				  nbytes---�����ֽ���
*				  flags--- 
* Output:         from---�޶��׽ӿڵ�ַ�ṹ
*				  addrlen---�޶��׽ӿڵ�ַ�ṹ����
* Return:         �����ֽ���/-1 
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
* Description:    ����UDP�׽ӿ� 
* Input:          bufsize---UDP�׽ӿڻ�������С
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

