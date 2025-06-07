#ifndef ALLOC_H
#define ALLOC_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#define packed __attribute__((__packed__))
#define unused __attribute__((__unused__))

#define MAX_WORDS (1024 * 1024 * 1024 / 4) - 1 // Need at least one word for a header
#define ERR_NO_MEM 1

struct packed s_header
{
  uint32_t w : 30;
  bool allocated : 1;
  bool unused reserved : 1;
};
typedef struct packed s_header header;

#define reterr(x) \
  errno = (x);    \
  return NULL;

void *mkalloc(uint32_t, header *);
void *alloc(uint32_t);

#endif // ALLOC_H