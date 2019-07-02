#ifndef _TTY_H
#define _TTY_H

/*
    Constants: terminal screen constants
        VGA_WIDTH = 80 - the number of rows on the screen
        VGA_HEIGHT = 25 - the number of columns on the screen
*/

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#include "../../libc/include/stdint.h"

extern size_t strlen(const char *str);
extern void *memset(void *ptr, int value, size_t size);

void terminal_init();
void terminal_putchar(char c);
void terminal_write(char *str);
void terminal_scroll();
void move_cursor(unsigned int x, unsigned int y);
int get_cursor_x();
int get_cursor_y();
void move_cursor(uint32_t _x, uint32_t _y);

#endif
