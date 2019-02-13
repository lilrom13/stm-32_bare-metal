PREFIX=arm-none-eabi-
CC=$(PREFIX)gcc
AS=$(PREFIX)as
LD=$(PREFIX)ld
GDB=$(PREFIX)gdb
ASFLAGS=-g -mcpu=cortex-m4
CFLAGS=-Wall -Werror -g -O1 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS=-nostdlib -g -T ld_ram.lds
EXEC=firmware
OBJ= init.o firmware.o crt0.o led.o clocks/clocks.o

CFILES= $(wildcard *.c)
DEPS = $(patsubst %.c,%.d,$(CFILES))

all: $(EXEC)

firmware: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: connect

connect:
	JLinkGDBServer -device STM32L475VG -endian little -if SWD -speed auto -ir -LocalhostOnly

.PHONY: debug

debug: $(EXEC)
	$(GDB) $^

.PHONY: clean

clean:
	rm -f $(EXEC) *.o

-include $(DEPS)
