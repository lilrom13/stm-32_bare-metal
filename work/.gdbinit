target remote :2331
mon endian little
mon halt

# Define "split" and "ss" commands to switch to different user interface layouts
define split
  layout split
  layout asm
  layout regs
  focus cmd
end

define ss
  layout split
  layout src
  layout regs
  focus cmd
end

# Define "flash" command to reflash processor
define flash
  dont-repeat
  mon reset
  load
end
