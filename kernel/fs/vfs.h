#ifndef _VFS_H
#define _VFS_H

#define MAX_MOUNTS           64
#define FS_NAME_LEN          16
#define FS_TYPE_LEN          16
#define FS_MOUNT_POINT_LEN   32

#include <stdbool.h>

#include "../../libc/include/size_t.h"
#include "../../libc/include/stdint.h"

extern int printf(const char* fmt, ...);
extern void* memset(void* ptr, int value, size_t size);
extern size_t strlen(const char* str);
extern char* strcpy(char* dst, const char* src);
extern char* strcat(char* str, const char* src);
extern int strcmp(const char* str1, const char* str2);
extern int strncmp(const char* str1, const char* str2, int n);
extern void set_error(const char* message, ...);
extern void* kcalloc(size_t size, size_t num);
extern void kfree(void* ptr);
extern const char* strchr(const char* str, int ch);

// FILE structure, as return by vfs_open()
typedef struct _FILE {
	uint8_t   flags[4];
	uint64_t  size;
	uint16_t  fd;
	uint8_t   eof;
	uint64_t  position;
	uint64_t  start_block;
    int8_t*   offset; // The offset of the index area entry on disk/in memory
} FILE;


// Each filesystem has its own properties and I/O operations which are used for files
struct filesystem {
    char name[FS_NAME_LEN];
    char type[FS_TYPE_LEN];
    char mount_point[FS_MOUNT_POINT_LEN];
    bool readonly;
    FILE* (*open)(const char *filename);
    void (*close)(FILE stream);
    void (*read)(FILE* stream, char* buf, size_t len);
    void (*write)(FILE* stream, const char* buf, size_t len);
    void (*readdir)(/*DIR* dir*/);
    void (*mount)();
    void (*umount)();
    /*void (*init)()*/
};

// Filesystem related errors that may occur when registering the filesystem or accessing files
enum fs_errors {
    TOO_MANY_MOUNTS         = 0,
    INVALID_STRUCTURE       = 1,
    NAME_TOO_LONG           = 2,
    TYPE_TOO_LONG           = 3,
    MOUNT_POINT_TOO_LONG    = 4,
    IO_OPERATION_NOT_SET    = 5,
    FS_ALREADY_MOUNTED      = 6,
    PATH_NOT_FOUND          = 7
};

struct filesystem mount_points[MAX_MOUNTS]; // The list of mount points
unsigned int mount_index;

void init_mounts();
int register_fs(struct filesystem fs);
void unregister_fs(struct filesystem fs);
int set_fs_error(enum fs_errors error_code);
int find_fs_by_filename(const char* filename);

// VFS I/O operations
FILE* vfs_open(const char* filename, const char* mode);

#endif