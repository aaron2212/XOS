#include "stdio.h"

// Unpacks the variable arguments and passes them to vprintf to be printed to the screen
int printf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    // Pass the arguments to vprintf to be unpacked and printed and then get the number
    // of characters written to the screen as returned by vprintf
    int num_chars_written = vprintf(fmt, args);

    va_end(args);

    return num_chars_written;
}