/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: tcpApiWrap.c 
 *  Description: TCP函数封装文件 
 *  Author: caoyun
 *  Create:2010-01-25
 *  Modification history:
 *
 *
 **************************************************************************************/ 
#include "netHeader.h"

#ifdef LINUX_OS
/*=======================================================
 *				基本　TCP API　Wrapper
 * ====================================================*/
/*************************************************
  * Function:       Socket()
  * Description:    socket()的封装函数 
  * Input:          family---网络协议族类型
  *                 type---套接口类型
  *                 protocol---协议类别
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
int Socket(int family, int type, int protocol)
{
	int rval;

	if((rval = socket(family, type, protocol)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d: socket failed!\n", __FILE__, __LINE__);	
	}

	return rval;
}

/*************************************************
  * Function:       Connect()
  * Description:    connect()的封装函数 
  * Input:          sockfd---套接口描述字 
  *                 servaddr---对端IP地址
  *                 addrlen---对端IP地址长度
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
int Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
{
	int rval;

	if((rval = connect(sockfd,servaddr, addrlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d:connect failed!\n", __FILE__, __LINE__);	
	}

	return rval;
}

/*************************************************
  * Function:       Bind()
  * Description:    bind()的封装函数
  * Input:          sockfd---套接口描述字 
  *                 myaddr---自身的IP地址
  *                 addrlen---自身IP地址的长度
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
int Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen)
{
	int rval;

	if((rval = bind(sockfd, myaddr, addrlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d: bind failed!\n", __FILE__, __LINE__);	
	}

	return rval;
}

/*************************************************
  * Function:       Listen()
  * Description:    listen()的封装函数 
  * Input:          sockfd---套接口描述字
  *                 backlog---
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
int Listen(int sockfd, int backlog)
{
	int rval;

	if((rval = listen(sockfd, backlog)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d: listen failed!\n", __FILE__, __LINE__);	
	}

	return rval;
}

/*************************************************
  * Function:       Accept()
  * Description:    accept()的封装函数 
  * Input:          sockfd---套接口描述字 
  *                 cliaddr---对端IP地址
  *                 addrlen---对端IP地址长度
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
{
	int rval;

	if((rval = accept(sockfd, cliaddr, addrlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d: accept failed!\n", __FILE__, __LINE__);	
	}

	return rval;
}

/*************************************************
  * Function:       Close()
  * Description:    close()的封装函数 
  * Input:          sockfd---套接口描述字 
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
int Close(int sockfd)
{
	int rval;

	if((rval = close(sockfd)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d: close failed!\n", __FILE__, __LINE__);	
	}

	return rval;
}

/*************************************************
  * Function:       Getsockname()
  * Description:    getsockname()的封装版本 
  * Input:          sockfd---套接口描述字
  * Output:         localaddr---与套件口关联的本地协议地址
  *                 addrlen---本地协议地址长度
  * Return:         0---success/-1---fail
*************************************************/
int Getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrlen)
{
	int rval;

	if((rval = getsockname(sockfd, localaddr, addrlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d :getsockname failed!\n", __FILE__, __LINE__);	
	}

	return rval;
}

/*************************************************
  * Function:       Getpeername()
  * Description:    getpeername()的封装函数 
  * Input:          sockfd---套接口描述字
  * Output:         peeraddr---与套件口关联的对端协议地址
  *                 addrlen---对端协议地址长度
  * Return:         0---success/-1---fail
*************************************************/
int Getpeername(int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen)
{
	int rval;

	if((rval = getpeername(sockfd, peeraddr, addrlen)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "%s %d: getpeername failed!\n", __FILE__, __LINE__);	
	}

	return rval;
}


/*=============================================================
 *				高级 TCP API Wrapper
 * ==========================================================*/
/*************************************************
  * Function:       connectWithTimeout()
  * Description:    带超时时限的connect 
  * Input:          sockfd---套接口描述字 
  *                 addrs---服务器的IP地址
  *                 adrLen---服务器IP地址长度
  *                 tm---超时时限
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
int connectWithTimeout(int sockfd, struct sockaddr*addrs, int adrsLen,struct timeval* tm)
{
    int err = 0;
    int len = sizeof(int);
    int flag;
    int ret;
    fd_set wset;
    struct timeval mytm;

    if(tm!=AII_NULL)
	{
        memcpy(&mytm, tm, sizeof(struct timeval));
    }

	/*设置socket非阻塞属性*/
    flag = 1;
    ioctl(sockfd,FIONBIO,&flag);

	/*man connect ,详细查看EINPROGRESS解释*/
    ret = connect(sockfd, addrs, adrsLen);
    if(ret == -1)
    {
	    if(EINPROGRESS == errno)
	    {
	    	FD_ZERO(&wset);
	    	FD_SET(sockfd,&wset);
		    if(select(sockfd+1,AII_NULL,&wset,AII_NULL,tm) > 0)
		    {
		    	getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&err,(socklen_t*)&len);
		    	if (0 == err)
				{
		    		ret = AII_OK;
				}
		    	else
				{
		    		ret = AII_ERROR;
				}
		    }
		    else
		    {
		    	ret = AII_ERROR;
		    }
	    }
    }

	/*恢复socket阻塞属性*/
    flag = 0;
    ioctl(sockfd,FIONBIO,&flag);

    if(tm!=AII_NULL)
	{
        memcpy(tm, &mytm, sizeof(struct timeval));
    }

    return ret;
}

/*************************************************
  * Function:       tcp_connect()
  * Description:    tcp的阻塞connect函数 
  * Input:          serverIp---服务器的IP地址
  *                 serverPort---服务器的端口
  *                 bufsize---服务器的缓冲区大小
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
static int tcp_connect(int serverIp, uint16_t serverPort, int bufsize)
{
	int sockfd;
	struct sockaddr_in serverAddr;

	if(serverIp==0 || serverPort==0)
	{
		return -1;	
	}

	if((sockfd = Socket(AF_INET,SOCK_STREAM, 0)) == -1)
	{
		return -1;
	}
	
	if(bufsize>0)
	{
		if(Sockopt_set_recvBuf(sockfd, bufsize) == -1)
		{
            Close(sockfd);
			return -1;
		}
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = serverIp;
	serverAddr.sin_port = htons(serverPort);	
	if(Connect(sockfd, (SA*)&serverAddr, sizeof(serverAddr)) == -1)
	{
		Sockopt_set_lingerOn(sockfd);
		Close(sockfd);
		return -1;
	}

	return sockfd;
}

/*************************************************
  * Function:       Tcp_connect()
  * Description:    tcp_connect()的封装函数 
  * Input:          serverIp---服务器的IP地址
  *                 serverPort---服务器的端口
  *                 bufsize---服务器的缓冲区大小
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
int Tcp_connect(int serverIp, uint16_t serverPort, int bufsize)
{
	int sockfd;

	if((sockfd = tcp_connect(serverIp, serverPort, bufsize)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "tcp_connect failed!\n");	
	}

	return sockfd;
}

/*************************************************
  * Function:       tcp_connect_nb()
  * Description:    TCP的非阻塞带超时connect版本() 
  * Input:          serverIp---服务器的IP地址
  *                 serverPort---服务器的端口
  *                 bufsize---服务器的缓冲区大小
  *                 delaytime---超时时限
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
static int tcp_connect_nb(int serverIp, uint16_t serverPort, int bufsize, struct timeval delaytime)
{
	int sockfd;
	struct sockaddr_in serverAddr;
	struct timeval timeout;

	if(serverIp==0 || serverPort==0)
	{
		return -1;	
	}

	if((sockfd = Socket(AF_INET,SOCK_STREAM, 0)) == -1)
	{
		return -1;
	}
	
	if(bufsize>0)
	{
		if(Sockopt_set_recvBuf(sockfd, bufsize) == -1)
		{
            Close(sockfd);
			return -1;
		}
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = serverIp;
	serverAddr.sin_port = htons(serverPort);	
	if(delaytime.tv_sec==0 && delaytime.tv_usec==0)
	{
		timeout.tv_sec = 5;//系统默认等待5秒
		timeout.tv_usec = 0;	
	}
	else
	{
		timeout.tv_sec = delaytime.tv_sec;
		timeout.tv_usec = delaytime.tv_usec;	
	}
	if(connectWithTimeout(sockfd,(SA*)&serverAddr, sizeof(serverAddr), &timeout) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "connectWithTimeout failed!\n");	
		Sockopt_set_lingerOn(sockfd);
		Close(sockfd);
		return -1;
	}
	
	return sockfd;
}

/*************************************************
  * Function:       Tcp_connect_nb()
  * Description:    tcp_connect_nb()的封装函数 
  * Input:          serverIp---服务器的IP地址
  *                 serverPort---服务器的端口
  *                 bufsize---服务器的缓冲区大小
  *                 delaytime---超时时限
  * Output:         N/A 
  * Return:         0---success/-1---fail
*************************************************/
int Tcp_connect_nb(int serverIp, uint16_t serverPort, int bufsize, struct timeval timeout)
{
	int sockfd;

	if((sockfd = tcp_connect_nb(serverIp, serverPort, bufsize, timeout)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "tcp_connect_nb failed!\n");	
	}

	return sockfd;
}

#endif


