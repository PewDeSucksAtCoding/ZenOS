ENTRY(_start)

/* Gives an order to how the OS kernel should be placed into memory */

SECTIONS
{
    /* The OS is usually loaded to the memory adress of 0x100000 (1MB) */
    /* That's also where we are far enough away from the BIOS' data so that our OS doesn't get overwritten! */
    . = 1M;

    .text :
    {
        KEEP(*(.multiboot)) /* Multiboot has to be first so that QEMU finds it first! */
        *(.boot)    /* Next up is the assembly code */
        *(.text)    /* And last our c-code */
    }

    .data : { *(.data) } /* Space for assigned, global variables, like int x = 5; */
    .bss  : { *(.bss)  } /* Space for unassigned variables, like int y; They don't space in the myos.bin (check MakeFile), unlike the global variables */
}