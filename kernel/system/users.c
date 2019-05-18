#include "system.h"

// Get the list of
void init_users() {
    FILE* fp = fopen("/etc/passwd", "r");

    char line[sizeof(struct user)];

    kprintf("");

    while (fgets(line, sizeof(line), fp) != NULL) {
        char** parts = kcalloc(sizeof(struct user) * 4, 1); // The number of members in the user struct
        unsigned int part_index = 0, user_index = 0;
        char* part = strtok(line, ":");

        while (part != NULL) {
            parts[part_index++] = part;
            // kprintf("part=%s\n", part);
            part = strtok(NULL, ":");
        }

        struct user user;

        memset(&user, 0, sizeof(user));

        strcpy(user.username, (char*)parts[0]);
        strcpy(user.password, (char*)parts[1]);
        user.uid = (int)parts[2];
        strcpy(user.home_dir, (char*)parts[3]);

        // kprintf("uid=%s\n", user.uid);
        // kprintf("username=%s\n", user.username);
        // kprintf("password=%s\n", user.password);
        // kprintf("home_dir=%s\n", user.home_dir);

        // users[user_index++] = user;

        memcpy(&users[user_index++], &user, sizeof(user));

        kfree(parts);

        kprintf("size=0x%x\n", sizeof(users));
    }

    // kprintf("xx\n");

    // kprintf("uid=0x%x\n", users[0].uid);
    // kprintf("name=%s\n", users[0].username);
    // kprintf("password=%s\n", users[0].password);
    // kprintf("home_dir=%s\n", users[0].home_dir);
}