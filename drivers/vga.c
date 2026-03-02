#include "vga.h"
#include "cpu/byteIO.h"

char *VIDEO_MEMORY = (char*)0xB8000;
char TERMINAL_COLOR = 0x0F; // Plain white for the color (nothing will show in the console)
int cursor_pos = 0; // Latest index in which the VGA-memory was written to

// Clearing the console from the 'crap' BIOS left behind
void InitConsole() {
    // The VGA-memory is a 80 row and 25 collum space and his means there is a total of 2000 characters (80x25) in this space
    // And since each character is 2 bytes long (character attribute + color attribute), there is a total of 2*2000B = 4kB of memory for VGA
    // Let's overwrite it all so we can start from scratch
    for (int i = 0; i < 2000; i++) {
        VIDEO_MEMORY[cursor_pos] = ' '; // Overwriting with a space
        VIDEO_MEMORY[cursor_pos + 1] = TERMINAL_COLOR;
        cursor_pos += 2;
    }

    // We still have to reset our cursor back to 0 so that ConsolePrint() doesn't crash
    cursor_pos = 0;
}

void MoveCursor(uint8_t x, uint8_t y) {
    // Calculating the cursor's position in the VGA-matrix --> (0-1999)
    uint16_t pos = y * 80 + x;

    // Telling the VGA graphics card (on port 0x3D4) that we want to change the registery's 0x0F 8 lowest bits
    outb(0x3D4, 0x0F);
    // Sending the rest to the port 0x3D5
    outb(0x3D5, (uint8_t) (pos & 0xFF));

    // Telling the VGA graphics card that we want to choose the 8 uppermost bits (from registry 0x0E)
    outb(0x3D4, 0x0E);
    // Moving the top part to down (>> 8) and sending it to the port 0x3D5
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void ConsolePrint(char *string, char bgr_color) {
    for (int i = 0; string[i] != '\0'; i++) {
        VIDEO_MEMORY[cursor_pos] = string[i];
        VIDEO_MEMORY[cursor_pos + 1] = bgr_color;
        cursor_pos += 2;
    }

    // Logic for moving the cursor with printed text

    int physical_cursor_pos = cursor_pos / 2; // Because the VGA-memory has a character and color attribute, we need to devide that cursor position by two to get the actual cursor position
    int y = physical_cursor_pos / 80; // the y-coordinate will be the rounded down, so that if the physical cursor position is e.g. 79, we stay in the first line (79/80≈0)
    if (y > 0) { // if the y-coordinate is bigger than 0 (aka in another line), we need to seperately calculate the x-coordinate 
        int x = physical_cursor_pos % 80; // The x-coordinate will be whatever is left when the physical cursor position is divided by 80 (e.g. 165 % 80 = 5)
        MoveCursor(x, y);
    } else {
        MoveCursor(physical_cursor_pos, 0); // If y = 0, we can just use the physical cursor position for the x-coordinate
    }
    // --- EXAMPLE CALCULATION WITH THE LOGIC ABOVE ---

    // 1. cursor_pos = 256 (it has been moved there by this function)
    // 2. Calculating the physical cursor position (because of the character + color attributes): physical_cursor_pos = 256/2 = 128
    // 3. Calculating the y-coordinate: y = 128 / 80 = 1 (when rounded down)
    // 4. Calculating the x-coordinate: x = 128 % 80 = 48
    // 5. Finally utting the physical cursor to its right place: (x, y) --> (1, 48) in the VGA-matrix
}