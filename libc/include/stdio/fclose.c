#include "stdio.h"

// Close the FILE stream and invalidate it
int fclose(FILE* stream)
{
    // Do not attempt to close a NULL stream (which may have already been closed)
    if (stream == NULL)
        return EOF;
    
    stream = NULL;
    // Remove the file stream `stream` from the list of file stream
    sys_close(stream);

    kfree(stream);
    

    return 0;
}