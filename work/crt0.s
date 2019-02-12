.syntax unified
.global _start
.thumb

_start:
  ldr sp, = _top_of_stack
  bl init_bss
  bl main
  b _exit
_exit:
  b .
