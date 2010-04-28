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

/*=====================================================
 * common struct infomation 
 struct sockaddr 
 {
 	u_char	sa_len;              total length 
 	u_char	sa_family;           address family 
 	char	sa_data[14];         actually longer; address value 
 };
 struct sockaddr_in 
 {
	u_char	sin_len;
	u_char	sin_family;
	u_short	sin_port;
	struct	in_addr sin_addr;
	char	sin_zero[8];
 };
 struct in_addr 
 {
	u_long s_addr;
 };
 * ===================================================*/
#ifndef SA
#define SA					struct sockaddr
#endif

/*=====================================================
 *					Basic API
 int Socket(int family, int type, int protocol);
 int Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);
 int Listen(int sockfd, int backlog);
 int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
 int Close(int sockfd);
 int Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);

 int ConnectWithTimeout(int sock, struct sockaddr*addrs, int adrsLen,struct timeval* tm);
 int Recv(int sockfd, void *buf, size_t len, int flags);
 int Send(int sockfd, void *buf, size_t len, int flags);
 int	Recvfrom(int sockfd, void *buf, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen) ;
 int Sendto(int sockfd, const void *buf, size_t nbytes, int flags, const struct sockaddr *to, socklen_t addrlen);
 int Getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrlen);
 int Getpeername(int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen);

 int Tcp_socket(int bufsize);
 int Udp_socket(int bufsize);

 int Tcp_connectToServer(int serverIp, UINT16 serverPort, int bufsize);
 int Tcp_timedconnectToServer(int serverIp, UINT16 serverPort, int bufsize, struct timeval delaytime);

 int readn(int connfd, void *vptr, int n);
 int writen(int connfd, void *vptr, size_t n);
 * ===================================================*/

/*=====================================================
 *					SOCKOPT API
 int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
 int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

 int GetsockRecvBuf(int sockfd, int *bufsize);
 int SetsockRecvBuf(int sockfd, int bufsize);

 int GetsockSendBuf(int sockfd, int *bufsize);
 int SetsockSendBuf(int sockfd, int bufsize);

 int SetsockLingerOn(int sockfd);
 int SetsockReuse(int sockfd);

 int GetsockNReadBytes(int sockfd);
 int SetsockNBlock(int sockfd);
 int SetsockBlock(int sockfd);
 * ===================================================*/

/*=====================================================
 *					sockaddr API
 int Sock_pton(struct sockaddr *sa, const char *ipStr);
 int Sock_ntop(const struct sockaddr *sa, char *ipStr);

 int Sock_pton_v4(const char *ipStr);
 int Sock_ntop_v4(int ipaddr, char *ipStr);
 * ===================================================*/

/*
 * =====================================================================
 * Function:Socket()
 * Description: open a socket
 * Input:   family -- AF_INET AF_INET6
 *          type -- SOCK_STREAM, SOCK_DGRAM, or SOCK_RAW
 *          protocol -- socket protocol (usually 0)
 * Output:  N/A
 * Return:  A socket descriptor, or ERROR. 
 *======================================================================
 */
int Socket(int family, int type, int protocol);

/*
 * =====================================================================
 * Function:Bind()
 * Description: bind a address to a socket
 * Input:   sockfd -- socket descriptor
 *          myaddr -- address to be bound
 *          addrlen -- length of address
 * Output:  N/A
 * Return:  0 if success, -1 if failed 
 *======================================================================
 */
int Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);

/*
 * =====================================================================
 * Function:Listen()
 * Description: This routine enables connections to a socket. 
 * It also specifies the maximum number of unaccepted connections that can 
 * be pending at one time (backlog). After enabling connections with listen( ), 
 * connections are actually accepted by accept( ). 
 * Input:   sockfd -- socket descriptor
 *          backlog -- number of connections
 * Output:  N/A
 * Return:  0 if success, -1 if failed 
 *======================================================================
 */
int Listen(int sockfd, int backlog);

/*
 * =====================================================================
 * Function:Accept()
 * Description: This routine accepts a connection on a socket, 
 * and returns a new socket created for the connection. 
 * The socket must be bound to an address with bind( ), 
 * and enabled for connections by a call to listen( ). 
 * NOTE:    It blocks the caller until a connection is present, 
 *          unless the socket is marked as non-blocking.
 * Input:   sockfd -- socket descriptor
 * Output:  
 *          cliaddr -- peer address to save
 *          addrlen -- peer address length to save
 * Return:  0 if success, -1 if failed 
 *======================================================================
 */
int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);

/*
 * =====================================================================
 * Function:Close()
 * Description: close the socket
 * Input:   sockfd -- socket descriptor
 * Output:  N/A
 * Return:  0 if success, -1 if failed 
 *======================================================================
 */
int Close(int sockfd);

/*
 * =====================================================================
 * Function:Connect()
 * Description: initiate a connection to a socket
 * Input:   sockfd -- socket descriptor
 *          servaddr -- addr of socket to connect
 *          addrlen -- length of server addr, in bytes
 * Output:  N/A
 * Return:  0 if success, -1 if failed 
 *======================================================================
 */
int Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);

/*
 * =====================================================================
 * Function:ConnectWithTimeout()
 * Description: attempt to connect server within a duration
 * Input:   sockfd -- socket descriptor
 *          servaddr -- addr of socket to connect
 *          addrlen -- length of server addr, in bytes
 *          tm -- timeout value
 * Output:  N/A
 * Return:  0 if success, -1 if failed 
 *======================================================================
 */
int ConnectWithTimeout(int sockfd, const struct sockaddr*serveraddr, int addrlen,struct timeval* tm);

/*
 * =====================================================================
 * Function:Recv()
 * Description: receive data from a socket
 * Input:   sockfd -- socket descriptor
 *          len -- length of buf
 *          flags -- flags to underlying protocols
 * Output:  
 *          buf -- space to save data
 * Return:  number of bytes received if success, -1 if failed 
 *======================================================================
 */
int Recv(int sockfd, void *buf, size_t len, int flags);

/*
 * =====================================================================
 * Function:Send()
 * Description: send data to a socket
 * Input:   sockfd -- socket descriptor
 *          buf -- data need to send
 *          len -- length of data to be sent
 *          flags -- flags to underlying protocols
 * Output:  N/A
 * Return:  number of bytes send if success, -1 if failed 
 *======================================================================
 */
int Send(int sockfd, void *buf, size_t len, int flags);

/*
 * =====================================================================
 * Function:Recvfrom()
 * Description: receive data from a udp socket regardless of whether it is connected.
 * Input:   sockfd -- socket descriptor
 *          nbytes -- length of buf
 *          flags -- flags to underlying protocols
 * Output:  
 *          buf -- space to save data
 *          from -- where to copy sender's address.NULL to ignored.
 *          addrlen -- length of "from" saved.NULL to ignored. 
 * Return:  number of bytes received if success, -1 if failed 
 *======================================================================
 */
int	Recvfrom(int sockfd, void *buf, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen) ;

/*
 * =====================================================================
 * Function:Send()
 * Description: send data to a udp socket name by "to"
 * Input:   sockfd -- socket descriptor
 *          buf -- data need to send
 *          nbytes -- length of data to be sent, limited by udp buffer size.
 *          flags -- flags to underlying protocols
 *          to -- send to the address
 *          addrlen -- length of destanation
 * Output:  N/A
 * Return:  number of bytes send if success, -1 if failed 
 *======================================================================
 */
int Sendto(int sockfd, const void *buf, size_t nbytes, int flags, const struct sockaddr *to, socklen_t addrlen);

/*
 * =====================================================================
 * Function:Getsockname()
 * Description: get local socket address
 * Input:   sockfd -- socket descriptor
 * Output:  
 *          localaddr -- local socket address
 *          addrlen -- length filled by localaddr, in bytes
 * Return:  0 if success, -1 if failed 
 *======================================================================
 */
int Getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrlen);

/*
 * =====================================================================
 * Function:Getpeername()
 * Description: get remote connected socket address
 * Input:   sockfd -- socket descriptor
 * Output:  
 *          peeraddr -- remote connected socket address
 *          addrlen -- length filled by peeraddr, in bytes
 * Return:  0 if success, -1 if failed 
 *======================================================================
 */
int Getpeername(int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen);

/*
 * =====================================================================
 * Function:Tcp_socket()
 * Description: open a tcp socket of buffsize
 * Input:   bufsize -- tcp socket buffsize
 * Output:  N/A
 * Return:  A socket descriptor, or ERROR. 
 *======================================================================
 */
int Tcp_socket(int bufsize);

/*
 * =====================================================================
 * Function:Udp_socket()
 * Description: open a udp socket of buffsize
 * Input:   bufsize -- udp socket buffsize
 * Output:  N/A
 * Return:  A socket descriptor, or ERROR. 
 *======================================================================
 */
int Udp_socket(int bufsize);

/*
 * =====================================================================
 * Function:Tcp_connectToServer()
 * Description: open a tcp socket of buffsize, and connect to a server 
 * Input:   serverIp -- IP of connected server
 *          serverPort -- Port of connected server
 *          bufsize -- size of socket buffer
 * NOTE: It will block always.
 * Output:  N/A
 * Return:  A socket descriptor, or ERROR. 
 *======================================================================
 */
int Tcp_connectToServer(int serverIp, UINT16 serverPort, int bufsize);

/*
 * =====================================================================
 * Function:Tcp_timedconnectToServer()
 * Description: open a tcp socket of buffsize, and connect to a server within a duration
 * Input:   serverIp -- IP of connected server
 *          serverPort -- Port of connected server
 *          bufsize -- size of socket buffer
 *          delaytime -- timeout value
 * Output:  N/A
 * Return:  A socket descriptor, or ERROR. 
 *======================================================================
 */
int Tcp_timedconnectToServer(int serverIp, UINT16 serverPort, int bufsize, struct timeval delaytime);

/*
 * =====================================================================
 * Function:readn()
 * Description: read specified bytes of data
 * Input:   connfd -- IP of connected server
 *          n -- bytes specified to read
 * Output:  
 *          vptr -- where to save data
 * Return:  bytes that read if success, -1 if failed.
 *======================================================================
 */
int readn(int connfd, void *vptr, int n);

/*
 * =====================================================================
 * Function:writen()
 * Description: writen specified bytes of data
 * Input:   connfd -- IP of connected server
 *          vptr -- data that need to write
 *          n -- bytes specified to write 
 * Output:  N/A
 * Return:  bytes that writen if success, -1 if failed.
 *======================================================================
 */
int writen(int connfd, void *vptr, size_t n);


/*
 * =====================================================================
 * Function:Getsockopt()
 * Description: get socket option
 * Input:   sockfd -- socket
 *          level --  To manipulate options at the "socket" level, level should be SOL_SOCKET
 *          optname -- name of options
 * Output:  
 *          optval -- where to put option
 *          optlen -- where to put option length
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);

/*
 * =====================================================================
 * Function:Setsockopt()
 * Description: Set socket option
 * Input:   sockfd -- socket
 *          level --  To manipulate options at the "socket" level, level should be SOL_SOCKET
 *          optname -- name of options
 *          optval -- pointer to option value
 *          optlen -- optval length
 * Output:  N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

/*
 * =====================================================================
 * Function:GetsockRecvBuf()
 * Description: get socket receive buffer length
 * Input:   sockfd -- socket
 * Output:  
 *          bufsize -- where to save length
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int GetsockRecvBuf(int sockfd, int *bufsize);

/*
 * =====================================================================
 * Function:SetsockRecvBuf()
 * Description: set socket receive buffer length
 * Input:   sockfd -- socket
 *          bufsize -- new length of socket receive buffer. 0 as default.
 * Output:  N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int SetsockRecvBuf(int sockfd, int bufsize);

/*
 * =====================================================================
 * Function:GetsockSendBuf()
 * Description: Get socket send buffer length
 * Input:   sockfd -- socket
 * Output:  
 *          bufsize -- where to save length
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int GetsockSendBuf(int sockfd, int *bufsize);

/*
 * =====================================================================
 * Function:SetsockSendBuf()
 * Description: set socket send buffer length
 * Input:   sockfd -- socket
 *          bufsize -- new length of socket send buffer. 0 as default.
 * Output:  N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int SetsockSendBuf(int sockfd, int bufsize);

/*
 * =====================================================================
 * Function:SetsockLingerOn()
 * Description: set socket close and release force.
 * Input:   sockfd -- socket
 * Output:  N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int SetsockLingerOn(int sockfd);

/*
 * =====================================================================
 * Function:SetsockReuse()
 * Description: set socket use local address again
 * Input:   sockfd -- socket
 * Output:  N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int SetsockReuse(int sockfd);

/*
 * =====================================================================
 * Function:GetsockNReadBytes()
 * Description: get socket not read bytes in receive buffer
 * Input:   sockfd -- socket
 * Output:  N/A
 * Return:  bytes of data in receive buffer if success, -1 if failed.
 *======================================================================
 */
int GetsockNReadBytes(int sockfd);

/*
 * =====================================================================
 * Function:SetsockNBlock()
 * Description: set socket non-block
 * Input:   sockfd -- socket
 * Output:  N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int SetsockNBlock(int sockfd);

/*
 * =====================================================================
 * Function:SetsockBlock()
 * Description: set socket block
 * Input:   sockfd -- socket
 * Output:  N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int SetsockBlock(int sockfd);

/*=====================================================
 *					sockaddr API
 int Sock_pton(struct sockaddr *sa, const char *ipStr);
 int Sock_ntop(const struct sockaddr *sa, char *ipStr);

 int Sock_pton_v4(const char *ipStr);
 int Sock_ntop_v4(int ipaddr, char *ipStr);
 * ===================================================*/

/*
 * =====================================================================
 * Function:Sock_pton()
 * Description: convert dot string ip into a network address
 * Input:   ipStr -- dot string ip
 * Output:  sa -- network address
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Sock_pton(const char *ipStr, struct sockaddr *sa);

/*
 * =====================================================================
 * Function:Sock_ntop()
 * Description: convert network address into a dot string ip 
 * Input:   sa -- network address
 * Output:  
 *          ipStr -- dot string ip
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Sock_ntop(const struct sockaddr *sa, char *ipStr);

/*
 * =====================================================================
 * Function:Sock_pton_v4()
 * Description: convert dot string ip into a network address, just ipv4
 * Input:   
 *          ipStr -- dot string ip
 * Output:  N/A
 * Return:  network address if success, -1 if failed.
 *======================================================================
 */
int Sock_pton_v4(const char *ipStr);

/*
 * =====================================================================
 * Function:Sock_ntop_v4()
 * Description: convert network address into a dot string ip, just ipv4 
 * Input:   ipaddr -- network address
 * Output:  
 *          ipStr -- dot string ip
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Sock_ntop_v4(int ipaddr, char *ipStr);


/*=====================================================
 *					MCAST API
 * ===================================================*/
/* mcast receiving operation */
/* 
 * 
 struct ip_mreq 
 {
    struct in_addr imr_multiaddr;	 IP multicast address of group 
    struct in_addr imr_interface;	 local IP address of interface 
 };

 struct ip_mreq_source 
 {
    uint32 imr_multiaddr;
    uint32 imr_interface;
    uint32 imr_sourceaddr;
 };

 struct in_addr 
 {
	u_long s_addr;
 };
 */

/*
 * =====================================================================
 * Function:Mcast_add_member()
 * Description: set local socket add to a mcast group, receive from any source.
 * Input:   sockfd
 *          ip_mreq -- struct of ip_mreq
 * Output:  
 *          N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Mcast_add_member(int sockfd, struct ip_mreq * ip_mreq);

/*
 * =====================================================================
 * Function:Mcast_drop_member()
 * Description: set local socket leave from a mcast group 
 * Input:   sockfd
 *          ip_mreq -- struct of ip_mreq
 * Output:  
 *          N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Mcast_drop_member(int sockfd, struct ip_mreq * ip_mreq);

/*
 * =====================================================================
 * Function:Mcast_block_source()
 * Description: set local socket stop receive from specified source in a mcast group.
 * Input:   sockfd
 *          ip_mreq -- struct of ip_mreq_source
 * Output:  
 *          N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Mcast_block_source(int sockfd, struct ip_mreq_source * ip_mreq);

/*
 * =====================================================================
 * Function:Mcast_unblock_source()
 * Description: set local socket receive from specified source in a mcast group, while it is blocked.
 * Input:   sockfd
 *          ip_mreq -- struct of ip_mreq_source
 * Output:  
 *          N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */

int Mcast_unlbock_source(int sockfd, struct ip_mreq_source * ip_mreq);
/*
 * =====================================================================
 * Function:Mcast_add_source_member()
 * Description: set local socket add to a mcast group.
 *              But receive just from a specified source, others will be blocked.
 * Input:   sockfd
 *          ip_mreq -- struct of ip_mreq_source
 * Output:  
 *          N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Mcast_add_source_member(int sockfd, struct ip_mreq_source * ip_mreq);

/*
 * =====================================================================
 * Function:Mcast_drop_source_member()
 * Description: set local socket stop receiving from specified source in a mcast group,
 *              others in the group can work normal.
 * Input:   sockfd
 *          ip_mreq -- struct of ip_mreq_source
 * Output:  
 *          N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Mcast_drop_source_member(int sockfd, struct ip_mreq_source * ip_mreq);


/* mcast sending operation */

/*
 * =====================================================================
 * Function:Mcast_set_if()
 * Description: Select a Default Interface for Outgoing Multicasts
 * Input:   sockfd
 *          if_addr -- outgoing network address
 * Output:  
 *          N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Mcast_set_if(int sockfd, struct in_addr * if_addr);

/*
 * =====================================================================
 * Function:Mcast_set_loop()
 * Description: Enable or Disable Loopback
 * Input:   sockfd
 *          onoff -- 1:enable 0:disable
 * Output:  
 *          N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Mcast_set_loop(int sockfd, int onoff);

/*
 * =====================================================================
 * Function:Mcast_set_ttl()
 * Description: Select a Default TTL
 * Input:   sockfd
 *          val -- time to live
 * Output:  
 *          N/A
 * Return:  0 if success, -1 if failed.
 *======================================================================
 */
int Mcast_set_ttl(int sockfd, int val);


#ifdef __cplusplus
}
#endif

#endif

