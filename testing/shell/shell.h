#ifndef _TEST_H
#define _TEST_H

#define BUF_SIZE	 1024
#define TOK_BUF_SIZE 64
#define TOK_DELIM 	 " \t\r\n\a"

int i;

void shell();
int command_count();
char* read_line();
char** split_line(char* line);
void (*fn_lookup(char* fname))();
void xsh_help();
void xsh_echo(char** args);
void xsh_exit();
void xsh_clear_screen();

#endif /* end of include guard: _TEST_H */
