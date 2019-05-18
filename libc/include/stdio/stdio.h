#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>
#include <stdbool.h>

#include "../../../kernel/fs/vfs.h"
#include "../_null.h"
#include "../size_t.h"
#include "../stdint.h"

#define EOF -1
#define BUFSIZ 1024
#define FILENAME_MAX 255
#define FOPEN_MAX 32768

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern char* kbd_getstring();
extern char* strcpy(char* dst, const char* src);
extern const char* strchr(const char* str, int ch);
extern void* memset(void* dst, int value, size_t count);
extern void* memcpy(void* dst, const void* src, size_t size);
extern void* kmalloc(size_t size);

extern unsigned char key_code;

enum file_open_modes {
    MODE_READ,
    MODE_READ_BINARY,
    MODE_READ_WRITE,
    MODE_READ_WRITE_BINARY,
    MODE_WRITE,
    MODE_WRITE_BINARY,
    MODE_READ_WRITE_CREATE,
    MODE_READ_WRITE_CREATE_BINARY,
    MODE_APPEND,
    MODE_APPEND_WRITE,
    MODE_APPEND_WRITE_BINARY
};

FILE* stdout;
FILE* stdin;
FILE* stderr;

unsigned int putchar(char c);
unsigned int puts(char* str);
int getchar();
void gets(char* s);
char itoa(int value, char* str, int base);
int vprintf(const char* fmt, va_list args);
int kprintf(const char* fmt, ...);

// Filesystem related functions
FILE* fopen(char* filename, const char* mode);
int fread(void* buffer, size_t size, size_t count, FILE* stream);
int fgetc(FILE* stream);
int feof(FILE* stream);
long ftell(FILE* stream);
int fseek(FILE* stream, long int offset, int origin);
void rewind(FILE* stream);
int fclose(FILE* stream);
char* fgets(char* str, int num, FILE* stream);

#endif