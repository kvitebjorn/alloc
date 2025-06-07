bits 64
global memspace

%define HEAP_SIZE 0x40001000 ; 1 GB

section .data alloc noexec write progbits
  memspace:
    dq _memspace ; Pointer to the heap memory

section .heap alloc noexec write nobits
  _memspace:
    resb HEAP_SIZE ; Reserve 1 GB of space for the heap

section .note.GNU-stack noalloc
