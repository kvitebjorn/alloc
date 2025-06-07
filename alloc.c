#include "alloc.h"

extern heap *memspace;

int main(int argc, char *argv[])
{
  // test the heap
  int8 *p;
  p = memspace;
  *p = 'x';

  return 0;
}
