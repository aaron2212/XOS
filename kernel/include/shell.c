#include "shell.h"
#include "../../libc/include/stdio/stdio.h"

extern void* kmalloc(size_t size);
extern void* krealloc(void* ptr, size_t size);
extern int kprintf(const char* fmt, ...);

typedef struct fn_table_entry {
	char* name;
	void (*fn)();
} fn_table_entry_t;

// A list of shell commands with their corresponding functions to be called when the command is entered
fn_table_entry_t fn_table[] = {
							   {"help", xsh_help},
							   {"echo", xsh_echo},
							   {"exit", xsh_exit},
							   {"clear", xsh_clear_screen},
							   {"cat", xsh_cat},
							   {"touch", xsh_touch},
							   {"cd", xsh_cd},
							  };

char* builtin_commands[] = {
	"help",
	"echo",
	"exit",
	"clear",
	"cat",
	"touch",
	"cd",
};

void (*fn_lookup(char *fname))()
{
	for (unsigned int i=0; i<sizeof(fn_table)/sizeof(fn_table[0]); i++) {
		if (strcmp(fname, fn_table[i].name) == 0) {
			return fn_table[i].fn;
		}
	}

	return NULL;
}

void shell()
{
	// TODO: change kprintf('dir$ ') to the format `user@host:dir$ `
	// future TODO: check if user is root and so change "$" to "#"

	char* line;
	char** args;

	kprintf("%s$ ", current_dir);

	line = read_line();

	if (strcmp(line, "") != 0) {
		args = split_line(line);

		for (unsigned int i=0; i<sizeof(builtin_commands)/sizeof(builtin_commands[0]); i++) {
			if (strcmp(args[0], builtin_commands[i]) == 0) {
				fn_lookup(args[0])(args);

				// Do not print an extra new line if the terminal screen was cleared
				if (strcmp(args[0], "clear") != 0) {
					kprintf("\n");
				}

				return;
			} else if (i == sizeof(builtin_commands)/sizeof(builtin_commands[0])-1) {
				// TODO: check if command is name of program and start a process
				kprintf("Unknown command. Try \"help\" for a list of commands\n");

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
		kprintf("Failed to allocate memory!");
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
				kprintf("Failed to allocate memory!");
		}
	}

	// return buffer;
}

char** split_line(char* line)
{
	unsigned int i = 0; // The current position within the `args` array

	unsigned int num_spaces = 0;
	num_args = 0; // Reset the number of arguments for each command

	// Determine the number of spaces in the line so that we know how much memory to allocate
	for (unsigned int i=0; i<strlen(line); i++) {
		if (line[i] == ' ')
			num_spaces++;
	}

	// Allocate memory for the number of arguments
	char** args = kcalloc(num_spaces+1, 1);
	char* arg; 

	if (!args) {
		kprintf("Failed to allocate memory!");
		// exit(EXIT_FAILURE);
	}

	/*
	 * Continue to split the line into arguments while there are still `TOK_DELIM`s
	 */
	arg = strtok(line, TOK_DELIM);

	while (arg != NULL) {
		args[i++] = arg;
		arg = strtok(NULL, TOK_DELIM); // Get the next argument
		num_args++;
	}

	args[i] = NULL;

	return args;
}

// Prints a help message, display a list of available shell commands
void xsh_help()
{
	kprintf(
		"echo:  print to the screen\n"
		"help:  show this help information\n"
		"exit:  exits the shell (or OS if this is the only shell)\n"
		"clear: clears the screen\n"
		"cat:   display the contents of a file\n"
		"touch: create a new file\n"
		"cd:    change the current working directory"
	);
}

// Prints the output after the "echo" command
void xsh_echo(char** args)
{
	for (unsigned int i=1; i<num_args; i++) {
		if (args[i] != NULL) {
			kprintf("%s ", args[i]);
		}
	}
}

// Exits the shell (and possibly the OS too. User should use "shutdown" command if they wish to shutdown the computer)
void xsh_exit()
{
	kprintf("Exit requested!");
}


// Clears the screen
void xsh_clear_screen()
{
	terminal_clear_screen();
}

// Display the contents of a file
void xsh_cat(char** args)
{
	// The name of the file to read
	char* filename = args[1];

	// Attempt to open a file stream with the given filename
	FILE* fp = fopen(filename, "r");

	if (fp == NULL) {
		kprintf("cat: %s: no such file", filename);

		return;
	}
	
	// The file is valid. So read its contents
	char buffer[fp->size+1];

	fread(buffer, sizeof(buffer), 1, fp);

	buffer[fp->size+1] = '\0';

	kprintf("%s\n", buffer);

	fclose(fp);
}

// TODO: check for invalid filename
// Create a new file
void xsh_touch(/*char** args*/)
{
	kprintf("TODO: implement");
}

// Change the current working directory
void xsh_cd(char** args)
{
	// Get the directory name to change to
	char* dirname = args[1];

	// Only attempt to change the directory if the user specified a directory
	if (strcmp(dirname, "") != 0) {
		// result=-1 means failed to find directory matching `dirname`
		int result = vfs_change_dir(dirname);

		if (result == -1)
			kprintf("cd: %s: no such directory", dirname);
	}
}