#include "syscalls.h"
#include "../../../libc/include/_null.h"

/*
    Func: sys_read
    Read the contents of a file into a buffer (not implemeted yet)

    Paramaters:
        buffer - the buffer to read into
        size - the number of bytes to read
        count - the number of time to read *size* bytes
        stream - an open file stream to read from
*/
int sys_read(void* buffer, size_t size, size_t count, FILE* stream)
{
    // Check if the file stream is valid
    if (stream != NULL) {
        unsigned int i = 0; // The index into the buffer
        unsigned int num_bytes_to_read = 0; // The number of bytes to read from the file
        unsigned int num_bytes_read = 0; // The number of bytes read from the file
        int c = 0; // The character read from the file

        if ((int64_t) (stream->size - stream->position) - (int64_t) (size*count) >= 0) // size=87; pos=10; len=1 // size=87; pos=7; len=100
            num_bytes_to_read = (size*count);
        else
            num_bytes_to_read = (stream->size - stream->position);

        char* ptr = buffer;

        // Continue to read into the buffer as long as the end of the file has not been reached
        while (i <= num_bytes_to_read) {
            c = fgetc(stream);

            ptr[i++] = (char) c;
            
            num_bytes_read++;
        }

        // Add the null byte to the end of the string and reassign the void* buffer
        // with the contents of the char* ptr buffer
        buffer = ptr;

        return num_bytes_read;
    }

    // Failed to read from the file stream because it wasn't valid (NULL)
    return -1;
}