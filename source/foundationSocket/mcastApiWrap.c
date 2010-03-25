/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: mcastApiWrap.c 
 *  Description: this file contains api list of mcast operations
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


#ifdef LINUX_OS
/*============================================================
 *				多播数据报接收相关：6　API
 * ==========================================================*/
static int mcast_join(int sockfd, const SA *grp, socklen_t grplen, const char *ifname, u_int ifindex)
{

#ifdef MCAST_JOIN_GROUP			//协议无关实现
	struct group_req req;

	if (ifindex > 0) //定义了接口索引，则使用接口索引值
	{
		req.gr_interface = ifindex;
	} 
	else if (ifname != NULL) //未定义接口索引，定义了接口名字，则使用接口名字
	{
		if ( (req.gr_interface = if_nametoindex(ifname)) == 0) 
		{
			errno = ENXIO;	/* i/f name not found */
			return(-1);
		}
	} 
	else //两者都未定义,由内核选择接口
	{
		req.gr_interface = 0;
	}

	if (grplen > sizeof(req.gr_group)) 
	{
		errno = EINVAL;
		return -1;
	}

	memcpy(&req.gr_group, grp, grplen);
	return (setsockopt(sockfd, Family_to_level(grp->sa_family), MCAST_JOIN_GROUP, &req, sizeof(req)));
#else

	switch (grp->sa_family) 
	{
		case AF_INET: 
		{
			struct ip_mreq		mreq;
			struct ifreq		ifreq;

			memcpy(&mreq.imr_multiaddr, &((const struct sockaddr_in *) grp)->sin_addr, sizeof(struct in_addr));

			if (ifindex > 0) 
			{
				if (if_indextoname(ifindex, ifreq.ifr_name) == NULL) 
				{
					errno = ENXIO;	/* i/f index not found */
					return(-1);
				}
				if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
				{
					return(-1);
				}
				memcpy(&mreq.imr_interface, &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
			} 
			else if (ifname != NULL) 
			{
				strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
				if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
				{
					return(-1);
				}
				memcpy(&mreq.imr_interface, &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
			} 
			else
			{
				mreq.imr_interface.s_addr = htonl(INADDR_ANY);
			}

			return (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)));
		}

		case AF_INET6: 
		{
			struct ipv6_mreq	mreq6;

			memcpy(&mreq6.ipv6mr_multiaddr, &((const struct sockaddr_in6 *) grp)->sin6_addr, sizeof(struct in6_addr));

			if (ifindex > 0) 
			{
				mreq6.ipv6mr_interface = ifindex;
			} 
			else if (ifname != NULL) 
			{
				if ( (mreq6.ipv6mr_interface = if_nametoindex(ifname)) == 0) 
				{
					errno = ENXIO;	/* i/f name not found */
					return(-1);
				}
			} 
			else
			{
				mreq6.ipv6mr_interface = 0;
			}

			return (setsockopt(sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6, sizeof(mreq6)));
		}

		default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
#endif
}

/*************************************************
* Function:       Mcast_join()
* Description:    加入一个不限源的多播组 
* Input:		  sockfd---打开的UDP套接口 
*				  *grp---多播组套接口地址结构
*				  grplen---多播组套接口地址结构长度
*				  *ifname---接口名字
*				  ifindex---接口索引
* Output:         N/A 
* Return:         0/-1
*************************************************/
int Mcast_join(int sockfd, const SA *grp, socklen_t grplen, const char *ifname, u_int ifindex)
{
	int rval;

	if((rval = mcast_join(sockfd, grp, grplen, ifname, ifindex)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_join error\n");
	}

	return rval;
}

static int mcast_join_source_group(
		int sockfd,
	   	const SA *src, socklen_t srclen,
		const SA *grp, socklen_t grplen,
		const char *ifname, u_int ifindex
	)
{
#ifdef MCAST_JOIN_SOURCE_GROUP
	struct group_source_req req;
	
	if (ifindex > 0) 
	{
		req.gsr_interface = ifindex;
	} 
	else if (ifname != NULL) 
	{
		if ( (req.gsr_interface = if_nametoindex(ifname)) == 0) 
		{
			errno = ENXIO;	/* i/f name not found */
			return(-1);
		}
	} 
	else
	{
		req.gsr_interface = 0;
	}

	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) 
	{
		errno = EINVAL;
		return -1;
	}

	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, Family_to_level(grp->sa_family), MCAST_JOIN_SOURCE_GROUP, &req, sizeof(req)));
#else

	switch (grp->sa_family) 
	{
#ifdef IP_ADD_SOURCE_MEMBERSHIP
		case AF_INET: 
			{
				struct ip_mreq_source	mreq;
				struct ifreq			ifreq;

				memcpy(&mreq.imr_multiaddr, &((struct sockaddr_in *) grp)->sin_addr, sizeof(struct in_addr));
				memcpy(&mreq.imr_sourceaddr, &((struct sockaddr_in *) src)->sin_addr, sizeof(struct in_addr));

				if (ifindex > 0) 
				{
					if (if_indextoname(ifindex, ifreq.ifr_name) == NULL) 
					{
						errno = ENXIO;	/* i/f index not found */
						return(-1);
					}
					if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
					{
						return(-1);
					}
					memcpy(&mreq.imr_interface, &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
				} 
				else if (ifname != NULL) 
				{
					strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
					if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
					{
						return(-1);
					}
					memcpy(&mreq.imr_interface, &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
				} 
				else
				{
					mreq.imr_interface.s_addr = htonl(INADDR_ANY);
				}

				return(setsockopt(sockfd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, &mreq, sizeof(mreq)));
			}
#endif
#ifdef	IPV6
		case AF_INET6: /* IPv6 source-specific API is MCAST_JOIN_SOURCE_GROUP */
#endif
		default:
			errno = EAFNOSUPPORT;
			return(-1);
	}
#endif
}

/*************************************************
* Function:       Mcast_join_source_group()
* Description:    加入一个特定源的多播组 
* Input:		  sockfd---打开的UDP套接口 
*				  *src--- 特定源套接口地址结构
*				  srclen---特定源套接口地址结构长度
*				  *grp---多播组套接口地址结构
*				  grplen---多播组套接口地址结构长度
*				  *ifname---接口名字
*				  ifindex---接口索引
* Output:         N/A 
* Return:         0/-1
*************************************************/
int Mcast_join_source_group(
		int sockfd, 
		const SA *src, socklen_t srclen,
		const SA *grp, socklen_t grplen,
		const char *ifname, u_int ifindex
	)
{
	int rval;

	if((rval = mcast_join_source_group(sockfd, src, srclen, grp, grplen, ifname, ifindex)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_join_source_group error\n");
	}

	return rval;
}

static int mcast_block_source(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen)
{
#ifdef MCAST_BLOCK_SOURCE
	struct group_source_req req;

	req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) 
	{
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, Family_to_level(grp->sa_family), MCAST_BLOCK_SOURCE, &req, sizeof(req)));
#else
	switch (grp->sa_family) 
	{
#ifdef IP_BLOCK_SOURCE
		case AF_INET: 
			{
				struct ip_mreq_source	mreq;

				memcpy(&mreq.imr_multiaddr, &((struct sockaddr_in *) grp)->sin_addr, sizeof(struct in_addr));
				memcpy(&mreq.imr_sourceaddr, &((struct sockaddr_in *) src)->sin_addr, sizeof(struct in_addr));
				mreq.imr_interface.s_addr = htonl(INADDR_ANY);

				return(setsockopt(sockfd, IPPROTO_IP, IP_BLOCK_SOURCE, &mreq, sizeof(mreq)));
			}
#endif
#ifdef	IPV6
		case AF_INET6: /* IPv6 source-specific API is MCAST_BLOCK_SOURCE */
#endif
		default:
			errno = EAFNOSUPPORT;
			return(-1);
	}
#endif
}

/*************************************************
* Function:       Mcast_block_source()
* Description:    阻塞接收从给定单播源到给定多播组的数据报 
* Input:		  sockfd---打开的UDP套接口 
*				  *src--- 单播源套接口地址结构
*				  srclen---单播源套接口地址结构长度
*				  *grp---多播组套接口地址结构
*				  grplen---多播组套接口地址结构长度
* Output:         N/A 
* Return:         0/-1
*************************************************/
int Mcast_block_source(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen)
{
	int rval;
	
	if((rval = mcast_block_source(sockfd, src, srclen, grp, grplen)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_block_source error\n");
	}

	return rval;
}

static int mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen)
{
#ifdef MCAST_UNBLOCK_SOURCE
	struct group_source_req req;

	req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) 
	{
		errno = EINVAL;
		return -1;
	}

	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, Family_to_level(grp->sa_family), MCAST_UNBLOCK_SOURCE, &req, sizeof(req)));
#else
	switch (grp->sa_family)
	{
#ifdef IP_UNBLOCK_SOURCE
		case AF_INET:
			{
				struct ip_mreq_source	mreq;

				memcpy(&mreq.imr_multiaddr, &((struct sockaddr_in *) grp)->sin_addr, sizeof(struct in_addr));
				memcpy(&mreq.imr_sourceaddr, &((struct sockaddr_in *) src)->sin_addr, sizeof(struct in_addr));
				mreq.imr_interface.s_addr = htonl(INADDR_ANY);

				return(setsockopt(sockfd, IPPROTO_IP, IP_UNBLOCK_SOURCE, &mreq, sizeof(mreq)));
			}
#endif

#ifdef	IPV6
		case AF_INET6: /* IPv6 source-specific API is MCAST_UNBLOCK_SOURCE */
#endif
		default:
			errno = EAFNOSUPPORT;
			return(-1);
	}
#endif
}

/*************************************************
* Function:       Mcast_block_source()
* Description:    开通接收从给定单播源到给定多播组的数据报 
* Input:		  sockfd---打开的UDP套接口 
*				  *src--- 单播源套接口地址结构
*				  srclen---单播源套接口地址结构长度
*				  *grp---多播组套接口地址结构
*				  grplen---多播组套接口地址结构长度
* Output:         N/A 
* Return:         0/-1
*************************************************/
int Mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen)
{
	int rval;

	if ((rval = mcast_unblock_source(sockfd, src, srclen, grp, grplen)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_unblock_source error\n");
	}

	return rval;
}


static int mcast_leave(int sockfd, const SA *grp, socklen_t grplen)
{
#ifdef MCAST_JOIN_GROUP
	struct group_req req;

	req.gr_interface = 0;
	if (grplen > sizeof(req.gr_group)) 
	{
		errno = EINVAL;
		return -1;
	}

	memcpy(&req.gr_group, grp, grplen);
	return (setsockopt(sockfd, Family_to_level(grp->sa_family), MCAST_LEAVE_GROUP, &req, sizeof(req)));
#else
	switch (grp->sa_family) 
	{

		case AF_INET: 
			{
				struct ip_mreq		mreq;

				memcpy(&mreq.imr_multiaddr, &((const struct sockaddr_in *) grp)->sin_addr, sizeof(struct in_addr));
				mreq.imr_interface.s_addr = htonl(INADDR_ANY);
				return(setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq)));
			}

		case AF_INET6: 
			{
				struct ipv6_mreq	mreq6;

				memcpy(&mreq6.ipv6mr_multiaddr, &((const struct sockaddr_in6 *) grp)->sin6_addr, sizeof(struct in6_addr));
				mreq6.ipv6mr_interface = 0;
				return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_LEAVE_GROUP, &mreq6, sizeof(mreq6)));
			}

		default:
			errno = EAFNOSUPPORT;
			return(-1);
	}
#endif
}

/*************************************************
* Function:       Mcast_leave()
* Description:    离开一个不限源的多播组 
* Input:		  sockfd---打开的UDP套接口 
*				  *grp---多播组套接口地址结构
*				  grplen---多播组套接口地址结构长度
* Output:         N/A 
* Return:         0/-1
*************************************************/
int Mcast_leave(int sockfd, const SA *grp, socklen_t grplen)
{
	int rval;

	if ((rval = mcast_leave(sockfd, grp, grplen)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_leave error\n");
	}

	return rval;
}

static int mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen)
{
#ifdef MCAST_LEAVE_SOURCE_GROUP
	struct group_source_req req;

	req.gsr_interface = 0;
	if (grplen > sizeof(req.gsr_group) || srclen > sizeof(req.gsr_source)) 
	{
		errno = EINVAL;
		return -1;
	}
	memcpy(&req.gsr_group, grp, grplen);
	memcpy(&req.gsr_source, src, srclen);
	return (setsockopt(sockfd, Family_to_level(grp->sa_family), MCAST_LEAVE_SOURCE_GROUP, &req, sizeof(req)));
#else
	switch (grp->sa_family) 
	{
#ifdef IP_DROP_SOURCE_MEMBERSHIP
		case AF_INET: 
			{
				struct ip_mreq_source	mreq;

				memcpy(&mreq.imr_multiaddr, &((struct sockaddr_in *) grp)->sin_addr, sizeof(struct in_addr));
				memcpy(&mreq.imr_sourceaddr, &((struct sockaddr_in *) src)->sin_addr, sizeof(struct in_addr));
				mreq.imr_interface.s_addr = htonl(INADDR_ANY);

				return(setsockopt(sockfd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, &mreq, sizeof(mreq)));
			}
#endif
#ifdef	IPV6
		case AF_INET6: /* IPv6 source-specific API is MCAST_LEAVE_SOURCE_GROUP */
#endif
		default:
			errno = EAFNOSUPPORT;
			return(-1);
	}
#endif
}

/*************************************************
* Function:		  Mcast_leave_source_group()
* Description:    离开一个特定源的多播组 
* Input:		  sockfd---打开的UDP套接口 
*				  *src--- 单播源套接口地址结构
*				  srclen---单播源套接口地址结构长度
*				  *grp---多播组套接口地址结构
*				  grplen---多播组套接口地址结构长度
* Output:         N/A 
* Return:         0/-1
*************************************************/
int Mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen)
{
	int rval;

	if ( (rval = mcast_leave_source_group(sockfd, src, srclen, grp, grplen)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_leave_source_group error\n");
	}

	return rval;
}

/*============================================================
 *				多播数据报发送相关：3　API
 * ==========================================================*/
static int mcast_set_loop(int sockfd, int onoff)
{
	switch (Sockfd_to_family(sockfd)) 
	{
		case AF_INET: 
			{
				u_char		flag;

				flag = onoff;
				return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, sizeof(flag)));
			}

		case AF_INET6: 
			{
				u_int		flag;

				flag = onoff;
				return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &flag, sizeof(flag)));
			}

		default:
			errno = EAFNOSUPPORT;
			return(-1);
	}
}

/*************************************************
* Function:       Mcast_set_loop()
* Description:    设置回馈套接口选项(默认为打开) 
* Input:		  sockfd---打开的UDP套接口
*				  onoff---1/0 
* Output:         N/A 
* Return:         0/-1
*************************************************/
int Mcast_set_loop(int sockfd, int onoff)
{
	int rval;

	if ((rval = mcast_set_loop(sockfd, onoff)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_set_loop error\n");
	}
	
	return rval;
}

static int mcast_get_loop(int sockfd)
{
	switch (Sockfd_to_family(sockfd)) 
	{
		case AF_INET:
			{
				u_char		flag;
				socklen_t	len;

				len = sizeof(flag);
				if (getsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, &len) < 0)
				{
					return(-1);
				}
				return(flag);
			}

		case AF_INET6:
			{
				u_int		flag;
				socklen_t	len;

				len = sizeof(flag);
				if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &flag, &len) < 0)
				{
					return(-1);
				}
				return(flag);
			}

		default:
			errno = EAFNOSUPPORT;
			return(-1);
	}
}

/*************************************************
* Function:       Mcast_get_loop()
* Description:    获取回馈套接口选项(默认为打开) 
* Input:		  sockfd---打开的UDP套接口
* Output:         N/A 
* Return:         0/1/-1
*************************************************/
int Mcast_get_loop(int sockfd)
{
	int		rval;

	if ( (rval = mcast_get_loop(sockfd)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_get_loop error\n");
	}

	return(rval);
}

static int mcast_set_if(int sockfd, const char *ifname, u_int ifindex)
{
	switch (Sockfd_to_family(sockfd))
	{
		case AF_INET: 
			{
				struct in_addr		inaddr;
				struct ifreq		ifreq;

				if (ifindex > 0) 
				{
					if (if_indextoname(ifindex, ifreq.ifr_name) == NULL) 
					{
						errno = ENXIO;	/* i/f index not found */
						return(-1);
					}
					if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
					{
						return(-1);
					}
					memcpy(&inaddr, &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
				} 
				else if (ifname != NULL) 
				{
					strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
					if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
					{
						return(-1);
					}
					memcpy(&inaddr, &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
				} 
				else
				{
					inaddr.s_addr = htonl(INADDR_ANY);	/* remove prev. set default */
				}

				return (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &inaddr, sizeof(struct in_addr)));
			}

		case AF_INET6: 
			{
				u_int	idx;

				if ( (idx = ifindex) == 0) 
				{
					if (ifname == NULL) 
					{
						errno = EINVAL;	/* must supply either index or name */
						return(-1);
					}
					if ( (idx = if_nametoindex(ifname)) == 0) 
					{
						errno = ENXIO;	/* i/f name not found */
						return(-1);
					}
				}
				return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_IF, &idx, sizeof(idx)));
			}

		default:
			errno = EAFNOSUPPORT;
			return(-1);
	}
}

/*************************************************
* Function:       Mcast_set_if()
* Description:    设置外出多播数据报的缺省接口索引 
* Input:		  sockfd---打开的UDP套接口
*				  ifname---接口名字
*				  ifindex---接口索引
* Output:         N/A 
* Return:         0/-1
*************************************************/
int Mcast_set_if(int sockfd, const char *ifname, u_int ifindex)
{
	int rval;

	if ((rval = mcast_set_if(sockfd, ifname, ifindex)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_set_if error\n");
	}

	return rval;
}

static int mcast_get_if(int sockfd)
{
	switch (Sockfd_to_family(sockfd)) 
	{
		case AF_INET: 
			{
				/* TODO: similar to mcast_set_if() */
				return(-1);
			}

		case AF_INET6: 
			{
				u_int		idx;
				socklen_t	len;

				len = sizeof(idx);
				if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_IF, &idx, &len) < 0)
				{
					return(-1);
				}
				return(idx);
			}

		default:
			errno = EAFNOSUPPORT;
			return(-1);
	}
}

/*************************************************
* Function:       Mcast_get_if()
* Description:    获取外出多播数据报的缺省接口索引 
* Input:		  sockfd---打开的UDP套接口
* Output:         N/A 
* Return:         0/-1
*************************************************/
int Mcast_get_if(int sockfd)
{
	int		rval;

	if ( (rval = mcast_get_if(sockfd)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_get_if error\n");
	}

	return(rval);
}

static int mcast_set_ttl(int sockfd, int val)
{
	switch (Sockfd_to_family(sockfd)) 
	{
		case AF_INET: 
			{
				u_char		ttl;

				ttl = val;
				return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)));
			}

		case AF_INET6: 
			{
				int		hop;

				hop = val;
				return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hop, sizeof(hop)));
			}

		default:
			errno = EAFNOSUPPORT;
			return(-1);
	}
}

/*************************************************
* Function:       Mcast_set_ttl()
* Description:    设置IPv4的TTL或IPv6的跳限 
* Input:		  sockfd---打开的UDP套接口
* Output:         N/A 
* Return:         0/-1
*************************************************/
int Mcast_set_ttl(int sockfd, int val)
{
	int rval;

	if ((rval = mcast_set_ttl(sockfd, val)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_set_ttl error\n");
	}

	return rval;
}

static int mcast_get_ttl(int sockfd)
{
	switch (Sockfd_to_family(sockfd)) 
	{
	case AF_INET: 
		{
		u_char		ttl;
		socklen_t	len;

		len = sizeof(ttl);
		if (getsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, &len) < 0)
		{
			return(-1);
		}
		return(ttl);
	}

	case AF_INET6: 
		{
		int			hop;
		socklen_t	len;

		len = sizeof(hop);
		if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hop, &len) < 0)
		{
			return(-1);
		}
		return(hop);
	}

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

/*************************************************
* Function:       Mcast_get_ttl()
* Description:    获取IPv4的TTL或IPv6的跳限 
* Input:		  sockfd---打开的UDP套接口
* Output:         N/A 
* Return:         ttl/hops/-1
*************************************************/
int Mcast_get_ttl(int sockfd)
{
	int		rval;

	if ( (rval = mcast_get_ttl(sockfd)) < 0)
	{
		debug_info(DEBUG_LEVEL_3, "mcast_get_ttl error\n");
	}

	return(rval);
}

#endif

