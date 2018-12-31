#include "stdio.h"

// Seek to a specific position in the file
int fseek(FILE* stream, long int offset, int origin)
{
    // The position to read to in the file (controls reading past the end of the file)
    unsigned long int read_until;

    // Seek from the beginning of the file until `offset`, or to the end of the file
    if (origin == SEEK_SET) {
        // Ensure we do not read past the end of the file
        read_until = ((unsigned long int) offset > stream->size)
                ? stream->size : (unsigned long int) offset;

        stream->position = read_until;
    // Seek from the current position until `offset`, or to the end of the file
    } else if (origin == SEEK_CUR) {
        // Ensure we do not read past the end of the file
        read_until = ((unsigned long int) offset + stream->position > stream->size)
                ? stream->size : stream->position + offset;

        stream->position = read_until;
    // Seek backwards from the end of the file until `offset`, or to the beginning of the file
    } else if (origin == SEEK_END) {
        // Ensure we do not read past the beginning of the file
        read_until = ((int64_t) stream->size - offset < 0) ? 0 : stream->size - offset;

        stream->position = read_until;
    }

    return 0;
}