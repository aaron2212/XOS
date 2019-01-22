#ifndef _DEBUG_H
#define _DEBUG_H

extern char itoa(int value, char *str, int base);
extern size_t strlen(const char *str);
extern int kprintf(const char* fmt, ...);
extern unsigned int x;

void dump_memory(void* ptr, size_t size);

#endif
