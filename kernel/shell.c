#include "shell.h"
#include "string.h"
#include "drivers/vga.h"

char accepted_commands[2][6] = {"help", "clear"};

// Commands follow a format: {command} {target} {flags}, e.g. rm $/System/users/test_folder -f -q (though not yet possible)
struct Command {
    char *name; // Points to the beginning of the command buffer
    char *target; // Points to the target (or NULL if there's no target)
    char *flags[10]; // 10 pointers for the flags
    int flag_count;
};

void help_command() {
    ConsolePrint("ZenOS v. 1.0 --- AVAILABLE COMMANDS", 0x0F, 1);
    ConsolePrint("cd {dest} -> Changes the current working directory (cwd), though at the moment only aesthetically", 0x0F, 1);
    ConsolePrint("clear -> Clears the entire console", 0x0F, 1);
    ConsolePrint("If you're looking for more commands, please refer to a newer version of the operating system", 0x0F, 1);
}

void clear_command() {
    InitConsole();
}

void cd_command(char *new_cwd) {
    if (!new_cwd) {
        ConsolePrint("'cd' takes at least one argument: target", 0x0F, 1);
    } else {
        HandleCWD(0, new_cwd);
    }
}

int parse_command(char commandBuffer[]) {
    int buffer_len = strlen(commandBuffer);

    for (int i = 0; i < buffer_len; i++) {
        if (commandBuffer[i] == ' ') {
            commandBuffer[i] = '\0';
        }
    }

    struct Command current_command;

    char *ptr = commandBuffer;
    char *end = commandBuffer + buffer_len;
    current_command.name = 0;
    current_command.target = 0;
    current_command.flag_count = 0;
    int curr_token = 0;

    while (ptr < end) {
        if (curr_token >= 11) {
            break;
        } 

        if (*ptr != '\0') {
            if (curr_token == 0) current_command.name = ptr;
            else if (curr_token == 1) current_command.target = ptr;
            else {
                current_command.flags[current_command.flag_count] = ptr;
                current_command.flag_count++;
            }
            
            curr_token++;
            
            while (*ptr != '\0' && ptr < end) ptr++;
        } else {
            ptr++;
        }
    }
    if (current_command.name == 0) return 0;

    if (strcmpr(current_command.name, "help")) {
        help_command();
    } else if (strcmpr(current_command.name, "clear")) {
        clear_command();
        return 1;
    } else if (strcmpr(current_command.name, "cd")) {
        cd_command(current_command.target);
    } else {
        ConsolePrint("Command not regocnized. Use 'help' for more info", 0x0F, 1);
    }

    return 0;
}