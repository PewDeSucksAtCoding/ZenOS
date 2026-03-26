#include "vga.h"
#include "cpu/byteIO.h"
#include "kernel/string.h"
#include "kernel/shell.h"

char *VIDEO_MEMORY = (char*)0xB8000;
char TERMINAL_COLOR = 0x0F; // Plain white for the color (nothing will show in the console)
// x and y coordinates of the physical cursor
int x = 0;
int y = 0;

char cwd[64] = "%/kernel>"; // Current working directory (just for aesthetics)
char cmdBuffer[128];
int bufferIndex = 0;

void HandleCWD(int PrintOnly, char *new_cwd) {
    if (PrintOnly) {
        ConsolePrint(cwd, TERMINAL_COLOR, 0);
    } else {
        strcpy(cwd, new_cwd, 1);
    }
}

// Clearing the console from the 'crap' BIOS left behind
void InitConsole() {
    // Making sure we start from (0, 0) on clear
    x = 0;
    y = 0;
    int cursor_pos = (y * 80 + x) * 2;
    // The VGA-memory is a 80 row and 25 collum space and his means there is a total of 2000 characters (80x25) in this space
    // And since each character is 2 bytes long (character attribute + color attribute), there is a total of 2*2000B = 4kB of memory for VGA
    // Let's overwrite it all so we can start from scratch
    for (int i = 0; i < 2000; i++) {
        VIDEO_MEMORY[cursor_pos] = ' '; // Overwriting with a space
        VIDEO_MEMORY[cursor_pos + 1] = TERMINAL_COLOR;
        cursor_pos += 2;
    }

    // We still have to reset our cursor back to (0,0) so that ConsolePrint() doesn't crash
    x = 0;
    y = 0;
    ConsolePrint(cwd, TERMINAL_COLOR, 0);
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

void ConsolePrint(char *string, char bgr_color, int newline) {
    if (newline) {
        x = 0;
        y += 1;
    }

    int cursor_pos = (y * 80 + x) * 2;
    for (int i = 0; string[i] != '\0'; i++) {
        VIDEO_MEMORY[cursor_pos] = string[i];
        VIDEO_MEMORY[cursor_pos + 1] = bgr_color;
        cursor_pos += 2;
        x += 1;
        }

    // Logic for moving the cursor with printed text
    if (x >= 80) {
        x = 0;
        y += 1;
    }
    MoveCursor(x, y);
}

void PrintChar(char character) {
    int cursor_pos = (y * 80 + x) * 2;
    cmdBuffer[bufferIndex] = character;
    bufferIndex += 1;
    VIDEO_MEMORY[cursor_pos] = character;
    VIDEO_MEMORY[cursor_pos + 1] = TERMINAL_COLOR;
    x += 1;

    if (x >= 80) {
        x = 0;
        y += 1;
    }
    MoveCursor(x, y);
}

void Backspace() {
    int cursor_pos = (y * 80 + x) * 2;
    if (x <= strlen(cwd)) {
        return;
    }

    cmdBuffer[bufferIndex] = '\0';
    bufferIndex -= 1;
    VIDEO_MEMORY[cursor_pos - 1] = 0x0F;
    VIDEO_MEMORY[cursor_pos - 2] = ' ';

    if (x == 0) {
        x = 79;
        y -= 1;
    } else {
        x -= 1;
    }
    MoveCursor(x, y);
}

void Enter() {
    cmdBuffer[bufferIndex] = '\0';
    int return_num = parse_command(cmdBuffer);
    if (return_num == 1) {
        // Nothing to do if we clear the console. Check shell.c
    } else {
        x = 0;
        y += 1;
        MoveCursor(x, y);
        HandleCWD(1, cwd);
    }
    clearstring(cmdBuffer);
    bufferIndex = 0;
}