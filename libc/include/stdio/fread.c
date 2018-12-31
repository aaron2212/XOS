#include "stdio.h"

// Attempt to read (`size` * `count`) bytes from the file stream `stream` into `buffer`
int fread(void* buffer, size_t size, size_t count, FILE* stream)
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