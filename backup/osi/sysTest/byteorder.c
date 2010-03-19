#include	"testHeader.h"

int testHostByteOrder(const char* recvBuf, char*sendBuf)
{
	union {
	  short  s;
      char   c[sizeof(short)];
    } un;

	un.s = 0x0102;
	if (sizeof(short) == 2) 
	{
		if (un.c[0] == 1 && un.c[1] == 2)
			printf("host byteorder is big-endian!\n");
		else if (un.c[0] == 2 && un.c[1] == 1)
			printf("host byteorder is little-endian!\n");
		else
			printf("host byteorder is unknown!\n");
	}
   	else
	{
		printf("sizeof(short) = %d\n", sizeof(short));
	}

	return 0;
}
