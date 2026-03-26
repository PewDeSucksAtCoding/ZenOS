// This file contains logic for inserting IDT-entries to the table

#include "IDT.h"
extern void isr0();
extern void isr32();
extern void isr33();

idt_entry idt[256];
idt_ptr idt_reg;

void set_idt_gate(int n, uint32_t handler) {
    // Slicing the 32-bit address into 16-bit parts
    idt[n].offset_low = (uint16_t)(handler & 0xFFFF);
    idt[n].offset_high = (uint16_t)((handler >> 16) & 0xFFFF);

    // Setting the code segment and the zero byte
    idt[n].selector = 0x08;
    idt[n].zero = 0;

    // Setting the flags
    idt[n].type_attr = 0x8E;
}

void init_idt() {
    // Calculating the size and location
    idt_reg.limit = (uint16_t)(sizeof(idt_entry) * 256) - 1;
    idt_reg.base = (uint32_t)&idt;

    set_idt_gate(0, (uint32_t)isr0);
    set_idt_gate(32, (uint32_t)isr32);
    set_idt_gate(33, (uint32_t)isr33);

    // Loading the table for the CPU
    idt_load((uint32_t)&idt_reg);
}