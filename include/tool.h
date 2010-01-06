/*
 *	tool.c
 *	tool C head file.
 *	This code file include some frequently used elemental tool functions
 *	in this project.
 *	
 *	Foundation Services, Tool Module.
 *	Based on SDK5.RC16, Linux 2.6.20, ARM Host CPU.
 *	
 *	MG3500 (MAXIM, mobilygen, Inc.) HD project
 *  Copyright (c) Artificial Intelligence Infinity.
 *
 *	2009.07.24 Solution designed by shaobin, Programmed by shaobin.
 */

#include "libtype.h"

/*
*	Function: getline_all
*	Description: 
*	getline()  reads an entire line includes delimiters from stream, storing 
*	the address of the buffer containing the text into *lineptr.  The  buffer  
*	is  null-terminated and includes the newline character includes delimiters.

*  If  *lineptr is NULL, then getline() will allocate a buffer for storing
*  the line, which should be freed by the  user  program.   Alternatively,
*  before  calling  getline(),  *lineptr  can  contain a pointer to a mal-
*  loc()-allocated buffer *n bytes in size. If the  buffer  is  not  large
*  enough  to hold the line, getline() resizes it with realloc(), updating
*  *lineptr and *n as necessary. In either case,  on  a  successful  call,
*  *lineptr and *n will be updated to reflect the buffer address and allo-
*  cated size respectively.
*	
*  Input:	
*			n:	character string buffer size.
*			fp: File pointer to text stream file.
*	Output:
*			lineptr: pointer to string line buffer
*	Return:
*			ERROR or count of read character.
*	Note: the real character number that stored in lineptr always 1 less than
*			n because of the delimiter character '\0'
*/
int getline_all(STRING* lineptr, int* n, FILE* fp);

/*
*	Function: findstr
*	Description: find a matched string from a original string.
*	Input:
*			original_str:	Original string.
*			match_str:		Match string.
*	Return:
*			ERROR or index of matched string.
*/
int findstr(char* original_str, char* match_str);

/*
*	Atom write, wait time: 1 minutes
*/
int atomwrite(int fd, void* buf, int len);

int strcmpn(const char* s1, const char* s2, int n);

void * zeros (int n);

void bezeros (int n,void * p);

int strmatch (int n,STRING strarray[], STRING cs);

int strcmplow(const char* s1,const char* s2);

int lower (int c);
int atomread(int fd,void* buf, int len);
unsigned long byte_sum(unsigned char* buf, int len);
int TCPLinkToServer(ULONG preAddr,UINT16 port,int bufsize);
int findchar(char c, char* str, int case_sensitive);

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))