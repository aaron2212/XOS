#include "stdio.h"

extern char kbd_getchar();

// Get inputted character, alias of kbd_getchar()
int getchar()
{
	return kbd_getchar();
}