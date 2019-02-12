void init_bss()
{
  extern int _bss, _ebss;
  int *dst;

  for (dst = &_bss; dst < &_ebss; dst++)
    *dst = 0;
}
