#include "stdio.h"

// Reset the file stream's position indicator to the beginning of the file
void rewind(FILE* stream)
{
    stream->position = 0;

    // Set the EOF indicator if necessary
    if (stream->size == 0)
        stream->eof = EOF;
}