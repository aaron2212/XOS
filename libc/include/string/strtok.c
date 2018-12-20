#include "string.h"
#include "../_null.h"

#define DICT_LEN 512

extern void* kmalloc(size_t size);
extern void kfree(void* ptr);

int *create_delim_dict(char* delim)
{
    int* d = (int*) kmalloc(sizeof(int) * DICT_LEN);
    memset((void*) d, 0, sizeof(int) * DICT_LEN);

    for (unsigned int i=0; i<strlen(delim); i++) {
        d[(int) delim[i]] = 1;
    }

    return d;
}

char* strtok(char* str, char* delim)
{
    static char* last, *to_free;
    int* delim_dict = create_delim_dict(delim);

    if (!delim_dict) {
        return NULL;
    }

    if (str) {
        last = (char*) kmalloc(strlen(str)+1);
        if (!last) {
            kfree(delim_dict);
        }

        to_free = last;
        strcpy(last, str);
    }

    while (delim_dict[(int) *last] && *last != '\0') {
        last++;
    }

    str = last;

    if (*last == '\0') {
        kfree(delim_dict);
        kfree(to_free);

        return NULL;
    }

    while (*last != '\0' && !delim_dict[(int) *last]) {
        last++;
    }

    *last = '\0';
    last++;

    kfree(delim_dict);
    kfree(last);

    return str;
}
