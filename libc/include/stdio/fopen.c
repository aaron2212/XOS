#include "../_null.h"
#include "stdio.h"

// Attempt to open a file and return it
FILE* fopen(char* filename, const char* mode)
{
    return sys_open(filename, mode);
}