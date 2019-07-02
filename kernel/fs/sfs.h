#ifndef _SFS_H
#define _SFS_H

/*
    Constants: SFS constants
        BYTES_PER_BLOCK = 4096 - the number of bytes in each block in the filesystem
        TOTAL_BLOCKS_OFFSET = 432
        ENTRY_SIZE = 64 - the size of an entry in the index area
        MAGIC = "SFS" - the filesystem magic number
        VOLUME_NAME = "SFS" - the name of the volume
        DIRECTORY_NAME_ENTRY_LEN = 54 - the maximum length of a directory name that can be stored in a *directory_entry* structure until one or more *continuation_entry* structures are needed
        FILENAME_ENTRY_LEN = 30 - the maximum length of a file name that can be stored in a *file_entry* structure until one or more *continuation_entry* structures are needed
        DIRECTORY_NAME_OFFSET = 10 - the offset into the *directory_entry* structure where the name of the directory begins
        FILENAME_OFFSET = 34 - the offset into the *file_entry* structure where the name of the file begins
*/

#define BYTES_PER_BLOCKS 4096   // Const: BYTES_PER_BLOCK
#define TOTAL_BLOCKS_OFFSET 432 // Const: TOTAL_BLOCKS_OFFSET
#define ENTRY_SIZE 64
#define MAGIC "SFS"
#define VOLUME_NAME "SFS"

#define DIRECTORY_NAME_ENTRY_LEN 54
#define FILENAME_ENTRY_LEN 30

#define DIRECTORY_NAME_OFFSET 10
#define FILENAME_OFFSET 34

#include "../../libc/include/size_t.h"
#include "../../libc/include/stdint.h"
#include "vfs.h"

extern int kprintf(const char *fmt, ...);
extern void *memset(void *ptr, int value, size_t count);
extern void *memcpy(void *dest, const void *src, size_t count);
extern char *strncpy(char *dst, const char *src, size_t count);
extern char *strcat(char *dst, const char *src);
extern char *strncat(char *dest, const char *src, size_t count);
extern int strcmp(const char *str1, const char *str2);
extern int strncmp(const char *str1, const char *str2, int n);
extern size_t strlen(const char *str);
extern void *kmalloc(size_t size);
extern void *kcalloc(size_t num, size_t size);
extern void kfree(void *ptr);
extern void set_error(const char *message, ...);

extern unsigned char *rootfs_start;            // The start of the filesystem
extern unsigned char *rootfs_end;              // The end of the filesystem
extern unsigned short current_file_descriptor; // The integer of the next file's file descriptor (ID)
unsigned char *index_start;                    // The start of the index area

void init();
FILE *open(char *filename);
void close();
int read(FILE *stream, char *buf, size_t len);
void write();
FILE *create(char *filename);
struct dirent *readdir(DIR *dir);
DIR *opendir(char *dirname);
int changedir(char *dirname);
int closedir(DIR *dir);
bool rm(char *filename);
bool rmdir(char *dirname);

char *get_entry_name_by_offset(char *pos);
unsigned char *get_offset_by_entry_name(char *name);

/*
    Enum: entries
    Entry types

    STARTING_MARKER_ENTRY = 0x02 - marks the beginning of the index area
    UNUSED_ENTRY = 0x10 - marks an entry as unused
    DIRECTORY_ENTRY = 0x11 - marks an entry as a directory
    FILE_ENTRY = 0x12 - marks an entry as a file
    UNUSABLE_ENTRY = 0x18 - marks an entry as unusable
    DELETED_DIRECTORY_ENTRY = 0x19 - marks an entry as a directory the was deleted
    DELETED_FILE_ENTRY = 0x1A - marks an entry as a file that was deleted
    VOLUME_IDENTIFIER_ENTRY = 0x01 - marks the end of the volume
 */
enum entries
{
    STARTING_MARKER_ENTRY = 0x02,
    UNUSED_ENTRY = 0x10,
    DIRECTORY_ENTRY = 0x11,
    FILE_ENTRY = 0x12,
    UNUSABLE_ENTRY = 0x18,
    DELETED_DIRECTORY_ENTRY = 0x19,
    DELETED_FILE_ENTRY = 0x1A,
    VOLUME_IDENTIFIER_ENTRY = 0x01
};

/*
    struct: superblock
    The filesystem superblock

    uint8_t reserved0[11] - reserved for compatibility (boot code)
    uint8_t reserved1[21] - reserved for compatibility (legacy BIOS parameter block)
    uint8_t reserved2[372] - reserved for compatibility (boot code)
    uint64_t last_alteration_time - the time the filesystem was last altered
    uint64_t data_area_size - size of the data area in blocks
    uint64_t index_area_size - size of the index area in bytes
    uint8_t magic[3] - magic number (0x534653 = *SFS*)
    uint8_t version - the filesystem version number (0x10 = 1.0)
    uint64_t total_blocks - the total number of blocks in the filesystem
    uint32_t reserved_blocks - the number of reserved blocks in the filesystem (including the superblock)
    uint8_t block_size - the size of each block in the filesystem
    uint8_t checksum - the checksum for integrity checking
    uint8_t reserved3[64] - reserved for compatibility (partition table)
    uint16_t reserved4 - reserved for compatibility (boot signature)
 */
struct superblock
{
    uint8_t reserved0[11];         // reserved for compatibility (boot code)
    uint8_t reserved1[21];         // reserved for compatibility (legacy BIOS parameter block)
    uint8_t reserved2[372];        // reserved for compatibility (boot code)
    uint64_t last_alteration_time; // timestamp
    uint64_t data_area_size;       // size of data area in blocks
    uint64_t index_area_size;      // size of index area in bytes
    uint8_t magic[3];              // magic number (0x534653 ['SFS'])
    uint8_t version;               // SFS version number (0x10 = 1.0)
    uint64_t total_blocks;         // total number of blocks
    uint32_t reserved_blocks;      // number of reserved blocks (including the superblock)
    uint8_t block_size;            // block size
    uint8_t checksum;              // checksum
    uint8_t reserved3[64];         // reserved for compatibility (partition table)
    uint16_t reserved4;            // reserved for compatibility (boot signature)
} __attribute__((packed));

/*
    struct: directory_entry
    Represents a directory in the filesystem

    uint8_t type - the type of entry (0x11 for a directory entry)
    uint8_t num_cont_entries - the number of continuation entries following this entry
    uint64_t timestamp - the time at which the directory was last modified, or created if it was never modified
    uint8_t name[54] - the name of the directory
*/
struct directory_entry
{
    uint8_t type;             // entry type (0x11 for directory entry)
    uint8_t num_cont_entries; // number of continuation entries following this entry
    uint64_t timestamp;       // timestamp
    uint8_t name[54];         // directory name
} __attribute__((packed));

/*
    struct: file_entry
    Represents a file in the filesystem

    uint8_t type - the type of entry (0x12 for a file entry)
    uint8_t num_cont_entries - the number of continuation entries following this entry
    uint64_t timestamp - the time at which the directory was last modified, or created if it was never modified
    uint64_t start_block - the starting block number in the data area
    uint64_t end_block - the ending block number in the data area
    uint64_t size - the size of the file in bytes
    uint8_t filename[30] - the name of the file in UTF-8
*/
struct file_entry
{
    uint8_t type;             // entry type (0x12 for file entry)
    uint8_t num_cont_entries; // number of continuation entries following this entry
    uint64_t timestamp;       // timestamp
    uint64_t start_block;     // starting block number in the data area
    uint64_t end_block;       // ending block number in the data area
    uint64_t size;            // file size in bytes
    uint8_t filename[30];     // filename in UTF-8
} __attribute__((packed));

/*
    struct: unusable_entry
    Represents an unusable entry

    uint8_t type - the type of entry (0x18 for an unusable entry)
    uint8_t reserved0[9] - unused
    uint64_t start_block - the starting block number in the data area
    uint64_t end_block - the ending block number in the data area
    uint8_t reserved1[38] - unused
*/
struct unusable_entry
{
    uint8_t type;          // entry type (0x18 for unusable)
    uint8_t reserved0[9];  // unused
    uint64_t start_block;  // the starting block number in the data area
    uint64_t end_block;    // the ending block number in the data area
    uint8_t reserved1[38]; // unused
};

/*
    struct: deleted_directory_entry
    Repesents a directory that was deleted

    uint8_t type - the type of entry (0x19 for a deleted directory entry)
    uint8_t num_cont_entries - the number of continuation entries following this entry
    uint64_t timestamp - the time when the directory was deleted
    uint8_t reserved[54] - unused
*/
struct deleted_directory_entry
{
    uint8_t type;             // entry type (0x19 for deleted directory)
    uint8_t num_cont_entries; // number of continuation entries following this entry
    uint64_t timestamp;       // the time when the directory was deleted
    uint8_t reserved[54];     // unused
} __attribute__((packed));

/*
    struct: deleted_file_entry
    Represents a file that was deleted

    uint8_t type - the type of entry (0x1A for a deleted file entry)
    uint8_t num_cont_entries - the number of continuation entries following this entry
    uint64_t timestamp - the time when the file was deleted
    uint64_t start_block - the starting block number in the data area
    uint64_t end_block - the ending block number in the data area
    uint64_t size - the length of the deleted file
    uint8_t filename[30] -  the name of the deleted file
*/
struct deleted_file_entry
{
    uint8_t type;             // entry type (0x1A for deleted file)
    uint8_t num_cont_entries; // number of continuation entries following this entry
    uint64_t timestamp;       // the time when the file was deleted
    uint64_t start_block;     // the starting block number in the data area
    uint64_t end_block;       // the ending block number in the data area
    uint64_t size;            // the length of the deleted file
    uint8_t filename[30];     // the name of the deleted file
} __attribute__((packed));

/*
    struct: continuation_entry
    Represents a continuation entry. This entry contains directory and file names
    that are too long to fit into a single entry

    uint8_t name[64] - the continued name of the file or directory name that was too long
*/
struct continuation_entry
{
    uint8_t name[64]; // continued name of the file or directory name that was too long
} __attribute__((packed));

/*
    struct: unused_entry
    Represents an unused entry

    uint8_t type - the type of the entry (0x10 for unused)
    uint8_t reserved[63] - unused
*/
struct unused_entry
{
    uint8_t type;         // entry type (0x10 for unused)
    uint8_t reserved[63]; // unused
} __attribute__((packed));

/*
    struct: starting_marked_entry
    Marks the beginning of the index area

    uint8_t type - the type of the entry (0x02 for starting marker)
    uint8_t reserved[63] - unused
*/
struct starting_marker_entry
{
    uint8_t type;         // entry type (0x02 for a starting marker entry)
    uint8_t reserved[63]; // unused
} __attribute__((packed));

/*
    struct: starting_marked_entry
    Marks the end of the volume

    uint8_t type - the type of the entry (0x01 for a volume identifier entry)
    uint8_t reserved[3] - reserved
    uint64_t timestamp - the time that the volume was last altered
    uint8_t volume_name[52] - volume name in UTF-8, null terminated
*/
struct volume_identifier_entry
{
    uint8_t type;            // entry type (0x01 for volume identifier)
    uint8_t reserved[3];     // reserved
    uint64_t timestamp;      // timestamp
    uint8_t volume_name[52]; // volume name in UTF-8, null terminated
} __attribute__((packed));

struct superblock rootfs_superblock;
unsigned int total_rootfs_entries;

/*
    struct: entry
    A structure representing any entry in the filesystem

    int type - the entry type
    union - a struct representing the entry in the filesystem
*/
struct entry
{
    int type;

    union {
        struct directory_entry dir_ent;
        struct file_entry file_ent;
        struct deleted_directory_entry del_dir_ent;
        struct deleted_file_entry del_file_ent;
        struct continuation_entry cont_entry;
        struct starting_marker_entry start_entry;
    } entry;
};

// The total number of files and directories in the filesystem
unsigned int total_rootfs_dirs, total_rootfs_files;

// Get a list of all entries in the filesystem
void find_all_entries(struct entry entries[]);
void get_entry_names(char **entry_names, struct entry entries[], int _entry_type);
void count_files_and_dirs();
bool is_filename_valid();
char *get_full_name(unsigned char *pos);
bool readdir_alternative(char *dirname);
bool path_exists(char *path);
bool is_dir(char *path);
bool is_file(char *path);

#endif