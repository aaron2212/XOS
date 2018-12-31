#include "stdio.h"

// Return the current position of the file stream `stream`
long ftell(FILE* stream)
{
    return stream->position;
}