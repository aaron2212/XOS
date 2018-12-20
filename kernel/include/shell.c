#include "shell.h"
#include "../../libc/include/stdio/stdio.h"

extern void* kmalloc(size_t size);
extern void* krealloc(void* ptr, size_t size);

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

	return NULL;
}

void shell()
{
	// TODO: change printf('$ ') to the format `user@host:dir$ `

	char* line;
	char** args;

	printf("%s$ ", current_dir);

	line = read_line();

	if (strcmp(line, "") != 0) {
		args = split_line(line);

		for (unsigned int i=0; i<sizeof(builtin_commands)/sizeof(builtin_commands[0]); i++) {
			if (strcmp(args[0], builtin_commands[i]) == 0) {
				fn_lookup(args[0])(args);

				// Do not print an extra new line if the terminal screen was cleared
				if (strcmp(args[0], "clear") != 0) {
					printf("\n");
				}

				return;
			} else if (i == sizeof(builtin_commands)/sizeof(builtin_commands[0])-1) {
				// TODO: check if command is name of program and start a process
				printf("Unknown command. Try \"help\" for a list of commands\n");

				return;
			}

		}

		kfree(args);
	}

	kfree(line);
}

char* read_line()
{
	int buffer_size = BUF_SIZE;
	char* buffer = kmalloc(buffer_size);
	int c;
	index = 0;

	if (!buffer) {
		printf("Failed to allocate memory!");
		// exit(EXIT_FAILURE);
	}

	while (1) {
		c = getchar();

		if (c == '\n') {
			buffer[index++] = '\0';
			return buffer;
		} else if (c != 0x08) {
			buffer[index++] = c;
		}

		if (index >= buffer_size) {
			buffer_size += BUF_SIZE;
			buffer = (char*) krealloc(buffer, buffer_size);

			if (!buffer)
				printf("Failed to allocate memory!");
		}
	}

	// return buffer;
}

char** split_line(char* line)
{
	int buffer_size = TOK_BUF_SIZE;
	int pos = 0;

	char** tokens = (char**) kmalloc(buffer_size);
	char* token;

	if (!tokens) {
		printf("Failed to allocate memory!");
		// exit(EXIT_FAILURE);
	}

	token = strtok(line, TOK_DELIM); // correct always

	while (token != NULL) {
		tokens[pos] = token;
		pos++;

		if (pos >= buffer_size) {
			buffer_size += BUF_SIZE;
			tokens = (char**) krealloc(tokens, buffer_size * sizeof(char*));

			if (!tokens)
				printf("Failed to allocate memory!");
				// exit(EXIT_FAILURE);
		}

		token = strtok(NULL, TOK_DELIM);
		numArgs++;
	}

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
	for (int i=1; i<numArgs; i++) {
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
	terminal_clear_screen();
}
