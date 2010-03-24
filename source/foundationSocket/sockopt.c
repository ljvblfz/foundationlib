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
 *			基本 sockopt API  Wrapper 
 * =====================================================*/
/*************************************************
* Function:       Getsockopt()
* Description:    getsockopt()的封装函数 
* Input:		  sockfd---套接口描述字
*                 level---套接口属性级别
*                 optname---套接口属性选项名
* Output:         optval---套接口属性选项值
*                 optlen---套接口属性选项长度
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
* Description:    setsockopt()的封装函数 
* Input:		  sockfd---套接口描述字
*                 level---套接口属性级别
*                 optname---套接口属性选项名
*                 optval---套接口属性选项值
*                 optlen---套接口属性选项长度
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
 *			高级 sockopt API Wrapper
 * ========================================================*/
/*************************************************
* Function:		  Sockopt_get_recvBuf()
* Description:    获取套接口接收缓冲区长度 
* Input:		  sockfd---套接口描述字 
* Output:         bufsize---套件口缓冲区长度 
* Return:         0---success/-1---fail
*************************************************/
int Sockopt_get_recvBuf(int sockfd, int *bufsize)
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
* Function:		  Sockopt_set_recvBuf()
* Description:    设置套接口接收缓冲区长度 
* Input:		  sockfd---套接口描述字 
*                 bufsize---套件口缓冲区长度
* Output:         N/A 
* Return:         0---success/-1---fail
*************************************************/
int Sockopt_set_recvBuf(int sockfd, int bufsize)
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
* Function:		  Sockopt_get_sendBuf()
* Description:    获取套接口发送缓冲区长度 
* Input:		  sockfd---套接口描述字 
* Output:         bufsize---套件口缓冲区长度 
* Return:         0---success/-1---fail
*************************************************/
int Sockopt_get_sendBuf(int sockfd, int *bufsize)
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
* Function:		  Sockopt_set_sendBuf()
* Description:    设置套接口发送缓冲区长度 
* Input:		  sockfd---套接口描述字 
*                 bufsize---套件口缓冲区长度
* Output:         N/A  
* Return:         0---success/-1---fail
*************************************************/
int Sockopt_set_sendBuf(int sockfd, int bufsize)
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
* Function:       Sockopt_set_lingerOn()
* Description:    设置套接口强制关闭连接属性 
* Input:		  sockfd---套接口描述字 
* Output:         N/A
* Return:         0---success/-1---fail
*************************************************/
int Sockopt_set_lingerOn(int sockfd)
{
	struct linger clinger;
	int rval;

	clinger.l_onoff = 1;		//强制关闭sockt属性
	clinger.l_linger = 0;		//没有延迟
	if((rval = Setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (char *)&clinger, sizeof(clinger))) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Setsockopt_lingerOn failed!\n");		
	}

	return rval;
}

/*************************************************
* Function:       Sockopt_set_reuse()
* Description:    设置套接口允许重用本地地址属性 
* Input:		  sockfd---套接口描述字 
* Output:         N/A
* Return:         0---success/-1---fail
*************************************************/
int Sockopt_set_reuse(int sockfd)
{
	int optval;
	int rval;

    optval = 1;
	if((rval = Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval))) == -1)
	{
		debug_info(DEBUG_LEVEL_4, "Sockopt_set_reuse failed!\n");		
	}

	return rval;
}

/*======================================================
 *				Family_to_level()
 *=====================================================*/
static int family_to_level(int family)
{
	switch (family) 
	{
		case AF_INET:
			return IPPROTO_IP;
		case AF_INET6:
			return IPPROTO_IPV6;
		default:
			return -1;
	}
}

/*************************************************
* Function:		  Family_to_level()
* Description:    根据协议族得到该选项level 
* Input:		  family 
* Output:         N/A
* Return:         level---success/-1---fail
*************************************************/
int Family_to_level(int family)
{
	int		rval;

	if((rval = family_to_level(family))<0)
	{
		debug_info(DEBUG_LEVEL_3, "family_to_level error, rval=%d\n", rval);
	}

	return (rval);
}

static int sockfd_to_family(int sockfd)
{
	struct sockaddr_storage ss;
	socklen_t	len;

	len = sizeof(ss);
	if (getsockname(sockfd, (SA *) &ss, &len) < 0)
	{
		return(-1);
	}
	return(ss.ss_family);
}

/*************************************************
* Function:		  Sockfd_to_family()
* Description:    根据sockfd得到family 
* Input:		  sockfd 
* Output:         N/A 
* Return:         family---success/-1---fail
*************************************************/
int Sockfd_to_family(int sockfd)
{
	int		rval;

	if ( (rval = sockfd_to_family(sockfd)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "sockfd_to_family error\n");
	}

	return(rval);
}

