#include "alloc.h"

extern void *memspace;

header *findblock_(
    header *currHeader,
    uint32_t numWordsToAllocate,
    uint32_t currWordPos)
{
  // 1 for the header and 1 for at least one word alloc
  const uint8_t RESERVED = 2;
  while (1)
  {
    if ((currWordPos + numWordsToAllocate) > (MAX_WORDS - RESERVED))
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
    if (!(currHeader->allocated) && (currHeader->w >= numWordsToAllocate))
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
    currHeader = (header *)((char *)currHeader + 4 + currHeader->w * 4);
    currWordPos += currHeader->w;
  }
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
  const int32_t MAX_BLOCKS = 1000000; // safety limit

  while (currHeader->w && n <= MAX_BLOCKS)
  {
    printf("Alloc %d = %u %s words\n",
           n,
           currHeader->w,
           currHeader->allocated ? "allocated" : "free");

    currHeader = (header *)((char *)currHeader + sizeof(header) + currHeader->w * 4);
    n++;
  }
  if (n > MAX_BLOCKS)
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

  // Should be +4 from base
  // This +4 is the header for `a`
  printf("%p (alloc a)\n", a);

  // +12 from `a`
  // 8 bytes (2 words) for `a`'s allocation
  //    7 bytes got rounded up to 8 bytes (nearest word)
  // 4 bytes for `b`'s header
  printf("%p (alloc b)\n", b);

  // +2004 from `b`
  // 2000 bytes (500 words) for `b`'s allocation
  // 4 bytes for `c`'s header
  printf("%p (alloc c)\n", c);

  // One day I'll make proper unit tests :)
  prettyprint();

  return 0;
}
