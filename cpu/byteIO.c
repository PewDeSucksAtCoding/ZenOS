#include "byteIO.h"

void io_wait() {
    // Sending trash values to the port 0x80 to give the PIC some time between many outb() calls
    outb(0x80, 0xAA);
}