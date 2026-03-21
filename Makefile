AS = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

CFLAGS = -ffreestanding -O2 -Wall -Wextra -I. -std=gnu99

# All objects
OBJ = boot.o \
      cpu/IDT_asm.o \
      cpu/IDT.o \
      cpu/ISR_handler.o \
      cpu/ISR.o \
      kernel/kernel.o \
      drivers/vga.o \
      drivers/pic.o \
      cpu/byteIO.o \
      drivers/keybr_driver.o

all: myos.bin

%.o: %.s
	$(AS) -f elf32 $< -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

boot.o: boot.s
	$(AS) -f elf32 boot.s -o boot.o

myos.bin: $(OBJ) linker.ld
	$(LD) -T linker.ld -o myos.bin $(OBJ)

clean:
	rm -rf *.o kernel/*.o drivers/*.o cpu/*.o myos.bin