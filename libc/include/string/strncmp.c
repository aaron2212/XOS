#include "string.h"

int strncmp(const char* str1, const char* str2, int n) {
    // Create two new strings with a length of `n` (the number of characters to compare)
    char* s1 = kcalloc(n, 1);
    char* s2 = kcalloc(n, 1);

    // Copy only the first `n` characters of each string passed into the function into the new strings
    strncpy(s1, str1, n);
    strncpy(s2, str2, n);
    strcat(s1, "\0");
    strcat(s2, "\0");

    // Compare the new strings of length `n`
    unsigned int return_value = strcmp(s1, s2);

    kfree(s1);
    kfree(s2);

    return return_value;
}
