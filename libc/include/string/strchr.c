#include "string.h"

// Returns a pointer to the first occurence of `ch` in `str`, or NULL if `ch` is not found in `str`
const char* strchr(const char* str, int ch)
{
    unsigned int i = 0;

    /*
     * Read `str` byte by byte and test if it is `ch`.
     * If it is found, return a pointer to the character
     */
    while (i != strlen(str)) {
        // If we found the character in the string, return its position (pointer to it)
        if (str[i] == (char) ch) {
            return (str+i);
        }

        i++;
    }

    // `ch` was not found in the string `str`
    return NULL;
}