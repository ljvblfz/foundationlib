/*
*	osi.h
*	OS Interface C head file.
*	
*	Foundation Services, Tool Module.
*	Based on SDK5.RC16, Linux 2.6.20, ARM Host CPU.
*	
*	MG3500 (MAXIM, mobilygen, Inc.) HD project
*   Copyright (c) Artificial Intelligence Infinity.
*
*	2009.07.30 Solution designed by shaobin, Programmed by shaobin.
*/

#include "datatype.h"

typedef void* mutex_t;


mutex_t mutex_create(void);
int mutex_init(mutex_t mutex, int opt);
int mutex_lock(mutex_t mutex);
int mutex_unlock(mutex_t mutex);

