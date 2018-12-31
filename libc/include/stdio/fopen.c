#include "../_null.h"
#include "stdio.h"

// Attempt to open a file and return it
FILE* fopen(const char* filename, const char* mode)
{
    FILE* stream = vfs_open(filename, mode);

    /* 
     * Only set the file's attributes if a FILE was returned.
     * The file's position and flags were not set and so need to be set
     */
    if (stream != NULL) {
        // Check if the file was opened in a valid mode and set the FILE stream's mode property
        if (strcmp(mode, "r") == 0) {                            // Read
            stream->mode = MODE_READ;
            stream->position = 0;
        } else if (strcmp(mode, "rb")) {                         // Read binary
            stream->mode = MODE_READ_BINARY;
            stream->position = 0;
        } else if (strcmp(mode, "r+")) {                         // Read/write
            stream->mode = MODE_READ_WRITE;
            stream->position = 0;
        } else if (strcmp(mode, "rb+") || strcmp(mode, "r+b")) { // Read/write binary
            stream->mode = MODE_READ_WRITE_BINARY;
            stream->position = 0;
        } else if (strcmp(mode, "w")) {                          // Write
            stream->mode = MODE_WRITE;
            stream->position = 0;
            // TODO: truncate file
        } else if (strcmp(mode, "wb")) {                         // Write binary
            stream->mode = MODE_WRITE_BINARY;
            stream->position = 0;
            // TODO: truncate file
        } else if (strcmp(mode, "w+")) {                         // Read/write. Create if not exists
            stream->mode = MODE_READ_WRITE_CREATE;
            stream->position = 0;
            // TODO: truncate file
        } else if (strcmp(mode, "wb+") || strcmp(mode, "w+b")) { // Read/write binary. Create if not exists
            stream->mode = MODE_READ_WRITE_CREATE_BINARY;
            stream->position = 0;
            // TODO: truncate file
        } else if (strcmp(mode, "a")) {                          // Append
            stream->mode = MODE_APPEND;
            stream->position = stream->size;
        } else if (strcmp(mode, "a+")) {                         // Read from beginning, write to end
            stream->mode = MODE_APPEND_WRITE;
            stream->position = 0;
        } else if (strcmp(mode, "ab+") || strcmp(mode, "a+b")) { // Read from beginning, write to end, in binary
            stream->mode = MODE_APPEND_WRITE_BINARY;
            stream->position = 0;
        }

        // Set the EOF indicator if the file's position indicator is equal to the size of the file
        if (stream->position == stream->size)
            stream->eof = EOF;
        else
            stream->eof = 0;
            
        // Return a FILE stream
        return stream;
    } else {
        if (strchr(mode, 'w') != NULL || strchr(mode, 'a') != NULL) {
            // TODO: create file if it does not exist
            //       vfs_create_file()
        }
    }

    // File not found
    return NULL;
}