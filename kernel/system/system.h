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
    uint8_t name[MAX_USERNAME_LENGTH];
    uint8_t password[MAX_PASSWORD_LENGTH];
    uint8_t home_dir[MAX_HOMEDIR_LENGTH];
};

char* hostname;
struct user users[MAX_USERS];

void init_system();
void init_users();
char* get_hostname();

extern FILE* fopen(char* filename, const char* mode);
extern int fread(void* buffer, size_t size, size_t count, FILE* stream);
extern int fclose(FILE* stream);
extern void* kmalloc(size_t size);

#endif