#include "../../libc/include/stdlib/stdlib.h"
#include "system.h"

// Get the list of
void init_users() {
    FILE* fp = fopen("/etc/passwd", "r");

    char line[sizeof(struct user)];

    kprintf("");

    for (unsigned int i = 0; i < 2; i++) {
        memset(&users[i], 0, sizeof(struct user));
    }

    unsigned int j = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        char** parts = kcalloc(sizeof(struct user) * 4, 1); // The number of members in the user struct
        unsigned int part_index = 0;
        char* part = strtok(line, ":");

        while (part != NULL) {
            parts[part_index++] = part;
            part = strtok(NULL, ":");
        }

        struct user user;

        memset(&user, 0, sizeof(user));

        strcpy(user.username, (char*)parts[0]);
        strcpy(user.password, (char*)parts[1]);
        user.uid = atoi(parts[2]);
        strcpy(user.home_dir, (char*)parts[3]);

        // kprintf("uid=%s\n", user.uid);
        // kprintf("username=%s\n", user.username);
        // kprintf("password=%s\n", user.password);
        // kprintf("home_dir=%s\n", user.home_dir);

        // users[j++] = user;
        total_user_count = j;

        memcpy(&users[j++], &user, sizeof(user));

        kfree(parts);
    }

    total_user_count = j;
    current_user = users[0];

    // kprintf("uid=0x%x\n", users[1].uid);
    // kprintf("username=%s\n", users[1].username);
    // kprintf("password=%s\n", users[1].password);
    // kprintf("home_dir=%s\n", users[1].home_dir);
}

// Return a structure representing the current user
struct user get_current_user() {
    return current_user;
}

// Change the user
bool set_current_user(struct user new_current_user) {
    for (unsigned int i = 0; i < total_user_count; i++) {
        if (users[i].uid == new_current_user.uid) {
            current_user = new_current_user;
            return true;
        }
    }

    return false;
}