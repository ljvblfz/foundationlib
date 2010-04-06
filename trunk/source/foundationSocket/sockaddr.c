/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: sockaddr.c 
 *  Description: this file contains api list of socket addresss convert
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
 *				基本　sock API　Wrapper
 * ====================================================*/
/*************************************************
  * Function:       Inet_pton()
  * Description:    根据网络协议族类型family,把字符型IP地址src(比如"192.0.0.5")转化为数字型IP地址dst.
  * Input:          family---网络协议族类型 
  *					src---字符型IP地址
  * Output:         dst---数字型IP地址
  * Return:         0---success/-1---fail
*************************************************/
static int Inet_pton(int family, const char *src, void *dst)
{
	int rval;
	
	if(src==NULL || dst==NULL)
	{
		return -1;	
	}

#ifdef LINUX_OS
	if((rval = inet_pton(family, src, dst)) <= 0)
	{
		debug_info(DEBUG_LEVEL_3, "inet_pton() failed!\n");	
		rval = -1;
	}

#elif VXWORKS_OS
    printf("This function is not yet available Vxworks!\n");
    rval = -1;
#endif
	return rval;
}

/*************************************************
  * Function:       Inet_ntop()
  * Description:    根据网络协议族类型family，把数字型IP地址src转化为字符型IP地址dst. 
  * Input:          family---网络协议族类型 
  *					src---数字型IP地址
  *					len---字符型IP地址长度 
  * Output:         dst---字符型IP地址
  * Return:         char*---success/NULL---fail
*************************************************/
static const char* Inet_ntop(int family, const void *src, char* dst, socklen_t len)
{
	if(src==NULL || dst==NULL)
	{
		return NULL;	
	}

#ifdef LINUX_OS
	if(inet_ntop(family, src, dst, len) == NULL)
	{
		debug_info(DEBUG_LEVEL_3, "inet_ntop() failed!\n");	
	}

	return inet_ntop(family, src, dst, len);
#elif VXWORKS_OS
    printf("This function is not yet available Vxworks!\n");
    return NULL;
#endif
}


/*=======================================================
 *				高级　sock API　Wrapper
 * ====================================================*/
/*************************************************
  * Function:       sock_ntop()
  * Description:    把数字型IP地址转化为字符型IP地址 
  * Input:          sa---通用套接口 
  * Output:         
  * Return:         字符型IP地址---success/NULL---fail
*************************************************/
static char *sock_ntop(const struct sockaddr *sa)
{
    static char str[128];		/* Unix domain is largest */

    if (sa == NULL)
    {
        return (NULL);
    }
     
	switch (sa->sa_family) 
	{
		case AF_INET:
			{
#ifdef LINUX_OS
				struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

				if (Inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
                {
					return(NULL);
                }
				return(str);
#elif VXWORKS_OS
				struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

                inet_ntoa_b(sin->sin_addr, str);
				return(str);
#endif
			}

		case AF_INET6: 
			{
#ifdef LINUX_OS
				struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

				str[0] = '[';
				if (Inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
                {
					return(NULL);
                }
				return (str + 1);
#elif VXWORKS_OS
                printf("This function is not yet available Vxworks!\n");
                return NULL;
#endif
			}
            break;

		default:
			snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d", sa->sa_family);
			return(str);
	}

	return (NULL);
}

/*************************************************
  * Function:       Sock_ntop()
  * Description:    sock_ntop()的封装函数.  
  * Input:          sa---通用套接口 
  * Output:         ipStr---字符型IP地址
  * Return:         0---success/-1---fail
*************************************************/
int Sock_ntop(const struct sockaddr *sa, char *ipStr)
{
	char	*ptr;
	
	if(sa==NULL || ipStr==NULL)
	{
		return -1;	
	}

	if((ptr = sock_ntop(sa)) == NULL)
	{
		debug_info(DEBUG_LEVEL_3, "sock_ntop() failed!\n");			
		return -1;
	}
	else
	{
		strcpy(ipStr, ptr);	
		return 0;
	}
}

/*************************************************
  * Function:       Sock_ntop_v4()
  * Description:    支持IPv4版本的Sock_ntop() 
  * Input:          IPv4版本的数字IP地址 
  * Output:         字符型IP地址 
  * Return:         0---success/-1---fail
*************************************************/
int Sock_ntop_v4(int ipaddr, char *ipStr)
{
#ifdef LINUX_OS
	char	ptr[INET_ADDRSTRLEN];
	struct sockaddr_in sockaddrIn;

	if(ipStr==NULL)
	{
		return -1;	
	}

	sockaddrIn.sin_family = AF_INET; 
	sockaddrIn.sin_addr.s_addr = ipaddr; 
	if(Sock_ntop((SA*)&sockaddrIn, ptr) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "Sock_ntop() failed!\n");			
		return -1;
	}
	else
	{
		strcpy(ipStr, ptr);	
		return 0;
	}

#elif VXWORKS_OS
    struct in_addr inetaddr;

	if(ipStr==NULL)
	{
		return -1;	
	}

    inetaddr.s_addr = ipaddr;
    inet_ntoa_b(inetaddr, ipStr);

    return 0;
#endif
}

/*************************************************
  * Function:       Sock_pton()
  * Description:    把字符型IP地址转化为数字型IP地址 
  * Input:          字符型IP地址 
  * Output:         N/A 
  * Return:         数字型IP地址---success/-1---fail
*************************************************/
static int sock_pton(const char *ipStr,struct sockaddr *sa)
{
    if (ipStr == NULL || sa == NULL)
    {
        return -1;
    }
     
	switch (sa->sa_family) 
	{
		case AF_INET:
			{
#ifdef LINUX_OS
				struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

				if (Inet_pton(AF_INET, ipStr, &sin->sin_addr) == -1)
                {
					return -1;
                }
#elif VXWORKS_OS
				struct sockaddr_in	*sin = (struct sockaddr_in *) sa;
                unsigned long inetaddr;

                inetaddr = inet_addr(ipStr);
                sin->sin_addr.s_addr = inetaddr;
#endif
			}
            break;

		case AF_INET6: 
			{
#ifdef LINUX_OS
				struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

				if (Inet_pton(AF_INET6, ipStr, &sin6->sin6_addr) == -1)
                {
					return -1;
                }
#elif VXWORKS_OS
                printf("This function is not yet available Vxworks!\n");
                return NULL;
#endif
			}
            break;

		default:
			printf("sock_ntop: unknown AF_xxx: %d", sa->sa_family);
			return -1;
	}

	return 0; 
}

/*************************************************
  * Function:       Sock_pton()
  * Description:    sock_pton()的封装函数 
  * Input:          字符型IP地址 
  * Output:         N/A 
  * Return:         数字型IP地址---success/-1---fail
*************************************************/
int Sock_pton(const char *ipStr, struct sockaddr *sa)
{
	int rval;

	if(sa==NULL || ipStr==NULL)
	{
		return -1;	
	}

	if((rval = sock_pton(ipStr, sa)) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "sock_pton() failed!\n");			
	}
	
	return rval;
}

/*************************************************
  * Function:       Sock_pton_v4()
  * Description:    支持IPv4版本的Sock_pton() 
  * Input:          字符型IP地址 
  * Output:         N/A 
  * Return:         数字型IP地址---success/-1---fail
*************************************************/
int  Sock_pton_v4(const char *ipStr)
{
#ifdef LINUX_OS
	struct sockaddr_in sockaddrIn;
	
	if(ipStr==NULL)
	{
		return -1;	
	}

	sockaddrIn.sin_family = AF_INET; 
	if(Sock_pton(ipStr, (SA*)&sockaddrIn) == -1)
	{
		debug_info(DEBUG_LEVEL_3, "Sock_pton() failed!\n");			
		return -1;
	}

	return sockaddrIn.sin_addr.s_addr; 

#elif VXWORKS_OS
	if(ipStr==NULL)
	{
		return -1;	
	}

    return inet_addr(ipStr);
#endif
}

