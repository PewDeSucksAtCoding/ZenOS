#include "drivers/vga.h"

unsigned char lookUpTable[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '`', '\b', '\t', 'q',
    'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', '^', '\n', 'a', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'o', 'a', '<',
    'a', '*', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', ' ', ' ', ' ', ' ', ' ', ' '
};

// Upper case letters
unsigned char lookUpTableUpper[128] = {
    0, 27, '!', '"', '#', '$', '%', '&', '/', '(', ')', '=', '?', '`', '\b', '\t', 'Q',
    'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', '^', '\n', 'A', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'O', 'A', '>',
    'A', '*', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', ' ', ' ', ' ', ' ', ' ', ' '
};

// 0 = false, 1 = true
int shift_down = 0;
int caps_lock_on = 0;

uint8_t port_byte_in(uint16_t port) {
    uint8_t result;

    __asm__ volatile("in %%dx, %%al" : "=a" (result) : "d" (port));

    return result;
}

void KeybrMain() {
    // Getting the scancode from the port 0x60
    uint8_t scancode = port_byte_in(0x60);

    // Checking shift
    if (scancode == 0x2A) {
        shift_down = 1;
        return;
    } else if (scancode == 0xAA) {
        shift_down = 0;
        return;
    }

    // Not doing anything on key relases
    if (scancode & 0x80) {
        return;
    }
    
    // Main block for a "normal" keypress, so "a", "r", "7" etc.
    switch (scancode) {
        case 0x1C: // Enter
            Enter();
            break;

        case 0x0E: // Backspace
            Backspace();
            break;
        
        default: // All other keys
            if (shift_down || caps_lock_on) {
                char letter = lookUpTableUpper[scancode];
                PrintChar(letter);
            } else {
                char letter = lookUpTable[scancode];
                PrintChar(letter);
            }
            break;
    }
}