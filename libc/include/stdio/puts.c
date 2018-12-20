#include "stdio.h"

extern void terminal_write(char *str);

// Print string to screen, alias of terminal_write()

unsigned int puts(char *str)
{
    terminal_write(str);
    
    return 0;
}
