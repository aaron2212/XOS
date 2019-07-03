#include "syscalls.h"

/*
    Func: sys_create
    Create a new, empty file
*/

void sys_creat(char* filename)
{
    vfs_create(filename);
}