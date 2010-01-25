# libxxx.a Makefile

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJS = $(patsubst %.c, %.o, $(SOURCES))

#CROSS_COMPILE= arm-wrs-linux-gnueabi-armv5tel-uclibc_small-
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
RANLIB = $(CROSS_COMPILE)ranlib

CFLAG	=
DFLAG	=-D_REENTRANT -O2 -Wall -D_GNU_SOURCE
OTHERS	= -DLINUX_OS 
INCLUDES=
CFLAGS	=$(CFLAG) $(DFLAG) $(OTHERS) $(INCLUDES)

LIB = libpthreadWrap.a

all : $(LIB)

$(LIB):$(OBJS)
	$(AR) r $@ $(OBJS)
	$(RANLIB) $@
	cp pthreadWrap.h ../../include/osi
	cp $@ ../../lib


%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean :
	rm -f *.o *.bak $(LIB)

