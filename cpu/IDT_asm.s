; Assembly logic for the IDT
[bits 32]
section .text
global idt_load

idt_load:
    mov eax, [esp + 4]
    lidt [eax] ; Loading the IDT-pointer to registery
    ret