#include "shell.h"
#include "string.h"
#include "drivers/vga.h"

char accepted_commands[2][6] = {"help", "clear"};

void help_command() {
    ConsolePrint("ZenOS v. 1.0 --- AVAILABLE COMMANDS", 0x0F, 1);
    ConsolePrint("clear -> Clears the entire console", 0x0F, 1);
    ConsolePrint("If you're looking for more commands, please refer to a newer version of the operating system", 0x0F, 1);
}

void clear_command() {
    InitConsole();
}

int parse_command(char commandBuffer[]) {
    if (strlen(commandBuffer) <= 0) {
        return 0;
    } else if (strcmpr(commandBuffer, "help")) {
        help_command();
        return 0;
    } else if (strcmpr(commandBuffer, "clear")) {
        clear_command();
        return 1; // Special return for Enter in vga.c
    } else {
        ConsolePrint("Command not recognized. Please use 'help' for more info", 0x0F, 1);
        return 0;
    }
}