#include "stdio.h"

// Print character to screen, alias of terminal_putchar()

extern void terminal_putchar(char c);

unsigned int putchar(char c)
{
    terminal_putchar(c);
    
    return 0;
}
