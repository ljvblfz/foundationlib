# libdebug.a Makefile

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJS = $(patsubst %.c, %.o, $(SOURCES))

#CROSS_COMPILE= arm-wrs-linux-gnueabi-armv5tel-uclibc_small-
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
RANLIB = $(CROSS_COMPILE)ranlib

CFLAG	= 
DFLAG	= -Wall -O2
OTHERS	= -DLINUX_OS 
INCLUDES=
CFLAGS	=$(CFLAG) $(DFLAG) $(OTHERS) $(INCLUDES)

LIB = libdebug.a

all : $(LIB)

$(LIB):$(OBJS)
	$(AR) r $@ $(OBJS)
	$(RANLIB) $@
	cp $@ ../../lib
	cp debug.h ../../include/osi
	cp debug.h ../pthreadWrap
	cp debug.c ../pthreadWrap
	cp debug.h ../netApiWrap
	cp debug.c ../netApiWrap
	cp debug.h ../ioApiWrap
	cp debug.c ../ioApiWrap
	cp debug.h ../timeApiWrap
	cp debug.c ../timeApiWrap
	cp debug.h ../miscApiWrap
	cp debug.c ../miscApiWrap

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean :
	rm -f *.o *.bak $(LIB)

