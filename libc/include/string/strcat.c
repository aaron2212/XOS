#include "string.h"

extern int kprintf(const char* fmt, ...);

char *strcat(char *dst, const char *src)
{
	// Create a new string to store the concatenated strings
	int s_len = strlen(dst) + strlen(src) + 1;
	char s[s_len];
	memset(s, 0, sizeof(s));

	// Copy the first string into `s`
	strcpy(s, dst);

	// Copy over the rest of the string (pointed to by `src`) into `s`
	memcpy(s+strlen(s), src, strlen(src));

	// Insert a null byte at the end of the string
	s[strlen(s)] = '\0';

	// Copy the temporary string over into "dst"
	strcpy(dst, s);

	return dst;
}
