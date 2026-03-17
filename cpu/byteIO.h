#ifndef BYTEIO_H
#define BYTEIO_H

#include "kernel/types.h"

extern void outb(uint16_t port, uint8_t data);
extern void inb(uint16_t port);
void io_wait();

#endif