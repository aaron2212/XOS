#ifndef _VFS_H
#define _VFS_H

/*
    Constants: VFS constants
        MAX_MOUNTS = 64 - the maximum number of filesystems that can be mounted
        FS_NAME_LEN = 16 - the maximum length for the name of a filesystem
        FS_TYPE_LEN = 16the maximum length for the type of a filesystem
        FS_MOUNT_POINT_LEN = 32 - the maximum length for the mount point of a filesystem
*/

#define MAX_MOUNTS 64
#define FS_NAME_LEN 16
#define FS_TYPE_LEN 16
#define FS_MOUNT_POINT_LEN 32

#include <stdbool.h>

#include "../../libc/include/size_t.h"
#include "../../libc/include/stdint.h"

extern int kprintf(const char *fmt, ...);
extern void *memset(void *ptr, int value, size_t size);
extern size_t strlen(const char *str);
extern char *strcpy(char *dst, const char *src);
extern char *strncpy(char *dst, const char *src, size_t count);
extern char *strcat(char *str, const char *src);
extern int strcmp(const char *str1, const char *str2);
extern int strncmp(const char *str1, const char *str2, int n);
extern char *strncat(char *dst, const char *src, size_t count);
extern char *strtok(char *str, const char *delim);
extern void set_error(const char *message, ...);
extern void *kcalloc(size_t size, size_t num);
extern void kfree(void *ptr);
extern const char *strchr(const char *str, int ch);

/*
    struct: dirent
    A high level abstraction of the directory in the filesystem

    uint8_t* name - name of the entry within the  directory
*/
struct dirent
{
    uint8_t *name;
};

/*
    struct: _FILE typedef'd as FILE
    A high level abstraction of a file in the filesystem

    uint8_t mode - the mode the file is opened in
    uint64_t size - the size of the file
    uint16_t fd - the file's file descriptor
    int8_t eof - used to mark if the file file pointer has reached the end of the file
    uint64_t position - the position of the file pointer within the file
    uint64_t start_block - the starting block of the file in the filesystem
    uint8_t *offset - the offset of the file in the index area of the filesystem
*/
typedef struct _FILE
{
    uint8_t mode;
    uint64_t size;
    uint16_t fd;
    int8_t eof;
    uint64_t position;
    uint64_t start_block;
    uint8_t *offset; // The offset of the index area entry on disk/in memory
} FILE;

/*
    struct: _DIR typedef'd as DIR
    A high level abstraction of a directory in the filesystem

    uint16_t fd - the directory's file description
    int8_t *dirname - the name of the directory
    uint32_t current_dir
    uint32_t total_entries - the total entries within this directory
    uint8_t *offset - the offset of the directory in the index area of the filesystem
    struct dirent **entries - a list of directory entries inside this directory
*/
typedef struct _DIR
{
    uint16_t fd;
    int8_t *dirname;
    uint32_t current_dir;
    uint32_t total_entries;
    uint8_t *offset;
    struct dirent **entries;
} DIR;

/*
    struct: filesystem
    A structure used to store properties and I/O operations for use with each filesystem

    char name[FS_NAME_LEN] - the name of the filesystem
    char type[FS_TYPE_LEN] - the type of the filesystem
    char mount_point[FS_MOUNT_POINT_LEN] - the filesystem's mount point
    bool readonly - whether the filesystem is in readonly mode, or is readable and writeable
    void (*init)() - used to initalize the filesystem (copy information into its superblock)
    FILE *(*open)(char *filename) - open a file
    int (*read)(FILE *stream, char *buf, size_t len) - read a file
    void (*write)(FILE *stream, const char *buf, size_t len) - write a file
    FILE *(*create)(char *filename) - create a file
    DIR *(*opendir)(char *dirname) - open a directory
    struct dirent *(*readdir)(DIR *dir) - read the contents of a directory
    int (*changedir)(char *dirname) - change the current working directory
    int (*closedir)(DIR *) - close the current directory
    bool (*rm)(char *filename) - remove a file from the filesystem
    bool (*rmdir)(char *dirname) - remove a directory from the filesystem
*/
struct filesystem
{
    char name[FS_NAME_LEN];
    char type[FS_TYPE_LEN];
    char mount_point[FS_MOUNT_POINT_LEN];
    bool readonly;
    void (*init)();
    FILE *(*open)(char *filename);
    int (*read)(FILE *stream, char *buf, size_t len);
    void (*write)(FILE *stream, const char *buf, size_t len);
    FILE *(*create)(char *filename);
    DIR *(*opendir)(char *dirname);
    struct dirent *(*readdir)(DIR *dir);
    int (*changedir)(char *dirname);
    int (*closedir)(DIR *);
    bool (*rm)(char *filename);
    bool (*rmdir)(char *dirname);
};

/*
    enum: fs_errors
    Filesystem related errors that may occur when registering the filesystem or accessing files

    TOO_MANY_MOUNTS = 0 - the maximum number of filesystem has already been mounted
    INVALID_STRUCTURE = 1 - the filesystem structure is invalid
    NAME_TOO_LONG = 2 - the name of the filesystem is too long
    TYPE_TOO_LONG = 3 - the type of the filesystem is too long
    MOUNT_POINT_TOO_LONG = 4 - the mount point for the filesystem is too long
    IO_OPERATION_NOT_SET = 5 - one or more of the I/O operations (function pointers) have not been assigned to
    FS_ALREADY_MOUNTED = 6 - the filesystem has already been mounted
    PATH_NOT_FOUND = 7 - the path was not found in the filesystem
    INVALID_FILENAME = 8 - the file has an invalid name
*/
enum fs_errors
{
    TOO_MANY_MOUNTS = 0,
    INVALID_STRUCTURE = 1,
    NAME_TOO_LONG = 2,
    TYPE_TOO_LONG = 3,
    MOUNT_POINT_TOO_LONG = 4,
    IO_OPERATION_NOT_SET = 5,
    FS_ALREADY_MOUNTED = 6,
    PATH_NOT_FOUND = 7,
    INVALID_FILENAME = 8,
};

struct filesystem mount_points[MAX_MOUNTS]; // The list of mount points
unsigned int mount_index;

void init_mounts();
int register_fs(struct filesystem fs);
void unregister_fs(struct filesystem fs);
int set_fs_error(enum fs_errors error_code);
int find_fs_by_filename(const char *filename);
struct filesystem get_fs();
int remove_empty_strings(char **array, unsigned int array_size);
void make_proper_path(char *path);
void make_full_path(char *path);

// VFS I/O operations
FILE *vfs_open(char *filename, const char *mode);
int vfs_read(FILE *stream, char *buf, size_t len);
void vfs_close(FILE *stream);
FILE *vfs_create(char *filename);
int vfs_changedir(char *dirname);
DIR *vfs_opendir(char *dirname);
struct dirent *vfs_readdir(DIR *dir);
int vfs_closedir(DIR *dir);
bool vfs_rm(char *filename);
bool vfs_rmdir(char *dirname);

#endif
