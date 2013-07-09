
ARMGNU ?= arm-elf

AOPS = --warn --fatal-warnings 
COPS = -std=c99 -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -fno-zero-initialized-in-bss -flto

OBJECTS = vectors.o pimon.o uart.o gpio.o timer.o string.o printf.o

all : pimon.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

%.o : %.c
	$(ARMGNU)-gcc $(COPS) -c $< -o $@

pimon.elf : memmap $(OBJECTS)
	$(ARMGNU)-gcc $(OBJECTS) -T memmap -o pimon.elf $(COPS) -lgcc
	$(ARMGNU)-objdump -D pimon.elf > pimon.list

pimon.bin : pimon.elf
	$(ARMGNU)-objcopy pimon.elf -O binary pimon.bin

pimon.hex : pimon.elf
	$(ARMGNU)-objcopy pimon.elf -O ihex pimon.hex

