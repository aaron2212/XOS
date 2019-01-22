#ifndef _FS_H
#define _FS_H

#define ROOTFS_NAME         "SFS"
#define ROOTFS_TYPE         "rootfs"
#define ROOTFS_MOUNT_POINT  "/"
#define CURRENT_DIR_BUF     1024
#define PATH_SEPARATOR      "/"
#define PATH_SEPARATOR_CHAR '/'
#define ROOT_DIR            '/'

#define MAX_OPEN_FILES     32768

#include "../../libc/include/size_t.h"
#include "../../include/multiboot.h"
#include "vfs.h"

extern char* strcpy(char *dst, const char *src);
extern void* kmalloc(size_t size);
extern void* kcalloc(size_t num, size_t size);
extern int* load_module(multiboot_info_t* mbinfo, const char* module);
extern void* memcpy(void* dst, const void* src, size_t size);
extern void* memset(void* ptr, int value, size_t count);

extern unsigned char* rootfs_start;
extern unsigned char* rootfs_end;
extern unsigned int kernel_end;
extern unsigned int heap_size;
extern unsigned int available_memory;

// Handle all filesystem related tasks
int init_fs(multiboot_info_t* mbinfo);

// Initialize and setup the rootfs filesystem
int init_rootfs(multiboot_info_t* mbinfo);

char* current_dir;

// The file descriptor to use when opening a file
unsigned short current_file_descriptor;

// A list of filesystem structures where the file stream's file descriptor is used as an index into the array
struct filesystem file_streams[MAX_OPEN_FILES];

#endif