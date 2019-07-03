#ifndef _CONFIG_H
#define _CONFIG_H

// Host
#define MAX_HOSTNAME_LENGTH 255
#define MAX_USERNAME_LENGTH 255
#define MAX_PASSWORD_LENGTH 255
#define MAX_HOMEDIR_LENGTH 255
#define MAX_UID 255
#define MAX_USERS 16

// Time
#define ADD_YEARS 2000
#define UNIX_EPOCH 1970

#include "../../libc/include/stdint.h"
#include "../../libc/include/stdio/stdio.h"
#include "../fs/vfs.h"
#include "../../include/isr_irq/isrs.h"

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

#define SYSCALL_EXIT     0
#define SYSCALL_FORK     1
#define SYSCALL_READ     2
#define SYSCALL_OPEN     3
#define SYSCALL_CLOSE    4
#define SYSCALL_CREAT    5
#define SYSCALL_EXECVE   6
#define SYSCALL_CHDIR    7
#define SYSCALL_TIME     8
#define SYSCALL_GETPID   9
#define SYSCALL_GETUID   10

// sizeof(struct user) = 766
struct user {
    uint8_t uid;
    int8_t username[MAX_USERNAME_LENGTH];
    int8_t password[MAX_PASSWORD_LENGTH];
    int8_t home_dir[MAX_HOMEDIR_LENGTH];
};

enum cmos_ports {
    cmos_address = 0x70,
    cmos_data = 0x71
};

enum cmos_register_types {
    CMOS_TYPE_SECOND = 0x00,
    CMOS_TYPE_MINUTE = 0x02,
    CMOS_TYPE_HOUR = 0x04,
    CMOS_TYPE_DAY = 0x07,
    CMOS_TYPE_MONTH = 0x08,
    CMOS_TYPE_YEAR = 0x09
};

enum time_seconds {
    JANUARY_SECONDS = 2678400,
    FEBRUARY_SECONDS = 2419200,
    MARCH_SECONDS = 2678400,
    APRIL_SECONDS = 2592000,
    MAY_SECONDS = 2678400,
    JUNE_SECONDS = 2592000,
    JULY_SECONDS = 2678400,
    AUGUST_SECONDS = 2678400,
    SEPTEMBER_SECONDS = 2592000,
    OCTOBER_SECONDS = 2678400,
    NOVEMBER_SECONDS = 2592000,
    DECEMBER_SECONDS = 2678400,
    YEAR_SECONDS = 31536000,
    LEAP_YEAR_SECONDS = 31622400
};

char* hostname;
struct user users[MAX_USERS];
unsigned int total_user_count;
struct user current_user;

int timer_ticks;

void init_system();
void init_users();
char* get_hostname();
struct user get_current_user();
bool set_current_user(struct user current_user);

int get_system_time();
char read_rtc_register(uint16_t type);
int get_uptime();
void init_timer();
void handle_timer();

int bcd2bin(int bcd);

// User mode
void enter_usermode();

// System calls
extern void setup_syscall_params();

void init_system_calls();
void register_system_call(int num, void (*syscall)());
void handle_syscall(struct regs r);
void (*syscalls[MAX_SYSCALL_COUNT])();
extern void execute_syscall(void* syscall, struct regs r);

void sys_exit(int status_code);
void sys_fork();
void sys_read(void* buffer, size_t size, size_t count, FILE* stream);
void sys_open(char* filename, char* mode);

extern FILE* fopen(char* filename, const char* mode);
extern int fread(void* buffer, size_t size, size_t count, FILE* stream);
extern int fclose(FILE* stream);
extern void irq_install_handler(int irq, void (*handler));

#endif