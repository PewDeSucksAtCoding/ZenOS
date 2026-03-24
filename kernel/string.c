#include "string.h"

int strlen(char str[]) {
    int len = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        len++;
    }
    return len;
}

int strcmpr(char str1[], char str2[]) {
    if (strlen(str1) != strlen(str2)) {
        return 0;
    } else {
        // Now that we know str1 and str2 are the same length, let's use that common length
        int loop_until = strlen(str1);
        for (int i = 0; i < loop_until; i++) {
            if (str1[i] != str2[i]) {
                return 0;
            } else {
                continue;
            }
        }
    }
    return 1;
}

void clearstring(char str[]) {
    int str_len = strlen(str);
    for (int i = 0; i < str_len; i++) {
        str[i] = '\0';
    }
}

void strcpy(char *dest, char *src, int is_cwd) {
    int i = 0;
    for (; src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    if (is_cwd) {
        dest[i] = '>';
        dest[i+1] = '\0';
    } else {
        dest[i] = '\0';
    }
}

void strcat(char *dest, char*src) {
    int dest_len = strlen(dest);
    int i = 0;
    for (; src[i] != '\0'; i++) {
        dest[dest_len + i] = src[i];
    }
    dest[dest_len + i] = '\0';
}