#include "libtype.h"

/* MD5 context. */
typedef struct 
{
  UINT32_T          state[4];       /* state (ABCD) */
  UINT32_T          count[2];       /* number of bits, modulo 2^64 (lsb first) */
  UINT8_T           buffer[64];     /* input buffer */
}MD5_CTX;

void MD5Init(MD5_CTX* context);
void MD5Update(MD5_CTX* context, UINT8_T *input, UINT32_T inputLen);
void MD5Final( UINT8_T digest[], MD5_CTX *context);
static void MD5Transform(UINT32_T state[], UINT8_T block[]);
