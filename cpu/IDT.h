#ifndef IDT_H
#define IDT_H

#include "kernel/types.h"
extern void idt_load(uint32_t idt_ptr_addr);
void init_idt();
void set_idt_gate(int n, uint32_t handler);

// Struture for the Interrupt Descriptor Table (IDT)
struct idt_entry_struct {
    uint16_t offset_low;    // Interrupt function's lowest 16 bits
    uint16_t selector;      // Code segment (0x08, defined in boot.s)
    uint8_t zero;           // Zero value, unused
    uint8_t type_attr;      // Type and attributes
    uint16_t offset_high;   // Interrupt function's highest 16 bits
} __attribute__((packed)); // Preventing C from adding additional bits
typedef struct idt_entry_struct idt_entry;

// Pointer to the IDT
struct idt_ptr_struct {
    uint16_t limit; // Size of the IDT
    uint32_t base; // The address of the first IDT-element in memory
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr;

#endif