#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/route.h>
#include <linux/sockios.h> 
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

#include "sysdep.h"

static int get_mac(char * interface_name,char * mac ,int len);

void get_system_time(uuid_time_t *uuid_time)
{
    struct timeval tp;

    gettimeofday(&tp, (struct timezone *)0);

    /* Offset between UUID formatted times and Unix formatted times.
       UUID UTC base time is October 15, 1582.
       Unix base time is January 1, 1970.*/
    *uuid_time = ((unsigned64_t)tp.tv_sec * 10000000)
        + ((unsigned64_t)tp.tv_usec * 10)
        + I64(0x01B21DD213814000);
}

/* system dependent call to get IEEE node ID.
   This sample implementation generates a random node ID. */
INT32_T get_ieee_node_identifier(uuid_node_t *node)
{
    if (get_mac(node->if_name,node->nodeID,6) == ERROR)
    {
        return ERROR;
    }
    
    return OK;
}

/*
 *	Function: get_mac
 *	Description: Get MAC address
 *	Input:
 *			interface_name
 *			len: length of MAC address.
 *	Output:
 *			mac: MAC address.
 *	Return:
 *			ERROR, OK.
 */
static int get_mac(char * interface_name,char * mac ,int len)
{   
	int s;
	struct ifreq ifr;
	u_char* ptr;

	if(strcmp(interface_name,"lo") == 0)
	{
		return ERROR;
	}

	if(len < 6) 
	{
		printf("The mac need 6 byte, but len=%d\n",len);
		return ERROR;
	}


	if((s = socket(PF_INET,SOCK_STREAM,0)) < 0) 
	{
		perror("Socket");
		return ERROR;
	}

	strcpy(ifr.ifr_name,interface_name);

	if(ioctl(s,SIOCGIFHWADDR,&ifr) != 0)
	{
		printf("get_mac_num ioctl error and errno=%d\n",errno);
		close(s);
		return ERROR;
	}

	ptr =(u_char *)&ifr.ifr_ifru.ifru_hwaddr.sa_data[0]; 
	close(s);
    memcpy(mac, ptr, min(len, 6));
    
	return OK;
}

