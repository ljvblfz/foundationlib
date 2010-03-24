#!/bin/bash
VINCLUDE = ../../include
VROOT = ../../source
VDEST = ../../lib
VPATH = $(VROOT)/foundationDbg $(VROOT)/foundationMsgQ $(VROOT)/foundationMutex $(VROOT)/foundationPthread $(VROOT)/foundationSem \
		$(VROOT)/foundationSocket $(VROOT)/foundationTask $(VROOT)/foundationTime
		

SOURCES := $(foreach dir,$(VPATH),$(wildcard $(dir)/*))
C_SRCS   = $(filter %.c,$(SOURCES))
CPP_SRCS = $(filter %.cpp,$(SOURCES))
SRCS = $(C_SRCS) $(CPP_SRCS)

C_OBJS   = $(C_SRCS:%.c=%.o)
CPP_OBJS = $(CPP_SRCS:%.cpp=%.o)
OBJS = $(C_OBJS) $(CPP_OBJS)
DEPS = $(OBJS:.o=.d)


PREFIX = arm-wrs-linux-gnueabi-armv5tel-uclibc_small-

CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AR 	= $(PREFIX)ar
RANLIB 	= $(PREFIX)ranlib

CFLAGS = -g -O2 -Wall -fsigned-char -Wno-unused -DDEV -D_REENTRANT -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE 
CFLAGS += -DLINUX_OS
CFLAGS += -I$(VINCLUDE)

#LDFLAGS += -lm -ljpeg -lttf

TARGET = foundationLib.a

all : $(TARGET)

%.d: %.c
	@set -e; rm -f $@; \
	 $(CC) -MM $(CFLAGS) $< > $@.$$$$; \
     sed 's,\($(notdir $*)\)\.o[ :]*,$*\.o $@ : ,g' < $@.$$$$ > $@; \
     rm -f $@.$$$$

%.d: %.cpp
	@set -e; rm -f $@; \
	 $(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
     sed 's,\($(notdir $*)\)\.o[ :]*,$*\.o $@ : ,g' < $@.$$$$ > $@; \
     rm -f $@.$$$$


$(TARGET): $(OBJS)
	$(AR) r $@ $(OBJS)
	$(RANLIB) $@
	cp $@ $(VDEST) 
	cp $(VINCLUDE)/*.h $(VDEST)
	-rm -f $(DEPS)
	rm -f $@

#  include $(DEPS)

.PHONY: all clean

all: $(TARGET)

clean:
	-rm -f $(TARGET) $(OBJS) $(DEPS)

