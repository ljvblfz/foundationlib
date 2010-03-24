/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: foundationSocket.h 
 *  Description: this file contains api list of socket operations 
 *  Author: caoyun
 *  Create:2010-01-25 
 *  Modification history:
 *  2010-01-25, created the file,           caoyun
 *  2010-03-09, refreshed the file,         chenwangxian
 *
 *
 **************************************************************************************/ 
#ifndef _SOCKET_API_H
#define _SOCKET_API_H

#include "foundationInclude.h"
#include "foundationDataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define INET_ADDR_STRLEN	INET_ADDRSTRLEN
#define INET6_ADDR_STRLEN	INET6_ADDRSTRLEN
#define SA					struct sockaddr

/*=====================================================
 *					sock API
 * ===================================================*/
int Sock_pton(struct sockaddr *sa, const char *ipStr);
int Sock_pton_v4(const char *ipStr);
int Sock_ntop(const struct sockaddr *sa, char *ipStr);
int Sock_ntop_v4(int ipaddr, char *ipStr);

/*=====================================================
 *					SOCKOPT API
 * ===================================================*/
//基本sockopt API
int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
//高级socket API
int Sockopt_get_recvBuf(int sockfd, int *bufsize);
int Sockopt_set_recvBuf(int sockfd, int bufsize);
int Sockopt_get_sendBuf(int sockfd, int *bufsize);
int Sockopt_set_sendBuf(int sockfd, int bufsize);
int Sockopt_set_lingerOn(int sockfd);
int Sockopt_set_reuse(int sockfd);
//
int Family_to_level(int family);
int Sockfd_to_family(int sockfd);

/*=====================================================
 *					NET IO  API
 * ===================================================*/
int Recv(int sockfd, void *buf, size_t len, int flags);
int Send(int sockfd, void *buf, size_t len, int flags);
int readn(int connfd, void *vptr, int n);
int writen(int connfd, void *vptr, size_t n);

/*=====================================================
 *					net ioctl wrapper API
 * ===================================================*/
int ioctl_sockBuf_datas(int sockfd);
int Sock_noBlock(int sockfd);
int Sock_block(int sockfd);

int Socket(int family, int type, int protocol);
int Tcp_socket(int bufsize);
int Udp_socket(int bufsize);
int Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);
int Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
int Close(int sockfd);
int Getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrlen);
int Getpeername(int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen);
//高级TCP API
int connectWithTimeout(int sock, struct sockaddr*addrs, int adrsLen,struct timeval* tm);
int Tcp_connect(int serverIp, UINT16 serverPort, int bufsize);
int Tcp_timedconnect(int serverIp, UINT16 serverPort, int bufsize, struct timeval delaytime);

/*=====================================================
 *					UDP API
 * ===================================================*/
int	Recvfrom(int sockfd, void *buf, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen) ;
int Sendto(int sockfd, const void *buf, size_t nbytes, int flags, const struct sockaddr *to, socklen_t addrlen);

/*=====================================================
 *					MCAST API
 * ===================================================*/
//MCAST接收相关
int Mcast_join(int sockfd, const SA *grp, socklen_t grplen, const char *ifname, u_int ifindex);
int Mcast_join_source_group(
		int sockfd, 
		const SA *src, socklen_t srclen,
		const SA *grp, socklen_t grplen,
		const char *ifname, u_int ifindex
	);
int Mcast_block_source(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen);
int Mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen);
int Mcast_leave(int sockfd, const SA *grp, socklen_t grplen);
int Mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen);
//MCAST发送相关
int Mcast_set_loop(int sockfd, int onoff);
int Mcast_get_loop(int sockfd);
int Mcast_set_if(int sockfd, const char *ifname, u_int ifindex);
int Mcast_get_if(int sockfd);
int Mcast_set_ttl(int sockfd, int val);
int Mcast_get_ttl(int sockfd);


#ifdef __cplusplus
}
#endif

#endif

