#include "system.h"
#include "host.c"
// #include "users.c"

// Initialized system configuration
void init_system() {
    init_host();
    init_users();
}