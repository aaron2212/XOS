#ifndef _KERNEL_H
#define _KERNEL_H

#include <stdbool.h>

#include "../../libc/include/size_t.h"

#define HEAP_SIZE_ALLOC_PERCENT 0.05 // The amount of memory to set aside for the heap (5%)

extern void init_gdt();
extern void init_idt();
extern void init_isrs();
extern void init_irqs();
extern void init_timer();
extern int  init_fs();
extern const char *get_last_error();

void timer_wait(unsigned int ticks);
void init_keyboard();
int printf(const char *format, ...);
char *strcpy(char *dest, const char *src);
void gets(char *s);

extern int end;
unsigned int available_memory; // Amount of available memory in KB
unsigned int heap_size; // Heap size in KB

// The start and end of the rootfs filesystem loaded after the heap
unsigned char* rootfs_start;
unsigned char* rootfs_end;

unsigned int kernel_end = (unsigned int) &end + 10;

bool is_fs_loaded;

#endif