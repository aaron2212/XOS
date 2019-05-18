#include "string.h"

int strcmp(const char* str1, const char* str2) {
    unsigned int i = 0;

    if (*str1 < *str2) {
        return -1;
    } else if (*str1 > *str2) {
        return 1;
    }

    // Loop over the strings char by char and test them (don't read the null bytes)
    while (*str1 == *str2 && *str1 != '\0' && *str2 != '\0') {
        if (*(str1 + 1) < *(str2 + 1)) {
            return -1;
        } else if (*(str1 + 1) > *(str2 + 1)) {
            return 1;
        }

        i++;

        // If we are at the end of the string and all the characters match, return 0
        if (i == strlen(str1) - 1 && i == strlen(str2) - 1) {
            return 0;
        }

        // Advance the pointers
        str1++, str2++;
    }

    // The first string/character is alphabetically less than the second string/character
    if (*str1 < *str2) {
        return -1;
    }
    // The first string/character is alphabetically greater than the second string/character
    else if (*str1 > *str2) {
        return 1;
    }
    // The first string/character is alphabetically equal to the second string/character
    else if (*str1 == *str2) {
        return 0;
    }

    return 0;
}
