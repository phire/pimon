PREFIX := arm-elf
CC := $(PREFIX)-gcc
AS := $(PREFIX)-as
OBJCOPY := $(PREFIX)-objcopy

TARGET := pimon
DIRS := core lib modules

CFLAGS := -std=c99 -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -fno-zero-initialized-in-bss -Iinclude/

all: $(TARGET).bin

CFILES := $(shell find $(DIRS) -type f -name "*.c")
ASMFILES := $(shell find $(DIRS) -type f -name "*.s")
OBJFILES := $(patsubst %.s,%.o,$(ASMFILES)) $(patsubst %.c,%.o,$(CFILES)) 
DEPFILES := $(patsubst %.c,%.d,$(CFILES))

-include $(DEPFILES)

%.o: %.s Makefile
	@echo "   AS   $<"
	@$(AS) $< -o $@

%.o: %.c Makefile
	@echo "   CC   $<"
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(TARGET).elf: $(OBJFILES) rpi.lds
	@echo "   LD   $@"
	@$(CC) $(CFLAGS) $(OBJFILES) -T rpi.lds -o $@ -lgcc

$(TARGET).bin: $(TARGET).elf
	@echo "OBJCOPY $@"
	@$(OBJCOPY) $(TARGET).elf -O binary $@
	
clean:
	@echo Cleaning up
	@rm -f $(OBJFILES) $(DEPFILES) $(TARGET).elf $(TARGET).bin
