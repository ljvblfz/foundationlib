#include <stdio.h>
#include <stdlib.h>
#include "libtype.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>

#define TEST_MCAST_IP           "239.255.255.250"
#define TEST_MCAST_PORT         3702
#define HELLO_MSG_FILE_NAME     "hello.xml"

int mcast_create(char* mcast_addr, short port, int mcast_ttl, int sock_buflen)
{
    int sockfd;
    struct sockaddr_in local_addr, mcast_sock_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        return -1;
    }

    if (sock_buflen > 0)
    {
        if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sock_buflen, sizeof(sock_buflen)) == -1)
        {
            close(sockfd);
            return -1;
        }
    }

    memset(&local_addr, 0x00, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(port);
    
    if (bind(sockfd,(struct sockaddr*) local_addr, sizeof(struct sockaddr_in)) == -1)
    {
        close(sockfd);
        return -1;
    }
    
    if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, (void*)mcast_ttl, sizeof(mcast_ttl)) == -1)
    {
        close(sockfd);
        return -1;
    }

    memset(&mcast_sock_addr, 0x00, sizeof(mcast_sock_addr));
    mcast_sock_addr.sin_family = AF_INET;
    inet_aton(mcast_addr, &mcast_sock_addr.sin_addr);
    mcast_sock_addr.sin_addr.s_addr = mcast_sock_addr.sin_addr.s_addr;
    mcast_sock_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&mcast_sock_addr, sizeof(struct sockaddr_in)) == -1)
    {
        close(sockfd);
        return -1;
    }
    
    return sockfd;
}

int mcast_join(char* mcast_addr, short port, int sock_buflen)
{
    int sockfd;
    struct sockaddr_in local_addr, mcast_sock_addr;
    struct ip_mreq ipMrep;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        return -1;
    }

    if (sock_buflen > 0)
    {
        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &sock_buflen, sizeof(sock_buflen)) == -1)
        {
            close(sockfd);
            return -1;
        }
    }
    
    memset(&local_addr, 0x00, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(port);
    if (bind(sockfd,(struct sockaddr*) local_addr, sizeof(struct sockaddr_in)) == -1)
    {
        close(sockfd);
        return -1;
    }
    
    ipMrep.imr_multiaddr.s_addr = inet_addr(mcast_addr);
    ipMrep.imr_interface.s_addr = htonl(INADDR_ANY);
    
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&ipMrep, sizeof(ipMrep)) == -1)
    {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int mcast_server_proc(void)
{
    int mcast_sock;
    FILE* fp;
    unsigned char buf[128];
    int rcount, wcount;

    mcast_sock = mcast_create(TEST_MCAST_IP, TEST_MCAST_PORT, 1, 64*1024);
    if (mcast_sock == -1)
    {
        printf("mcast_create error!\n");
        return -1;
    }
    
    fp = fopen(HELLO_MSG_FILE_NAME, "rb");
    
    while (1)
    {
        rcount = fread(buf, 1, 128, fp);
        if (rcount == -1 || rcount == EOF)
        {
            fseek(fp, 0, SEEK_SET);
            sleep(1);
        }
        
        wcount = send(mcast_sock, buf, rcount, 0);
        if (wcount == -1)
        {
            printf("MCAST send error!\n");
            sleep(1);
        }
    }
    
    return 0;
}

int mcast_client_proc(void)
{
    int mcast_sock;
    int recv_count, i;
    char* buf_ptr;
    
    mcast_sock = mcast_join(TEST_MCAST_IP, TEST_MCAST_PORT, 128*1024);
    if (mcast_sock == -1)
    {
        printf("mcast_join error!\n");
        return -1;
    }
    
    buf_ptr = malloc(128*1024);

    while (1)
    {
        recv_count = recv(mcast_sock, buf_ptr, 128*1024, 0);
        if (recv_count == -1)
        {
            sleep(1);
        }
        
        for (i=0;i<recv_count;i++)
        {
            printf("%c", buf_ptr[i]);
        }
    }

    return 0;
}

int main (void)
{

}