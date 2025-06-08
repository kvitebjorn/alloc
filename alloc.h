#ifndef ALLOC_H
#define ALLOC_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

// GCC specific - might change if ever need to port
#define packed __attribute__((__packed__))
#define unused __attribute__((__unused__))

#define ERR_NO_MEM 1

#define MAX_WORDS (1024 * 1024 * 1024 / 4) - 1 // Need at least one word for a header

struct packed s_header
{
  uint32_t w : 30;
  bool allocated : 1;
  bool unused reserved : 1;
};
typedef struct packed s_header header;

// The do...while(false) trick lets us have {} for safety
// particularly on `if` statements that don't use {}!
#define reterr(x) \
  do              \
  {               \
    errno = (x);  \
    return NULL;  \
  } while (false)

#define findblock(x) \
  findblock_((header *)memspace, (x), 0);

#define prettyprint() \
  prettyprint_((header *)memspace);

void prettyprint_(header *);
header *findblock_(header *, uint32_t, uint32_t);
void *mkalloc(uint32_t, header *);
void *alloc(uint32_t);

#endif // ALLOC_H