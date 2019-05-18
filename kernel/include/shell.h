#ifndef _SHELL_H
#define _SHELL_H

#define BUF_SIZE 1024
#define TOK_BUF_SIZE 64
#define TOK_DELIM " \t\r\n\a\""

#include "../../libc/include/stdio/stdio.h"

extern int strcmp(const char* str1, const char* str2);
extern char* strtok(char* str, const char* delim);
extern void kfree(void* ptr);
extern void* kmalloc(size_t size);
extern void* krealloc(void* ptr, size_t size);
extern int kprintf(const char* fmt, ...);
extern DIR* opendir(char* dirname);
extern struct dirent* readdir(DIR* dir);
extern int closedir(DIR* dir);
extern unsigned int total_sub_dirs;
extern char** sub_dirnames;
extern char* current_dir;

int index;
unsigned int num_args; // The number of arguments passed to the command

extern void terminal_clear_screen();
extern bool readdir_alternative(char* dirname);

void shell();
int command_count();
char* read_line();
char** split_line(char* line);
void (*fn_lookup(char* fname))();
void xsh_help();
void xsh_echo(char** args);
void xsh_exit();
void xsh_clear_screen();
void xsh_cat(char** args);
void xsh_touch(/*char** args*/);
void xsh_cd(char** args);
void xsh_ls(char** args);
void xsh_pwd();
void xsh_rm(char** args);
void xsh_rmdir(char** args);

#endif
