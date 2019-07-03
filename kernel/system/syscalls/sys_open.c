#include "syscalls.h"
#include "../../../libc/include/stdio/stdio.h"

/*
    Func: open
    Open a file for reading

    Parameters:
        filename - the name of the file to open
        mode - the mode to open the file in
    
    Returns:
        A pointer to a structure representing information about the file
*/
FILE* sys_open(char* filename, const char* mode)
{
    // Prevent opening of files when no filename (an empty string) has been given
    if (strcmp(filename, "") == 0)
        return NULL;

    FILE *stream = vfs_open(filename, mode);

    /* 
     * Only set the file's attributes if a FILE was returned.
     * The file's position and flags were not set and so need to be set
     */
    if (stream != NULL)
    {
        // Check if the file was opened in a valid mode and set the FILE stream's mode property
        if (strcmp(mode, "r") == 0)
        { // Read
            stream->mode = MODE_READ;
            stream->position = 0;
        }
        else if (strcmp(mode, "rb") == 0)
        { // Read binary
            stream->mode = MODE_READ_BINARY;
            stream->position = 0;
        }
        else if (strcmp(mode, "r+") == 0)
        { // Read/write
            stream->mode = MODE_READ_WRITE;
            stream->position = 0;
        }
        else if (strcmp(mode, "rb+") == 0 || strcmp(mode, "r+b") == 0)
        { // Read/write binary
            stream->mode = MODE_READ_WRITE_BINARY;
            stream->position = 0;
        }
        else if (strcmp(mode, "w") == 0)
        { // Write
            stream->mode = MODE_WRITE;
            stream->position = 0;
            // TODO: truncate file
        }
        else if (strcmp(mode, "wb") == 0)
        { // Write binary
            stream->mode = MODE_WRITE_BINARY;
            stream->position = 0;
            // TODO: truncate file
        }
        else if (strcmp(mode, "w+") == 0)
        { // Read/write. Create if not exists
            stream->mode = MODE_READ_WRITE_CREATE;
            stream->position = 0;
            // TODO: truncate file
        }
        else if (strcmp(mode, "wb+") == 0 || strcmp(mode, "w+b") == 0)
        { // Read/write binary. Create if not exists
            stream->mode = MODE_READ_WRITE_CREATE_BINARY;
            stream->position = 0;
            // TODO: truncate file
        }
        else if (strcmp(mode, "a") == 0)
        { // Append
            stream->mode = MODE_APPEND;
            stream->position = stream->size;
        }
        else if (strcmp(mode, "a+") == 0)
        { // Read from beginning, write to end
            stream->mode = MODE_APPEND_WRITE;
            stream->position = 0;
        }
        else if (strcmp(mode, "ab+") == 0 || strcmp(mode, "a+b") == 0)
        { // Read from beginning, write to end, in binary
            stream->mode = MODE_APPEND_WRITE_BINARY;
            stream->position = 0;
        }
        else
        {
            set_error("Invalid file open mode"); // TODO: change to ferror()

            return NULL;
        }

        // Set the EOF indicator if the file's position indicator is equal to the size of the file
        if (stream->position == stream->size)
            stream->eof = EOF;
        else
            stream->eof = 0;

        // Return a FILE stream
        return stream;
    }
    else
    {
        // Need to set mode still
        if (strchr(mode, 'w') != NULL || strchr(mode, 'a') != NULL)
        {
            FILE *stream = vfs_create((char *)filename);

            // Set the file's open mode
            if (strcmp(mode, "w") == 0)
            {
                stream->mode = MODE_WRITE;
            }
            else if (strcmp(mode, "wb") == 0)
            {
                stream->mode = MODE_WRITE_BINARY;
            }
            else if (strcmp(mode, "w+") == 0)
            {
                stream->mode = MODE_READ_WRITE_CREATE;
            }
            else if (strcmp(mode, "wb+") || strcmp(mode, "w+b") == 0)
            {
                stream->mode = MODE_READ_WRITE_CREATE_BINARY;
            }
            else if (strcmp(mode, "a") == 0)
            {
                stream->mode = MODE_APPEND;
            }
            else if (strcmp(mode, "a+") == 0)
            {
                stream->mode = MODE_APPEND_WRITE;
            }
            else if (strcmp(mode, "ab+") == 0 || strcmp(mode, "a+b") == 0)
            {
                stream->mode = MODE_APPEND_WRITE_BINARY;
            }

            return stream;
        }
    }

    // File not found
    return NULL;
}