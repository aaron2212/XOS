#ifndef _MODULES_H
#define _MODULES_H

/*
    Constants: constants for loading GRUB modules
        FS_MODULE_MAGIC = "SFS" - the magic that is used to find the root filesystem
        FS_MODULE_MAGIC_OFFSET = 0x1AC - the offset for the root filesystem magic in the GRUB module (filesystem)
*/
#define FS_MODULE_MAGIC "SFS"
#define FS_MODULE_MAGIC_OFFSET 0x1AC

extern int strncmp(const char *str, const char *str2, int n);
extern void *memcpy(void *dest, const void *src, size_t size);
extern void set_error(const char *message, ...);

// Load a GRUB multiboot module
int *load_module(multiboot_info_t *mbinfo, const char *module);
int *load_module_by_magic(multiboot_info_t *mbinfo, const char *magic, int offset);

#endif
