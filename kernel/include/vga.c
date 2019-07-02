#include "vga.h"

unsigned int terminal_color = 0x07;
unsigned int cursor_x = 0;
unsigned int cursor_y = 0;

/*
    Func: make_color
    Produce an integer for the attribute byte describing what color the text and screen must be

    Parameters:
        fg - the foreground (text) color
        bg - the background color

    Returns:
        The attribute byte containing describing the foreground and background colors 
*/
int make_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

/*
    Func: terminal_set_color
    Set the color for the terminal

    Parameters:
        fg - the foreground color for the terminal
        bg - the background color for the terminal
*/
void terminal_set_color(enum vga_color fg, enum vga_color bg)
{
    terminal_color = make_color(fg, bg);
}

/*
    Func: terminal_get_color
    Get the current color that is being used for the terminal

    Returns:
        An integer that is the attribute byte representing the terminal's foreground and background colors
*/
// Get the current color of the terminal
int terminal_get_color()
{
    return terminal_color;
}
