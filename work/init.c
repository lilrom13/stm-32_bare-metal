extern char _bss, _ebss;

void init_bss()
{
  for (char *dst = &_bss; dst < &_ebss; dst++)
    *dst = 0;
}
