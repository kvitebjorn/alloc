#include "alloc.h"

extern void *memspace;

header *findblock_(
    header *currHeader,
    uint32_t numWordsToAllocate,
    uint32_t currWordPos)
{
  // 1 for the header and 1 for at least one word alloc
  const uint8_t RESERVED = 2;
  if ((currWordPos + numWordsToAllocate) >
      (MAX_WORDS - RESERVED))
  {
    reterr(ERR_NO_MEM);
  }

  bool ok = false;
  if (!(currHeader->w))
  {
    // We are at the next available block and
    // we have already checked for enough space above
    ok = true;
  }
  if (!(currHeader->allocated) &&
      (currHeader->w >= numWordsToAllocate))
  {
    // We are at a block that was once allocated,
    // and since de-allocated, and the hole is big enough
    ok = true;
  }

  if (ok)
  {
    return currHeader;
  }

  // Otherwise we need to continue our search for a suitable block...
  header *nextHeader = (header *)((char *)currHeader + 4 + currHeader->w * 4);
  uint32_t nextWordPos = currWordPos + currHeader->w;
  return findblock_(nextHeader, numWordsToAllocate, nextWordPos);
}

void *mkalloc(uint32_t numWords, header *hdr)
{
  void *ret;
  uintptr_t bytesIn;
  uint32_t wordsIn;

  bytesIn = (uintptr_t)hdr - (uintptr_t)memspace;
  wordsIn = (bytesIn / 4) + 1;
  if (numWords > (MAX_WORDS - wordsIn))
  {
    reterr(ERR_NO_MEM);
  }

  hdr->w = numWords;
  hdr->allocated = true;
  ret = (void *)((char *)hdr + 4);

  return ret;
}

void *alloc(uint32_t numBytes)
{
  uint32_t numWords = (numBytes + 3) / 4; // Round up to nearest word
  void *mem = memspace;

  header *hdr = findblock(numWords);
  if (!hdr)
  {
    return NULL;
  }

  mem = mkalloc(numWords, hdr);
  if (!mem)
  {
    return NULL;
  }

  return mem;
}

void prettyprint_(header *hdr)
{
  header *currHeader = hdr;
  int32_t n = 1;
  const int32_t max_blocks = 1000000; // safety limit

  while (currHeader->w && n <= max_blocks)
  {
    printf("Alloc %d = %u %s words\n",
           n,
           currHeader->w,
           currHeader->allocated ? "allocated" : "free");

    currHeader = (header *)((char *)currHeader + sizeof(header) + currHeader->w * 4);
    n++;
  }
  if (n > max_blocks)
  {
    printf("...possible heap corruption or too many blocks\n");
  }
}

int main(int argc, char *argv[])
{
  printf("%p (base)\n", memspace);

  void *a = alloc(7);
  void *b = alloc(2000);
  void *c = alloc(1);

  printf("%p (alloc a)\n", a);
  printf("%p (alloc b)\n", b);
  printf("%p (alloc c)\n", c);

  prettyprint();

  return 0;
}
