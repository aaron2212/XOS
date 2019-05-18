#include "system.h"

// Read the system's hostname from /etc/hostname
void init_host() {
    FILE* fp = fopen("/etc/hostname", "r");
    hostname = kcalloc(MAX_HOSTNAME_LENGTH, 1);
    fread(hostname, MAX_HOSTNAME_LENGTH, 1, fp);
    hostname[strlen(hostname) - 2] = '\0'; // Remove trailing newline
    fclose(fp);
}

char* get_hostname() {
    return hostname;
}