#include "syscalls.h"

/*
    Func: sys_exit
    Exit the current process (not implemeted yet)

    Parameters:
        status_code - the status code to exit the process with
 */
void sys_exit(int error_code)
{
    kprintf("exit(0x%x): not implemented\n", error_code);
}