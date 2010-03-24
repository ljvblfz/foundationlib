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
* Function:       ioctl_sockBuf_datas()
* Description:    ��ȡ�׽ӿڻ������Ŀɶ����� 
* Input:		  sockfd---�׽ӿ�������
* Output:         N/A 
* Return:         �׽ӿڻ������Ŀɶ�����
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
* Description:    �����׽ӿڷ���������
* Input:		  sockfd---�׽ӿ�������
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
* Description:    �����׽ӿ��������� 
* Input:		  sockfd---�׽ӿ�������
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

