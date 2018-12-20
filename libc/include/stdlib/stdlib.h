#ifndef _STDLIB_H
#define _STDLIB_H

#define NULL 0

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define RAND_MAX 0x7FFF

#define BLOCK_SIZE 4096

typedef unsigned int size_t;

extern void* memset(void* ptr, int value, size_t size);

char itoa(int value, char *str, int base);
int abs(int value);
void* kmalloc(size_t size);
void kfree(void *ptr);
void* krealloc(void *ptr, size_t size);
void* kcalloc(size_t num, size_t size);

#endif
