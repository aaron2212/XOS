#ifndef _SHELL_H
#define _SHELL_H

#define BUF_SIZE	 1024
#define TOK_BUF_SIZE 64
#define TOK_DELIM 	 " \t\r\n\a\""

extern int strcmp(const char* str1, const char* str2);
extern char* strtok(char* str, const char* delim);
extern void kfree(void* ptr);

extern char* current_dir;

int index;
unsigned int num_args; // The number of arguments passed to the command

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

#endif
