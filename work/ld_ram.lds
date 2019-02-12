MEMORY {
  FLASH (rx) : ORIGIN = 0x0, LENGTH = 1M
  SRAM1 (!rx) : ORIGIN = 0x20000000, LENGTH = 96K
  SRAM2 (!rx) : ORIGIN = 0x10000000, LENGTH = 32K
}

SECTIONS {
  .text : {
    *(.text)
    _etext = . ;
  } > SRAM1

  .rodata : {
    _rodata = . ;
    *(.rodata)
    _erodata = . ;
  } > SRAM1

  .data : {
    _data = . ;
    *(.data)
    _edata = . ;
  } > SRAM1

  .bss : {
    _bss = . ;
    *(.bss) *(COMMON)
    _ebss = . ;
  } > SRAM1
}

ENTRY(_start)

_top_of_stack = ORIGIN(SRAM2) + LENGTH(SRAM2);
