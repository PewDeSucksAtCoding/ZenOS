// Run the OS with "qemu-system-i386 -kernel myos.bin" on THIS DIRECTORY

#include "cpu/byteIO.h"
#include "drivers/vga.h"

char cwd[] = "/kernel"; // Current working directory (just for aesthetics)

int kernel_main() {
    InitConsole(); // Running only once
    MoveCursor(0, 0); // Setting the cursor to the left corner in the beginning
    ConsolePrint(cwd, 0x0F);

    while (1); // NOTE: This is why it's extremely unlikely for us to run into the problem
    // in boot.s on the .loop-section!
}