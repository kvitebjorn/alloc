#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#define packed __attribute__((__packed__))
#define unused __attribute__((__unused__))

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;
typedef __uint128_t int128;
typedef void heap;
typedef int32 word;

struct packed s_header
{
  word w : 30;
  bool allocated : 1;
  bool unused reserved : 1;
};
typedef struct packed s_header header;

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $16 (int128)
#define $c (char *)
#define $i (int)
#define $v (void *)
#define $h (header *)

int main(int, char **);
