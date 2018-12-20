#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    FILE* fp = fopen("/var/wwws", "r");

    if (fp == NULL) {
        printf("Failed to open!");
    } else {
        printf("Opened successfully!");
    }
}
