# Creates a myos.bin file that is runnable with QEMU, check kernel.c

AS = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

# Telling the makefile translator to find header-files from the project root
# --> Allows us to do #include "drivers/vga.h" instead of a long system file path
CFLAGS = -ffreestanding -O2 -Wall -Wextra -I. -std=gnu99

# All objects need to be translated before linking
OBJ = boot.o kernel/kernel.o drivers/vga.o cpu/byteIO.o

all: myos.bin

boot.o: boot.s
	$(AS) -f elf32 boot.s -o boot.o

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

myos.bin: $(OBJ) linker.ld
	$(LD) -T linker.ld -o myos.bin $(OBJ)

clean:
	rm -rf *.o kernel/*.o drivers/*.o cpu/*.o myos.bin