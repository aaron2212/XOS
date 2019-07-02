#include <stdbool.h>

#include "io.h"
#include "kbd.h"
#include "shell.h"
#include "../../libc/include/stdlib/stdlib.h"
#include "tty.h"
#include "vga.h"

char* vidmem = (char *) 0xB8000;

// Scan code set 1 for when shift is not pressed
static char scancode[59] = {
		0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
        '\b', 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd',
        'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
        'm', ',', '.', '/', 0, 0, 0, ' '
};

// Scan code set 1 for when shift is pressed
static char shift_scancode[59] = {
		0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
        0, 0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D',
        'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
        'M', '<', '>', '?', 0, 0, 0, ' '
};

void init_keyboard()
{
	// NOT WORKING: irq_install_handler(1, (void*) kbd_getchar);
}

/*
	Func: kbd_getchar
	Get the inputted value from the keyboard

	Returns:
		the character the was pressed on the keyboard
*/
char kbd_getchar()
{
	outb(0x60, 0); // clear output buffer (newline from grub menu selection)

	unsigned char c = 0;
	unsigned int i = 0;
	unsigned int pos = 0;
	unsigned int is_first_backspace = false;
	bool is_first_key_pressed = true;
	bool is_shift_pressed = false;

	i = y * VGA_WIDTH + x;

	for (;;) {
		if (inb(0x60) != c) {
			c = inb(0x60);

			if (c > 0) {
				key_code = c;

				// Set cursor color
				if (is_first_key_pressed) {
					vidmem[i+1] = make_color(COLOR_BLACK, COLOR_LIGHT_GRAY);
					is_first_key_pressed = false;
				}

				if (c >= 0x81 && c != KBD_LSHIFT_PRESSED && c != KBD_RSHIFT_PRESSED
							&& c != KBD_LSHIFT_RELEASED && c != KBD_RSHIFT_RELEASED
							&& c != KBD_BACKSPACE) {
					// key release, so do nothing
				} else {
					/*
					 * TODO: check for control key, caps lock and other keys
					 * 		 use arrow keys to move cursor
					*/

					// LShift or RShift pressed
					if (c == KBD_LSHIFT_PRESSED || c == KBD_RSHIFT_PRESSED) {
						is_shift_pressed = true;
					} else if (c == KBD_LSHIFT_RELEASED || c == KBD_RSHIFT_RELEASED) {
						// LShift or RShift released
						is_shift_pressed = false;
					} else if (x % 80 == 160)	{
						move_cursor(0, y+1);
					} else if (c == KBD_UP_ARROW) {
						// TODO: implement command history
						/*vidmem[i-1] = terminal_color;
						vidmem[i+3] = make_color(COLOR_BLACK, COLOR_LIGHT_GRAY);
						vidmem[i] = 'U';
						vidmem[i+1] = terminal_color;
						i += 2;
						move_cursor(x+2, y);*/
					} else if (c == KBD_DOWN_ARROW) {
						// TODO: implement command history
						/*vidmem[i-1] = terminal_color;
						vidmem[i+3] = make_color(COLOR_BLACK, COLOR_LIGHT_GRAY);
						vidmem[i] = 'D';
						vidmem[i+1] = terminal_color;
						i += 2;
						move_cursor(x+2, y);*/
					} else if (c == KBD_RIGHT_ARROW) {
						// TODO: move one character to the right
						/*vidmem[i-1] = terminal_color;
						vidmem[i+3] = make_color(COLOR_BLACK, COLOR_LIGHT_GRAY);
						vidmem[i] = 'R';
						vidmem[i+1] = terminal_color;
						i += 2;
						move_cursor(x+2, y);*/
					} else if (c == KBD_LEFT_ARROW) {
						// TODO: move one character to the left
						/*vidmem[i-1] = make_color(COLOR_WHITE, COLOR_LIGHT_GRAY);
						vidmem[i+1] = terminal_color;
						i -= 2;
						move_cursor(x-2, y);*/
					} else if (c == KBD_ENTER) {
						// enter pressed
						vidmem[i+1] = terminal_color;
						is_shift_pressed = false;
						kprintf("\n");
						return '\n';			// tell the shell we've pressed enter and process the input
					} else if (is_shift_pressed && c != KBD_BACKSPACE) {
						// shift pressed and not backspace
						terminal_putchar(shift_scancode[c]);
						pos++;

						return shift_scancode[c];
					} else if (!is_shift_pressed && c != KBD_BACKSPACE) {
						// shift not pressed and not backspace
						terminal_putchar(scancode[c]);
						pos++;

						return scancode[c];
					}

					if (c == KBD_BACKSPACE) {
						if (vidmem[i-4] != '$') {
							if (!is_first_backspace) {
								// printf("%d ", backspace_pressed);

								i = y * VGA_WIDTH + x;
								vidmem[i-2] = 0;
								x -= 2;
								index--; // remove character from array when pressing backspace
								vidmem[i+3] = make_color(COLOR_WHITE, COLOR_BLUE);
								vidmem[i+1] = make_color(COLOR_WHITE, COLOR_BLUE);
								vidmem[i-1] = make_color(COLOR_BLACK, COLOR_LIGHT_GRAY);
								i -= 2;
							} else {
								i = y * VGA_WIDTH + x;
								vidmem[i-2] = 0;
								x -= 2;
								index--; // remove character from array when pressing backspace
								vidmem[i+3] = make_color(COLOR_WHITE, COLOR_BLUE);
								vidmem[i+1] = make_color(COLOR_WHITE, COLOR_BLUE);
								vidmem[i-1] = make_color(COLOR_BLACK, COLOR_LIGHT_GRAY);
								i -= 2;
							}
						}

						return scancode[c];
					}
				}
			}
		}
	}
}

char* kbd_getstring()
{
	char c;
	char* s = "";
	int pos = 0;

	while ((c = kbd_getchar()) != '\n') {
		if (c == KBD_BACKSPACE) {
			kprintf("%d ", c);
		}

		s[pos++] = c;
	}

	s[pos] = '\0';

	return s;
}