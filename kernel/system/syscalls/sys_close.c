#include "syscalls.h"

/*
    Func: sys_close
    Close an open file stream

    Parameters:
        stream - the open file stream
*/

void sys_close(FILE* stream)
{
    vfs_close(stream);
}