/*
*	tool.c
*	tool C code file.
*	This code file include some frequently used elemental tool functions
*	in this project.
*	
*	Foundation Services, Tool Module.
*	Based on SDK5.RC16, Linux 2.6.20, ARM Host CPU.
*	
*	MG3500 (MAXIM, mobilygen, Inc.) HD project
*   Copyright (c) Artificial Intelligence Infinity.
*
*	2009.07.24 Solution designed by shaobin, Programmed by shaobin.
*/

/* Standard library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LINUX_OS
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <asm/ioctl.h>
#include <sys/vfs.h>
#include <stdarg.h>
/* Network */
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <net/if.h>
#include <resolv.h>
#include <linux/sockios.h>

#endif


#include "tool.h"

/*
 *	Description:
 *	
 *	
 *	Parameters:
 *	
 *	
 *	Return:
 *		
 */

#ifdef WINDOWS_OS

int getline(char** lineptr, int* n, FILE* fp)
{
	int count;
	long cur_offset;
	int ci;
	char* readbuf;
	
	cur_offset=ftell(fp);
	
	count=0;
	while ((ci=fgetc(fp)) != '\n' && ci != EOF)
	{
		count++;
	}
	
	if (count == 0)
	{
		return OSI_ERROR;
	}
	
	count++; /* For storing extra '\0' char */
	if (*lineptr == NULL || *n < count)
	{
		if ((*lineptr=(char*)realloc(*lineptr,count))==NULL)
		{
			return OSI_ERROR;
		}
	}
	*n=count;

	fseek(fp,cur_offset,SEEK_SET);
	readbuf=*lineptr;
	count=0;
	while ((ci=fgetc(fp)) != '\n' && ci != EOF)
	{
		readbuf[count++]=(char)ci;
	}
	
	readbuf[count]='\0';
	
	return count;
}

#endif

/*
 *	Function: findstr
 *	Description: find a matched string from a original string.
 *	Input:
 *			original_str:	Original string.
 *			match_str:		Match string.
 *	Return:
 *			OSI_ERROR or index of matched string.
 */
int findstr(char* original_str, char* match_str)
{
	int index=0;

	if (original_str==NULL || match_str==NULL)
	{
		return OSI_ERROR;
	}

	while (original_str[index]!='\0')
	{
		if (original_str[index]==match_str[0])
		{
			if (strcmp(original_str+index, match_str)==0)
			{
				return index;
			}
		}
		index++;
	}

	return OSI_ERROR;
}



int TCPLinkToServer(ULONG preAddr,UINT16 port,int bufsize)
{
#ifdef LINUX_OS
	int s;
	struct	sockaddr_in	peeraddr_in;
	int optlen;

	/*
	*	Parameters check
	*/
	if (preAddr==0 || port==0)
	{
		return OSI_ERROR;
	}

	if ((s=socket(AF_INET,SOCK_STREAM,0))==OSI_ERROR)
	{
		printf("Create socket fail!\n");
		return OSI_ERROR;
	}

	if (bufsize>0)
	{
		if (setsockopt (s, SOL_SOCKET, SO_RCVBUF, (char *)&bufsize, sizeof (bufsize))==OSI_ERROR)
		{
			printf("Set socket buffer size error, use the default buffer size!\n");
		}
	}

	/* Bind IP address but chosen any port */
	peeraddr_in.sin_family      = AF_INET;
	peeraddr_in.sin_addr.s_addr = INADDR_ANY;
	peeraddr_in.sin_port        = 0;    
	if (bind (s, (struct sockaddr*)&peeraddr_in, sizeof (peeraddr_in))< 0)
	{
		printf("bind error\n");
		close (s);
		return OSI_ERROR;
	}	
	peeraddr_in.sin_addr.s_addr = preAddr;/*(int) inet_addr (preAddr);*/
	peeraddr_in.sin_port        = htons (port);	

	if (connect (s, (struct sockaddr *)&peeraddr_in, sizeof(peeraddr_in)) == OSI_ERROR)
	{
		close(s);    
		return OSI_ERROR;    
	}

	return s;
#endif
	
#ifdef WINDOWS_OS

	return OSI_OK;
#endif
}

int atomread(int fd,void* buf, int len)
{

#ifdef LINUX_OS
	fd_set rset;
	struct timeval select_timeout;
	int remlen,rlen,ret,count=60;

	remlen=len;
	rlen=0;
	while (remlen>0 && count--)
	{
		FD_ZERO(&rset);
		FD_SET(fd,&rset);
		select_timeout.tv_sec=1;
		select_timeout.tv_usec=0;
		if (select(fd+1,&rset,NULL,NULL,&select_timeout)>0)
		{
			ret=read(fd,buf+rlen,remlen);
			if (ret<=0)
			{
				return OSI_ERROR;
			}
			remlen-=ret;
			rlen+=ret;
		}
	}
	if (remlen==0)
	{
		return OSI_OK;
	}
	return OSI_ERROR;

#endif

#ifdef WINDOWS_OS
	return OSI_OK;
#endif
}


/*
 *	Function: atomwrite
 *	Description: atom write
 *	Input:
 *			fd:
 *			buf:
 *			len:
 *	return: OSI_OK, OSI_ERROR
 */
int atomwrite(int fd, void* buf, int len)
{

#ifdef LINUX_OS
	fd_set wset;
	struct timeval select_timeout;
	int remlen,wlen,ret,count=60;

	if (len<0)
	{
		return OSI_ERROR;
	}
	if (len==0)
	{
		return OSI_OK;
	}

	remlen=len;
	wlen=0;
	while (remlen>0 && count--)
	{
		FD_ZERO(&wset);
		FD_SET(fd,&wset);
		select_timeout.tv_sec=1;
		select_timeout.tv_usec=0;
		if (select(fd+1,NULL,&wset,NULL,&select_timeout)>0)
		{
			ret=write(fd,buf+wlen,remlen);
			if (ret==OSI_ERROR)
			{
				return OSI_ERROR;
			}
			remlen-=ret;
			wlen+=ret;
		}
	}

	if (remlen==0)
	{
		return OSI_OK;
	}

	return OSI_ERROR;
#endif

#ifdef WINDOWS_OS

	return OSI_OK;
#endif
}

int lower (int c)
{
	if (c >= 'A' && c<= 'Z')
		return c+'a'-'A';
	else
		return c;
}

int strcmplow(const char* s1,const char* s2)
{
	int i,j,c1,c2,cmp;

	cmp=0;
	i=0;
	j=0;
	while (s1[i] || s2[j])
	{
		c1=lower(s1[i]);
		c2=lower(s2[j]);
		if ((cmp=c1-c2)!=0)
		{
			return cmp;
		}
		if (s1[i])
			i++;
		if (s2[j])
			j++;
	}
	return cmp;
}

/* find the first match of cs in a n length string array */
int strmatch (int n,STRING strarray[], STRING cs)
{
	while (n--)
	{
		if (strcmplow(strarray[n],cs)==0)
		{
			return n;
		}
	}
	return OSI_ERROR;
}

void bezeros (int n,void * p)
{
	char * cp;

	cp=p;
	while (n--)
	{
		cp[n]='\0';
	}
}

void * zeros (int n)
{
	void * p;

	p=malloc(n);
	if (p==NULL)
	{
		return p;
	}
	bezeros(n,p);
	return p;
}

int strcmpn(const char* s1, const char* s2, int n)
{
	int i,c1,c2,cmp;

	cmp=0;
	i=0;
	while ((s1[i] || s2[i]) && n-->0)
	{
		c1=s1[i];
		c2=s2[i];
		if ((cmp=c1-c2)!=0)
		{
			return cmp;
		}
		i++;
	}
	return cmp;
}

unsigned long byte_sum(unsigned char* buf, int len)
{
	unsigned long sum;

	if (buf==NULL || len<0)
	{
		return OSI_ERROR;
	}

	sum=0;
	
	while (len--)
	{
		sum+=*buf++;
	}
	
	return sum;
}

int findchar(char c, char* str, int case_sensitive)
{
	int index=0;

	if (str==NULL)
	{
		return OSI_ERROR;
	}
	
	while (str[index])
	{
		if (case_sensitive==1)
		{
			if (c==str[index])
			{
				return index;
			}
		}
		else
		{
			if (lower(c)==lower(str[index]))
			{
				return index;
			}
		}
		index++;
	}

	return OSI_ERROR;
}

