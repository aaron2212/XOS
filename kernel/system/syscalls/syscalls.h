#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#define MAX_SYSCALL_COUNT 11

/*
    Constants: system call numbers
        SYSCALL_EXIT     = 0
        SYSCALL_FORK     = 1
        SYSCALL_READ     = 2
        SYSCALL_OPEN     = 3
        SYSCALL_CLOSE    = 4
        SYSCALL_CREATE   = 5
        SYSCALL_EXECVE   = 6
        SYSCALL_CHDIR    = 7
        SYSCALL_TIME     = 8
        SYSCALL_GETPID   = 9
        SYSCALL_GETUID   = 10
 */

#define SYSCALL_EXIT 0
#define SYSCALL_FORK 1
#define SYSCALL_READ 2
#define SYSCALL_OPEN 3
#define SYSCALL_CLOSE 4
#define SYSCALL_CREAT 5
#define SYSCALL_EXECVE 6
#define SYSCALL_CHDIR 7
#define SYSCALL_TIME 8
#define SYSCALL_GETPID 9
#define SYSCALL_GETUID 10

#include "../../../libc/include/size_t.h"
#include "../../../include/isr_irq/isrs.h"
#include "../../fs/vfs.h"
#include "../system.h"

void sys_exit(int status_code);
void sys_fork();
int sys_read(void* buffer, size_t size, size_t count, FILE* stream);
FILE* sys_open(char* filename, const char* mode);
void sys_close(FILE* stream);
void sys_creat(char* filename);
void sys_execve(const char* filename, const char* argv[], const char* envp[]);
int sys_chdir(char* dirname);
int sys_time();
int sys_getpid();
int sys_getuid();

void init_system_calls();
void register_system_call(int num, void (*syscall)());
void handle_syscall(struct regs r);
void (*syscalls[MAX_SYSCALL_COUNT])();
extern void execute_syscall(void* syscall, struct regs r);

extern int kprintf(const char* format, ...);
extern int fgetc(FILE* stream3);

#endif