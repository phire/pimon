
ARMGNU ?= arm-elf

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

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

pimon.o : pimon.c
	$(ARMGNU)-gcc $(COPS) -c pimon.c -o pimon.o

pimon.elf : memmap vectors.o pimon.o 
	$(ARMGNU)-ld vectors.o pimon.o -T memmap -o pimon.elf
	$(ARMGNU)-objdump -D pimon.elf > pimon.list

pimon.bin : pimon.elf
	$(ARMGNU)-objcopy pimon.elf -O binary pimon.bin

pimon.hex : pimon.elf
	$(ARMGNU)-objcopy pimon.elf -O ihex pimon.hex

