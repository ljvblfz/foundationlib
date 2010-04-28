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

int getline(char** lineptr, int* n, FILE* fp);

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
int TCPLinkToServer(UINT32_T preAddr,UINT16_T port,int bufsize);
int findchar(char c, char* str, int case_sensitive);

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
