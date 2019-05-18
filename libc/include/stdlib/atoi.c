#include "../string/string.h"
#include "stdio.h"

int atoi(const char* str) {
    int n = 0;

    for (unsigned int i = 0; i < strlen(str); i++) {
        n = n * 10 + (str[i] - '0');
    }

    return n;
}