#include "stdio.h"

int fgetc(FILE* stream)
{
    // Check if the file stream is valid
    if (stream == NULL)
        return EOF; // TODO: set error message
    
    // Check if the file is open for reading. Only allow reading from the file if it is open for reading
    bool is_file_open_for_reading =
              (stream->mode != MODE_READ || stream->mode != MODE_READ_BINARY
            || stream->mode != MODE_READ_WRITE || stream->mode != MODE_READ_WRITE_BINARY
            || stream->mode != MODE_READ_WRITE_CREATE || stream->mode != MODE_READ_WRITE_CREATE_BINARY);
    
    if (!is_file_open_for_reading)
        return EOF; // TODO: set error message

    if (!feof(stream)) {
        char c[1];
        
        vfs_read(stream, c, 1);

        // Check if we've read to the end of the file, and set the eof flag in necessary
        if (++stream->position == stream->size) {
            stream->eof = EOF;
        }

        // Return the read character
        return (int) c[0];
    }

    // Unable to read file contents, because the end of the file was reached
    return EOF;
}