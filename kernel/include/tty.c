#include "tty.h"
#include "vga.h"
#include "vga.c"
#include "../../libc/include/stdio/stdio.h"

unsigned int x = 0;
unsigned int y = 0;

/*
	Func: terminal_init
	Initialize the terminal and clear the screen
*/
// Initialize the terminal and clear the screen
void terminal_init()
{
	terminal_color = make_color(COLOR_WHITE, COLOR_BLUE);

	char *vidmem = (char *)0xB8000;
	unsigned int i = 0;

	while (i < (80 * 25 * 2))
	{
		vidmem[i++] = ' ';
		vidmem[i++] = terminal_get_color();
	}

	move_cursor(0, 0);
}

/*
	Func: terminal_putchar
	Write a character to the screen
*/
void terminal_putchar(char c)
{
	char *vidmem = (char *)0xB8000;

	// '\n' sends cursor to beginning of next line
	if (c == '\n')
	{
		x = 0;
		y += 2;
	}
	else
	{
		unsigned int i = y * VGA_WIDTH + x;

		vidmem[i] = c;
		move_cursor(x + 1, y);
		i++;
		vidmem[i] = terminal_get_color();
		move_cursor(x + 1, y);
	}

	if (y > 48)
	{
		terminal_scroll();

		y -= 2;
	}
}

/*
	Func: terminal_write
	Write a string to the screen by repeatedly calling *terminal_putchar*
*/
void terminal_write(char *str)
{
	for (unsigned int i = 0; i < strlen(str); i++)
	{
		terminal_putchar(str[i]);
	}
}

/*
	Func: terminal_write
	Scroll the screen
*/
void terminal_scroll()
{
	char *vidmem = (char *)0xB8000;
	unsigned int i = 0;

	while (i < (80 * 25 * 2))
	{
		vidmem[i - 80 * 2] = vidmem[i];
		i++;
	}
}

/*
	Func: terminal_clear_screen
	Clear the entire screen
*/
void terminal_clear_screen()
{
	char *vidmem = (char *)0xB8000;

	unsigned int i = 0;

	while (i < (80 * 25 * 2))
	{
		vidmem[i++] = 0;
		vidmem[i++] = terminal_get_color();
	}

	move_cursor(0, 0);
}

/*
	Func: get_cursor_x
	Get the current X position of the cursor on the screen

	Returns:
		The current X position of the cursor on the screen as an integer
*/
int get_cursor_x()
{
	return x;
}

/*
	Func: get_cursor_y
	Get the current Y position of the cursor on the screen

	Returns:
		The current Y position of the cursor on the screen as an integer
*/
int get_cursor_y()
{
	return y;
}

/*
	Func: move_cursor
	Move the cursor to a specific (X, Y) coordinate on the screen

	Parameters:
		_x - the X cooridnate to move the cursor to
		_y - the Y coordinate to move the cursor to
*/
void move_cursor(uint32_t _x, uint32_t _y)
{
	x = _x;
	y = _y;
}
