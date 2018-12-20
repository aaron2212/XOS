#include "string.h"

int strncmp(const char* str1, const char* str2, int n)
{
    // Create two new strings with a length of `n` (the number of characters to compare)
    char* s1 = kcalloc(n, 1);
    char* s2 = kcalloc(n, 1);

    // Copy only the first `n` characters of each string passed into the function into the new strings
    strncpy(s1, str1, n);
    strncpy(s2, str2, n);

    // Compare the new strings of length `n`
    if (strcmp(s1, s2) == 0)
        return 0;
    else if (strcmp(s1, s2) == 1)
        return 1;
    else if (strcmp(s1, s2) == -1)
        return -1;

    kfree(s1);
    kfree(s2);

    return 0;
}
