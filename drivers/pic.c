#include "cpu/byteIO.h"
#include "pic.h"

void pic_remap() {
    // Configuring Master and Slave circuits in the PIC
    outb(0x20, 0x11);
    io_wait();
    outb(0xA0, 0x11);
    io_wait();

    // Creating new addresses for the circuits
    outb(0x21, 0x20);
    io_wait();
    outb(0xA1, 0x28);
    io_wait();

    // Connecting the circuits
    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();

    // Setting up a x86-mode for the circuits
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();
}