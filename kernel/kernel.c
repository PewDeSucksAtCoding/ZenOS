/*
    Run the OS with
    "qemu-system-i386 -kernel myos.bin"
    on THIS DIRECTORY
*/
/*
    Alternatively run the OS with
    "qemu-system-i386 -kernel myos.bin -d int,cpu_reset -D qemu.log".
    It will create a qemu.log file containing useful information if the OS crashes or has other unexpected behaviour
*/

#include "cpu/byteIO.h"
#include "cpu/IDT.h"
#include "drivers/vga.h"
#include "drivers/pic.h"

int kernel_main() {
    // Running only once
    InitConsole();
    init_idt();
    pic_remap();

    asm volatile("sti");
    
    while (1); // NOTE: This is why it's extremely unlikely for us to run into the problem in boot.s on the .loop-section!
}