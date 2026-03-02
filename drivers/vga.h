#ifndef VGA_H
#define VGA_H

#include "kernel/types.h"

void InitConsole();
void ConsolePrint(char *string, char bgr_color);
void MoveCursor(uint8_t x, uint8_t y);

#endif