#include "system.h"

// Get the list of
void init_users() {
    FILE* fp = fopen("/etc/passwd", "r");
    char* file_contents = kmalloc(sizeof(struct user) * MAX_USERS);

    // while (!feof(fp)) {
    //     fgetc(fp);
    // }

    fread(file_contents, sizeof(struct user) * MAX_USERS, 1, fp);

    kprintf("/etc/passwd=%s\n", file_contents);

    kfree(users);
}