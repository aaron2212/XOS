#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

int main(void)
{
	for (;;)
		shell();
}

int numArgs;

// Maybe a basic shell that is launched before te rest of the operting systems runs (ie. no processes or users etc, only a fs)

typedef struct fn_table_entry {
	char* name;
	void (*fn)();
} fn_table_entry_t;

fn_table_entry_t fn_table[] = {{"help", xsh_help}, {"echo", xsh_echo}, {"exit", xsh_exit},
								{"clear", xsh_clear_screen}
							   };

char* builtin_commands[] = {
	"help",
	"echo",
	"exit",
	"clear"
};

void (*fn_lookup(char *fname))()
{
	for (unsigned int i=0; i<sizeof(fn_table)/sizeof(fn_table[0]); i++)
	{
		if (strcmp(fname, fn_table[i].name) == 0)
		{
			return fn_table[i].fn;
		}
	}
}

void shell()
{
	// TODO: change printf('$ ') to the format `user@host:dir$ `

	char* line;
	char** args;

	printf("$ ");

	line = read_line();

	args = split_line(line);

	for (unsigned int i=0; i<sizeof(builtin_commands)/sizeof(builtin_commands[0]); i++)
	{
		//printf("%d:%d\n", sizeof(args[0]), sizeof(builtin_commands[i]));

		if (strcmp(args[0], builtin_commands[i]) == 0)
		{
			fn_lookup(args[0])(args);

			printf("\n");
			return;
		}
		else if (i == sizeof(builtin_commands)/sizeof(builtin_commands[0])-1)
		{
			// TODO: check if command is name of program and start a process
			printf("Unknown command. Try \"help\" for a list of commands\n");
			return;
		}

	}

	free(line);
	free(args);
}

char* read_line()
{
	int buffer_size = BUF_SIZE;
	char* buffer = malloc(buffer_size);
	int c;
	i = 0;

	if (!buffer)
		printf("Failed to allocate memory!");

	while (1) {
		c = getchar();

		if (c == '\n')
		{
			buffer[i++] = '\0';
			return buffer;
		}
		else
		{
			buffer[i++] = c;
		}

		if (i >= buffer_size)
		{
			buffer_size += BUF_SIZE;
			buffer = realloc(buffer, buffer_size);

			if (!buffer)
				printf("Failed to allocate memory!");
		}
	}
}

char** split_line(char* line)
{
	int buffer_size = TOK_BUF_SIZE;
	int pos = 0;

	char** tokens = malloc(buffer_size);
	char* token;

	if (!tokens)
		printf("Failed to allocate memory!");
		// exit(EXIT_FAILURE);

	token = strtok(line, TOK_DELIM); // correct always

	while (token != NULL) {
		//printf("token=%s\n", token);
		tokens[pos] = token;
		pos++;

		if (pos >= buffer_size)
		{
			buffer_size += BUF_SIZE;
			tokens = realloc(tokens, buffer_size * sizeof(char*));

			if (!tokens)
				printf("Failed to allocate memory!");
				// exit(EXIT_FAILURE);
		}

		//token = strtok(NULL, TOK_DELIM);
		token = strtok(NULL, TOK_DELIM);
		numArgs++;
	}

	//numArgs -= 1;
	tokens[pos] = NULL;

	return tokens;
}

void xsh_help()
{
	printf(
		"echo:  print to the screen\n"
		"help:  show this help information\n"
		"exit:  exits the shell (or OS if this is the only shell)\n"
		"clear: clears the screen"
	);
}

// Prints the output after the "echo" command
void xsh_echo(char** args)
{
	for (int i=1; i<numArgs; i++)
	{
		if (args[i] != NULL) {
			printf("%s ", args[i]);
		}
	}
}

// Exits the shell (and possibly the OS too. User should use "shutdown" command if they wish to shutdown the computer)
void xsh_exit()
{
	printf("Exit requested!");
}


// Clears the screen
void xsh_clear_screen()
{
	printf("Clear screen requested!");
}
