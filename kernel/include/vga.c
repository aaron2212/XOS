#include "vga.h"

unsigned int terminal_color = 0x07;
unsigned int cursor_x = 0;
unsigned int cursor_y = 0;

// Make terminal color from fg and bg colors

int make_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

// Set the color for the terminal

void terminal_set_color(enum vga_color fg, enum vga_color bg)
{
    terminal_color = make_color(fg, bg);
}

// Get the current color of the terminal

int terminal_get_color()
{
    return terminal_color;
}
