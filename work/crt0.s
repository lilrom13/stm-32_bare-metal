.syntax unified
.global _start
.thumb

_start:
  ldr sp, = _top_of_stack
  bl init_bss
  bl main
_exit:
  b .
