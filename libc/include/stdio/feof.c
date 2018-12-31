#include "stdio.h"

// Determines if the end of the file has been reached
int feof(FILE* stream)
{
    // A non-zero value indicates the end of a file
    if (stream->eof != 0) // == EOF?
        return EOF;
    
    // 0 indicates that it is not the end of the file
    return 0;
}