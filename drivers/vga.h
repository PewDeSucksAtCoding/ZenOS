#ifndef VGA_H
#define VGA_H

#include "kernel/types.h"

void InitConsole();
void HandleCWD(int PrintOnly, char new_cwd[]);
void ConsolePrint(char *string, char bgr_color);
void PrintChar(char character);
void MoveCursor(uint8_t x, uint8_t y);
void Enter();
void Backspace();

#endif