# libtimeApiWrap.a Makefile

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJS = $(patsubst %.c, %.o, $(SOURCES))

#CROSS_COMPILE= arm-wrs-linux-gnueabi-armv5tel-uclibc_small-
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
RANLIB = $(CROSS_COMPILE)ranlib

CFLAG	= 
DFLAG	=-Wall -O2 
OTHERS	=-DLINUX_OS 
INCLUDES=
CFLAGS	=$(CFLAG) $(DFLAG) $(OTHERS) $(INCLUDES)

LIB = libtimeApiWrap.a

all : $(LIB)

$(LIB):$(OBJS)
	$(AR) r $@ $(OBJS)
	$(RANLIB) $@
	cp timeApiWrap.h ../../include/osi
	cp $@ ../../lib

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean :
	rm -f *.o *.bak $(LIB)

