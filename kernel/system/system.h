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
    YEAR_SECONDS = /*31557600*/31536000,
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

extern FILE* fopen(char* filename, const char* mode);
extern int fread(void* buffer, size_t size, size_t count, FILE* stream);
extern int fclose(FILE* stream);
extern void irq_install_handler(int irq, void (*handler));

#endif