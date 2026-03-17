; Multiboot constant headers that tell QEMU this operating system is 32-bit and follows the multiboot standard
MBOOT_PAGE_ALIGN    equ 1 << 0
MBOOT_MEM_INFO      equ 1 << 1
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

; Here we write the headers above to the memory
section .multiboot
align 4
    ; dd (define double) means a 4-byte (32-bit) sized data and we are in a 32-bit system after all, aren't we?
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_CHECKSUM

; The code itself starts here
section .boot
bits 32
global _start
extern kernel_main

global outb

outb:
    mov edx, [esp + 4] ; Fetching the 2nd highest (port) argument from the stack
    mov al, [esp + 8] ; Fetching the 3rd highest (value) argument from the stack
    out dx, al ; Sending the value (al) to the port (dx)
    ret ; Returning back to the C-code

global inb

inb:
    mov edx, [esp + 4] ; Setting the registery dx's value to the argument given in C
    in al, dx ; Writing that value to the al registery where the C-code can see it as a return value
    ret

_start:
    cli
    ; C requires a stack for its local variables and functions
    ; Therefore we move to our allocated memory space to handle our C-code
    mov esp, stack_top
    call kernel_main

; If kernel_main() happened to come back...
.loop:
    hlt ; ... we put the processor to sleep ...
    jmp .loop ; ... and make sure we stay in this state forever to make the OS not crash!

; Here we declare the memoryarea. More specifically we allocate ~16kB of ram for the OS' stack
section .bss
align 16
stack_bottom:
    resb 16384
stack_top: