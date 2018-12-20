#include "stdio.h"
#include "../size_t.h"

extern void* kmalloc(size_t size);
extern void* krealloc(void* ptr, size_t size);

// Continue to call getc() repeatedly until the user hits enter
void gets(char* s)
{
	char c;
	int pos = 0;

	// Continue to read characters into `s` as long as the user hasn't pressed the enter key
	while ((c = getchar()) != '\n') {
		// Backspace pressed, remove character from array
		if (c == 0x08) {
			pos--;
		} else {
			s[pos++] = c;
		}
	}

	// Insert a null byte
	s[pos] = '\0';
}
