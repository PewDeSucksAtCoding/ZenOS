#include "kernel/types.h"
#include "drivers/vga.h"
#include "cpu/byteIO.h"
#include "drivers/keybr_driver.h"

// This struct is what assembly pushes to the stack
typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Registers from pusha
    uint32_t int_no, err_code; // Push bytes
    uint32_t eip, cs, eflags, useresp, ss; // Automatically added by the prossessor
} registers_t;

void isr_handler(registers_t *regs) {
    if (regs->int_no == 0) {
        ConsolePrint("Division by zero is not allowed!", 0x0C, 1);
        asm volatile("cli; hlt");
    }
    else if (regs->int_no >= 32) {
        if (regs->int_no >= 40) outb(0xA0, 0x20); // EOI for Slave

        outb(0x20, 0x20); // Handling EOI (End Of Interrupt) // EOI for Master

        if (regs->int_no == 33) {
            KeybrMain();

            inb(0x60); // Clearing the scancode off
        }
    }
    else if (regs->int_no < 32) {
        ConsolePrint("Generic Exception. System halted.", 0x0F, 1);
        asm volatile("cli; hlt");
    }
}