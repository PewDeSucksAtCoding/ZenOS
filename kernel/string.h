// Recreations of some basic string functions

#ifndef STRING
#define STRING

int strlen(char string[]);
int strcmpr(char str1[], char str2[]);
void clearstring(char string[]);
void strcpy(char *dest, char *src, int is_cwd);
void strcat(char *dest, char*src);

#endif