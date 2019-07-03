#include "system.h"

/*
    Func: initialize_system_calls
    Initialize the system calls used by the operating system
*/
void init_system_calls()
{
    register_system_call(SYSCALL_EXIT, sys_exit);
    register_system_call(SYSCALL_FORK, sys_fork);
    register_system_call(SYSCALL_READ, sys_read);
    register_system_call(SYSCALL_OPEN, sys_open);
}

/*
    Func: register_system_call
    Register a system call into the system call table

    Parameters:
        num - the index into the table where to register the system call
        syscall - a pointer to the system call function
*/
void register_system_call(int num, void (*syscall)())
{
    if (!syscalls[num]) {
        syscalls[num] = syscall;
    }
}

/*
    Func: handle_syscall
    Receives a system call number number and calls the function corresponding with that number

    Parameters:
        regs - a structure containing the register values passed to the functions
*/
void handle_syscall(struct regs r)
{
    kprintf("\n\nSystem call detected...:\n");

    if (r.eax <= MAX_SYSCALL_COUNT-1 && syscalls[r.eax]) {
        void* syscall = syscalls[r.eax];
        execute_syscall(syscall, r);
    }
}

/*
    Func: sys_exit
    Exit the current process (not implemeted yet)

    Parameters:
        status_code - the status code to exit the process with
 */
void sys_exit(int error_code)
{
    kprintf("exit(): not implemented: 0x%x\n", error_code);
}

/*
    Func: sys_fork
    Create a new process (not implemeted yet)
*/
void sys_fork()
{
    kprintf("fork(): not implemented\n");
}

/*
    Func: sys_read
    Read the contents of a file into a buffer (not implemeted yet)

    Paramaters:
        buffer - the buffer to read into
        size - the number of bytes to read
        count - the number of time to read *size* bytes
        stream - an open file stream to read from
*/
void sys_read(void* buffer, size_t size, size_t count, FILE* stream)
{
    kprintf("read(): not implemented\n");
}

/*
    Func: sys_open
    Open a file for reading (not implemeted yet)

    Parameters:
        filename - the name of the file to open
        mode - the mode to open the file in
    
    Returns:
        A pointer to a structure representing information about the file
*/
void sys_open(char* filename, char* mode)
{
    kprintf("open(%s, %s): not implemented\n", filename, mode);
}