#ifndef _VGA_H
#define _VGA_H

/*
	enum: vga_color
	Colors used for 16 bit graphics (VGA text mode, 80x25)
		COLOR_BLACK = 0 - black
		COLOR_BLUE = 1 - blue
		COLOR_GREEN = 2 - green
		COLOR_CYAN = 3 - cyan
		COLOR_RED = 4 - red
		COLOR_MAGENTA = 5 - magenta
		COLOR_BROWN = 6 - brown
		COLOR_LIGHT_GRAY = 7 - light gray
		COLOR_DARK_GRAY = 8 - dark gray
		COLOR_LIGHT_BLUE = 9 - light blue
		COLOR_LIGHT_GREEN = 10 - light green
		COLOR_LIGHT_CYAN = 11 - light cyan
		COLOR_LIGHT_RED = 12 - light red
		COLOR_LIGHT_MAGENTA = 13 - light magenta
		COLOR_LIGHT_BROWN = 14 - light brown
		COLOR_WHITE = 15 - white
*/

// Colors for 16 bit graphics (80x25)
enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GRAY = 7,
	COLOR_DARK_GRAY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

int make_color(enum vga_color fg, enum vga_color bg);
void terminal_set_color(enum vga_color fg, enum vga_color bg);
int terminal_get_color();

#endif
