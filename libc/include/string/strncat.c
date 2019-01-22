#include "string.h"

char* strncat(char* dest, const char* src, size_t count)
{
    // Create a new string to store the concatenated strings
	int s_len = strlen(dest) + count;
	char s[s_len];
	memset(s, 0, sizeof(s));

	// Copy the first string into `s`
	strcpy(s, dest);

	// Copy over the rest of the string (pointed to by `src`) into `s`
	memcpy(s+strlen(s), src, count);

	// Insert a null byte at the end of the string
	s[strlen(s)] = '\0';

	// Copy the temporary string over into "dst"
	strcpy(dest, s);
	
	return dest;
}