#ifndef _ERRORS_H
#define _ERRORS_H

#define ERROR_MESSAGE_LEN   128
#define MEMORY_ALLOC_FAILED "Failed to allocate memory"

#include "../../libc/include/size_t.h"

extern char *strcpy(char *dst, const char *src);
extern size_t strlen(const char *str);
extern int printf(const char *fmt, ...);
extern char* strcat(char* dst, const char* src);
extern char itoa(int value, char* str, int base);
extern void strrev(char* str);

const char *get_last_error(); // Return the error message
void set_error(const char* message, ...); // Set the error message

char error_message[ERROR_MESSAGE_LEN]; // The error message

#endif