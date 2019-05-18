#ifndef _CONFIG_H
#define _CONFIG_H

#define MAX_HOSTNAME_LENGTH 255
#define MAX_USERNAME_LENGTH 255
#define MAX_PASSWORD_LENGTH 255
#define MAX_HOMEDIR_LENGTH 255
#define MAX_UID 255
#define MAX_USERS 16

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

char* hostname;
struct user users[MAX_USERS];
unsigned int total_user_count;
struct user current_user;

void init_system();
void init_users();
char* get_hostname();
struct user get_current_user();
bool set_current_user(struct user current_user);

extern FILE* fopen(char* filename, const char* mode);
extern int fread(void* buffer, size_t size, size_t count, FILE* stream);
extern int fclose(FILE* stream);

#endif