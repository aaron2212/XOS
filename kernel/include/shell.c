#include "shell.h"
#include "../system/system.h"
#include "../system/syscalls/syscalls.h"

/*
    Struct: fn_table_entry
    The command text, along with a function that is executed when the command is typed
*/
typedef struct fn_table_entry {
    char* name;
    void (*fn)();
} fn_table_entry_t;

/*
    Array: fn_table
    Stores all the shell commands with their corresponding functions to be called when the command is entered
*/
fn_table_entry_t fn_table[] = {
    {"help", xsh_help},
    {"echo", xsh_echo},
    {"exit", xsh_exit},
    {"clear", xsh_clear_screen},
    {"cat", xsh_cat},
    {"touch", xsh_touch},
    {"cd", xsh_cd},
    {"ls", xsh_ls},
    {"pwd", xsh_pwd},
    {"rm", xsh_rm},
    {"rmdir", xsh_rmdir},
    {"hostname", xsh_hostname},
    {"login", xsh_login},
    {"uptime", xsh_uptime},
};

char* builtin_commands[] = {
    "help",
    "echo",
    "exit",
    "clear",
    "cat",
    "touch",
    "cd",
    "ls",
    "pwd",
    "rm",
    "rmdir",
    "hostname",
    "login",
    "uptime"
};

/*
    Func: fn_lookup
    Searches for a valid command, and calls the function corresponding to it
    
    Parameters:
        fname - the command
    
    Returns:
        A pointer to the function to be called when the command in entered, or NULL if no command was found
*/
void (*fn_lookup(char* fname))() {
    for (unsigned int i = 0; i < sizeof(fn_table) / sizeof(fn_table[0]); i++) {
        if (strcmp(fname, fn_table[i].name) == 0) {
            return fn_table[i].fn;
        }
    }

    return NULL;
}

/*
    Func: shell
    The shell that handles use input
*/
void shell() {
    char* line;
    char** args;

    struct user current_user = get_current_user();

    char prompt = strcmp(current_user.username, "root") == 0 ? '#' : '$';

    kprintf("%s@%s:%s%c ", current_user.username, get_hostname(), current_dir, prompt);

    line = read_line();

    if (strcmp(line, "") != 0) {
        args = split_line(line);

        for (unsigned int i = 0; i < sizeof(builtin_commands) / sizeof(builtin_commands[0]); i++) {
            if (strcmp(args[0], builtin_commands[i]) == 0) {
                fn_lookup(args[0])(args);

                // Do not print an extra new line if the terminal screen was cleared
                if (strcmp(args[0], "clear") != 0) {
                    // kprintf("\n");
                }

                return;
            } else if (i == sizeof(builtin_commands) / sizeof(builtin_commands[0]) - 1) {
                // TODO: check if command is name of program and start a process
                kprintf("%s: command not found\n", args[0]);

                return;
            }
        }

        kfree(args);
    }

    kfree(line);
}

/*
    Func: read_line
    Read the inputted line

    Returns:
        the inputted string
*/
char* read_line() {
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
            buffer = (char*)krealloc(buffer, buffer_size);

            if (!buffer)
                kprintf("Failed to allocate memory!");
        }
    }

    // return buffer;
}

/*
    Func: split_line
    Split the inputted line by _TOK_DELIM_

    Parameters:
        line - the inputted line to split
    
    Returns:
        An array of strings containing the tokens after splitting the line
*/
char** split_line(char* line) {
    unsigned int i = 0; // The current position within the `args` array

    unsigned int num_spaces = 0;
    num_args = 0; // Reset the number of arguments for each command

    // Determine the number of spaces in the line so that we know how much memory to allocate
    for (unsigned int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ')
            num_spaces++;
    }

    // Allocate memory for the number of arguments
    char** args = kcalloc(num_spaces + 1, 1);
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

/*
    Func: xsh_help
    Print a help message displaying the list of available commands
*/
void xsh_help() {
    kprintf(
        "echo:   print to the screen\n"
        "help:   show this help information\n"
        "exit:   exits the shell (or OS if this is the only shell)\n"
        "clear:  clears the screen\n"
        "cat:    display the contents of a file\n"
        "touch:  create a new file (not yet implemented)\n"
        "cd:     change the current working directory\n"
        "ls:     list the contents of a directory\n"
        "pwd:    display the current working directory\n"
        "rm:     remove a file\n"
        "rmdir:  remove a directory and its contents\n"
        "uptime: get the system uptime in seconds\n");
}

/*
    Func: xdh_echo
    Print the text after the _echo_ command
*/
void xsh_echo(char** args) {
    for (unsigned int i = 1; i < num_args; i++) {
        if (args[i] != NULL) {
            kprintf("%s ", args[i]);
        }
    }

    kprintf("\n");
}

/*
    Func: xsh_exit
    Exit the shell
*/
void xsh_exit() {
    kprintf("Exit requested!\n");
}

/*
    Func: xsh_clear_screen
    Clear the terminal screen
*/
void xsh_clear_screen() {
    terminal_clear_screen();
}

/*
    Func: xsh_cat
    Display the contents of a file
*/
void xsh_cat(char** args) {
    // The name of the file to read
    char* filename = args[1];

    if (strlen(filename) == 0 || filename == NULL) {
        kprintf("cat: no file specified\n");
        return;
    }

    // Attempt to open a file stream with the given filename
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        kprintf("cat: %s: no such file\n", filename);
        return;
    }

    // The file is valid. So read its contents
    char buffer[fp->size + 1];

    fread(buffer, sizeof(buffer), 1, fp);

    buffer[fp->size + 1] = '\0';

    kprintf("%s\n", buffer);

    fclose(fp);
}

/*
    Func: xsh_touch
    Create a new file
*/
void xsh_touch(char** args) {
    char* filename = args[0];

    if (filename == NULL || strncmp(filename, "", 1) == 0) {
        kprintf("touch: missing file operand\n");
        return;
    }

    sys_creat(filename);
}

/*
    Func: xsh_cd
    Change the current working directory
*/
void xsh_cd(char** args) {
    // Get the directory name to change to
    char* dirname = args[1];

    // Only attempt to change the directory if the user specified a directory
    if (strcmp(dirname, "") != 0) {
        // result=-1 means failed to find directory matching `dirname`
        int result = sys_chdir(dirname);

        if (result == -1)
            kprintf("cd: %s: no such directory\n", dirname);
    }
}
/*
    Func: xsh_ls
    Display the contents of the current directory
*/
void xsh_ls(char** args) {
    char* dirname = args[1];

    if (strncmp(dirname, "", 1) == 0) {
        strcpy(dirname, current_dir);
    }

    if (!readdir_alternative(dirname)) {
        kprintf("ls: cannot access '%s': no such file or directory\n", dirname);
    }
}

/*
    Func: xsh_pwd
    Print the current working directory
*/
void xsh_pwd() {
    kprintf("%s\n", current_dir);
}

/*
    Func: xsh_rm
    Remove a file from the filesystem
*/
void xsh_rm(char** args) {
    char* filename = args[1];

    if (strlen(filename) == 0 || filename == NULL) {
        kprintf("rm: missing operand\n");
        return;
    }

    if (!vfs_rm(filename)) {
        kprintf("rm: cannot remove '%s': No such file\n", filename);
    }
}

/*
    Func: xsh_rmdir
    Remove a directory and its contents from the filesystem
*/
void xsh_rmdir(char** args) {
    char* dirname = args[1];

    if (strlen(dirname) == 0 || dirname == NULL) {
        kprintf("rmdir: missing operand\n");
        return;
    }

    if (!vfs_rmdir(dirname)) {
        kprintf("rmdir: failed to remove '%s': no such directory\n", dirname);
    }
}

/*
    Func: xsh_hostname
    Print the system's hostname
*/
// Get the hostname for the system
void xsh_hostname() {
    kprintf("%s\n", get_hostname());
}

/*
    Func: xsh_login
    Login as a user using a username and password
*/
void xsh_login() {
    char* username = kmalloc(MAX_USERNAME_LENGTH);
    char* password = kmalloc(MAX_PASSWORD_LENGTH);

    bool login_success = false;

    while (!login_success) {
        kprintf("Username: ");
        gets(username);

        kprintf("Password: ");
        gets(password);

        for (unsigned int i = 0; i < total_user_count; i++) {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
                login_success = true;
                set_current_user(users[i]);
            }
        }

        if (!login_success)
            kprintf("Login incorrect\n");
    }
}

/*
    Func: xsh_uptime
    Print the system uptime in seconds
*/
void xsh_uptime()
{
    kprintf("0x%x seconds\n", get_uptime());
}