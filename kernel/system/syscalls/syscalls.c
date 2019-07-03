#include "syscalls.h"

/*
    Func: initialize_system_calls
    Initialize the system calls used by the operating system
*/
void init_system_calls()
{
    register_system_call(SYSCALL_EXIT, (void*) sys_exit);
    register_system_call(SYSCALL_FORK, (void*) sys_fork);
    register_system_call(SYSCALL_READ, (void*) sys_read);
    register_system_call(SYSCALL_OPEN, (void*) sys_open);
    register_system_call(SYSCALL_CLOSE, (void*) sys_close);
    register_system_call(SYSCALL_CREAT, (void*) sys_creat);
    register_system_call(SYSCALL_EXECVE, (void*) sys_execve);
    register_system_call(SYSCALL_CHDIR, (void*) sys_chdir);
    register_system_call(SYSCALL_TIME, (void*) sys_time);
    register_system_call(SYSCALL_GETPID, (void*) sys_getpid);
    register_system_call(SYSCALL_GETUID, (void*) sys_getuid);
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
    kprintf("eax=0x%x\n", r.eax);
    kprintf("ebx=0x%x\n", r.ebx);

    if (r.eax <= MAX_SYSCALL_COUNT-1 && syscalls[r.eax]) {
        void* syscall = syscalls[r.eax];
        execute_syscall(syscall, r);
    }
}
