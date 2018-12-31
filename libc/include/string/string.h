#ifndef _STRING_H
#define _STRING_H

#include "../stdlib/stdlib.h"

size_t strlen(const char* str);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t count);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, int n);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, int n);
void* memcpy(void* dest, const void* src, size_t size);
void* memset(void* ptr, int value, size_t size);
int memcmp(const void* ptr, const void* ptr2, size_t num);
char* strrev(char* str);
char* strtok(char* str, char* delim);
int* create_delim_dict(char* delim);
const char* strchr(const char* str, int ch);

#endif
