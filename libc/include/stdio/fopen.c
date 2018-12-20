#include "../_null.h"
#include "stdio.h"
#include "../../../kernel/fs/vfs.h"

// Attempt to open a file and return it
FILE* fopen(const char* filename, const char* mode)
{
    FILE* fp = vfs_open(filename, mode);

    // Used for adding flags to the file's `flags` field
    unsigned int i = 0;

    /* 
     * Only set the file's attributes if a FILE was returned.
     * The file's position and flags were not set and so need to be set
     */
    if (fp != NULL) {
        // The file was opened in append mode, set its position to the end of the file
        if (strchr(mode, 'a') != NULL) {
            fp->position = fp->size;
            fp->flags[i++] = 'a';
        } else {
            fp->position = 0;
        }

        /* 
         * Set the file's flags (read, write, append)
         */
        if (strchr(mode, 'r') != NULL)
            fp->flags[i++] = 'r';
        
        if (strchr(mode, 'w') != NULL)
            fp->flags[i++] = 'w';
        
        fp->flags[i] = '\0';
    }

    return fp;
}