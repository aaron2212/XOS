#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>
#include <stdbool.h>

#include "../stdint.h"
#include "../size_t.h"
#include "../../../kernel/fs/vfs.h"

#define EOF -1

// TODO: fclose() needs to kfree() the file

extern char* kbd_getstring();
extern char* strcpy(char* dst, const char* src);
extern const char* strchr(const char* str, int ch);
extern void* memset(void* dst, int value, size_t count);
extern void* kmalloc(size_t size);
extern FILE* vfs_open(const char* filename, const char* mode);

extern unsigned char key_code;

unsigned int putchar(char c);
unsigned int puts(char *str);
int getchar();
void gets(char *s);
char itoa(int value, char *str, int base);
int vprintf(const char* fmt, va_list arg);
int printf(const char *fmt, ...);

// Filesystem related functions
FILE* fopen(const char* filename, const char* mode);

#endif