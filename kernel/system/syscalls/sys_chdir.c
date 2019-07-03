#include "syscalls.h"

/*
    Func: sys_chdir
    Change directory

    Parameters:
        dirname - the name of the directory to change to
    
    Returns:
        0 on success, -1 on failure
*/

int sys_chdir(char* dirname)
{
    return vfs_changedir(dirname);
}