#include "alloc.h"

extern void *memspace;

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
  header *hdr = (header *)memspace;
  void *mem = memspace;

  if (!(hdr->w))
  {
    // This is the first allocation
    // Since we made our own heap in .bss/resb, it will be zero-initialized.
    mem = mkalloc(numWords, hdr);
    if (!mem)
    {
      return NULL;
    }

    return mem;
  }
  else
  {
    // TODO
    NULL;
  }

  return NULL;
}

int main(int argc, char *argv[])
{
  char *p;
  p = alloc(7);
  printf("%p\n", (void *)p);

  return 0;
}
