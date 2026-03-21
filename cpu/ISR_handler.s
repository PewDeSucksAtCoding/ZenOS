[bits 32]
section .text

; Exporting the C-function that handles the logic itself
extern isr_handler

; Division by zero
global isr0
isr0:
    push byte 0
    push byte 0
    jmp isr_common

; Handling the timer
global isr32
isr32:
    push byte 0
    push byte 32
    jmp isr_common

global isr33
isr33:
    push byte 0
    push byte 33
    jmp isr_common

; All interrupts follow the same set of steps to make the interrupt work
; This is that common function for all the interrupts
isr_common:
    pusha ; Saving EAX, ECX, EDX, EBX, ESP, ESI, EDI

    push esp ; Pushing a pointer to the registeries C needs in isr_handler()
    call isr_handler

    pop eax
    popa
    add esp, 8 ; Clearing the two push bytes off from the stack
    iret ; Interrupt return