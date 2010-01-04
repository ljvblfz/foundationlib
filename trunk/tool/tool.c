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

#include "tool.h"

/*
 *	tools for test file
 */

/*
 *	Function: mg_getline
 *	Description: Get line function special for MG project.
 *					Unlike standard function "getline", this similar function
 *					will not allocate any buffer internally, this will be more
 *					easily to use and more safe for huge system distribution.
 *	Input:	
 *			n:	character string buffer size.
 *			fp: File pointer to text stream file.
 *	Output:
 *			lineptr: pointer to string line buffer
 *	Return:
 *			ERROR or count of read character.
 *	Note: the real character number that stored in lineptr always 1 less than
 *			n because of the delimiter character '\0'
 */
int mg_getline(STRING* lineptr, int* n, FILE* fp)
{
	int count;
	int c,linelen;
	long offset;
	char tempbuf[1024];
	STRING readstr;
	
	if (*lineptr==NULL)	/* Internal allocate memory */
	{
		count=0;
		while ((c=fgetc(fp))>0)
		{
			if (count < 1024)
			{
				tempbuf[count]=(char)c;
			}
			count++;

			if (c == '\n')
			{
				break;
			}
		}

		if (count==0)
		{
			return ERROR;
		}
		
		linelen=count;
		if ((readstr=malloc(linelen+1))==NULL)
		{
			printf("Memory limited!\n");
			return ERROR;
		}
		
		if (linelen > 1024)
		{
			offset=1024-linelen;
			fseek(fp,offset,SEEK_CUR);
			memcpy(readstr,tempbuf,1024);
			if (fread(readstr+1024, linelen-1024, 1, fp)!=1)
			{
				printf("Read data error, the storage media maybe damaged!\n");
				return ERROR;
			}
		}
		else
		{
			memcpy(readstr,tempbuf,linelen);
		}

		readstr[linelen]='\0';
		*n=linelen;
	}
	else	/* Memory has been allocated outside */
	{
		readstr=*lineptr;
		linelen=*n;
		count=0;
		while (--linelen && (c=fgetc(fp))>0 && readstr[count]=c)
		{
			count++;
			if (c == '\n')
			{
				break;
			}
		}
		readstr[count]='\0';

		if (count == 0)
		{
			return ERROR;
		}
	}
	
	return count;
}

/*
 *	Function: findstr
 *	Description: find a matched string from a original string.
 *	Input:
 *			original_str:	Original string.
 *			match_str:		Match string.
 *	Return:
 *			ERROR or index of matched string.
 */
int findstr(char* original_str, char* match_str)
{
	int index=0;

	if (original_str==NULL || match_str==NULL)
	{
		return ERROR;
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

	return ERROR;
}

int TCPLinkToServer(ULONG preAddr,UINT16 port,int bufsize)
{
	int s;
	struct	sockaddr_in	peeraddr_in;
	int optlen;

	/*
	*	Parameters check
	*/
	if (preAddr==0 || port==0)
	{
		return ERROR;
	}

	if ((s=socket(AF_INET,SOCK_STREAM,0))==ERROR)
	{
		printf("Create socket fail!\n");
		return ERROR;
	}

	if (bufsize>0)
	{
		if (setsockopt (s, SOL_SOCKET, SO_RCVBUF, (char *)&bufsize, sizeof (bufsize))==ERROR)
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
		return ERROR;
	}	
	peeraddr_in.sin_addr.s_addr = preAddr;/*(int) inet_addr (preAddr);*/
	peeraddr_in.sin_port        = htons (port);	

	if (connect (s, (struct sockaddr *)&peeraddr_in, sizeof(peeraddr_in)) == ERROR)
	{
		close(s);    
		return ERROR;    
	}

	return s;
}

int atomread(int fd,void* buf, int len)
{
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
				return ERROR;
			}
			remlen-=ret;
			rlen+=ret;
		}
	}
	if (remlen==0)
	{
		return OK;
	}
	return ERROR;
}


/*
 *	Function: atomwrite
 *	Description: atom write
 *	Input:
 *			fd:
 *			buf:
 *			len:
 *	return: OK, ERROR
 */
int atomwrite(int fd, void* buf, int len)
{
	fd_set wset;
	struct timeval select_timeout;
	int remlen,wlen,ret,count=60;

	if (len<0)
	{
		return ERROR;
	}
	if (len==0)
	{
		return OK;
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
			if (ret==ERROR)
			{
				return ERROR;
			}
			remlen-=ret;
			wlen+=ret;
		}
	}

	if (remlen==0)
	{
		return OK;
	}

	return ERROR;
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
	return ERROR;
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
		return ERROR;
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
		return ERROR;
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

	return ERROR;
}

