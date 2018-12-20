#include "string.h"

char* strncpy(char* dest, const char* src, int n)
{
    // Crate a temporary string to add the characters from "src"
    char* s = kmalloc(n+1);

    // Copy the characters from "src" into the temporary string, the specified number of times
    memcpy(s, src, n);

    // Insert a null byte
    s[n+1] = '\0';

    // Copy over the temporary string into the destination
    strcpy(dest, s);

    kfree(s);

    return dest;
}
