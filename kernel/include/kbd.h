#ifndef _KBD_H
#define _KBD_H

#include <stdbool.h>

#include "../../libc/include/stdint.h"

#define KBD_LSHIFT_PRESSED  0x2A
#define KBD_RSHIFT_PRESSED  0x36
#define KBD_LSHIFT_RELEASED 0xAA
#define KBD_RSHIFT_RELEASED 0xB6

#define KBD_ENTER 	  		0x1C
#define KBD_BACKSPACE 		0x0E

#define KBD_UP_ARROW 		0x48
#define KBD_DOWN_ARROW  	0x50
#define KBD_RIGHT_ARROW 	0x4D
#define KBD_LEFT_ARROW  	0x4B

struct regs {
	unsigned int gs, fs, es, ds;							// push segments last
	unsigned int edi, esi, ebp, ebx, edx, ecx, eax;					// pushed by 'pusha'
	unsigned int int_no, err_code;							// 'push byte #' and error code
	unsigned int eip, cs, eflags;							// pushed by CPU automatically
};

unsigned char key_code;

extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern unsigned int x;
extern unsigned int y;
extern unsigned int vga_width;
extern int terminal_color;
extern void terminal_write(char *str);
extern void terminal_putchar(char c);
extern void terminal_scroll();
extern int kprintf(const char *fmt, ...);
extern void move_cursor(uint32_t _x, uint32_t _y);

void init_keyboard();
char kbd_getchar();
char* kbd_getstring();

#endif
