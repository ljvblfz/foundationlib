CC          := gcc
CXX         := gcc

DIST_INC        := -I/usr/include
NET_TOOLS_INC   := -I/usr/include/arpa/
LOCAL_INC       := -I./include
INC_PATH        := $(DIST_INC) $(NET_TOOLS_INC) $(LOCAL_INC)

DIST_LIB        := -L/usr/lib
LIB_PATH        := $(DIST_LIB)

LIB             := -lpthread
MODULE_DEFINES  :=-fno-strict-aliasing -DLINUX -DMG3500_UMA -DBSWRITERS_USE_SENDFILE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
CFLAGS	    := $(MODULE_DEFINES)  -Wall -O2 -o  

TARGET    := fd_test 
SRCC	    := $(wildcard *.c)
SRCC      += ./uuid/uuid.c
SRCC      += ./uuid/sysdep.c

COBJ	    := $(SRCC:.c=.o)
SRCXX	    := $(wildcard *.cpp)
CXXOBJ    := $(SRCXX:.cpp=.o)
OBJ	      := $(COBJ) $(CXXOBJ)

$(TARGET):$(OBJ)
	$(CXX) $(INC_PATH) $(LIB_PATH) $(CFLAGS) $@ $(OBJ) $(LIB) 

$(COBJ):%.o:%.c 
	$(CC) $(INC_PATH) $(CFLAGS) $@ -c $<

$(CXXOBJ):%.o:%.cpp 
	$(CXX) $(INC_PATH) $(CFLAGS) $@ -c $<

.PHONY:clean
clean:
	rm -f $(TARGET) *~ *.o
