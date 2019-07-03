#include "stdio.h"

// Attempt to read (`size` * `count`) bytes from the file stream `stream` into `buffer`
int fread(void* buffer, size_t size, size_t count, FILE* stream)
{
    return sys_read(buffer, size, count, stream);
}