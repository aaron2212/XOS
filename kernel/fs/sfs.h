#ifndef _SFS_H
#define _SFS_H

#define BYTES_PER_BLOCKS 4096
#define TOTAL_BLOCKS_OFFSET 432
#define ENTRY_SIZE 64
#define MAGIC "SFS"
#define VOLUME_NAME "SFS"

#define DIRECTORY_NAME_ENTRY_LEN 54
#define FILENAME_ENTRY_LEN 30

#define DIRECTORY_NAME_OFFSET 10
#define FILENAME_OFFSET 34

#include "../../libc/include/stdint.h"
#include "../../libc/include/size_t.h"
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

extern unsigned char *rootfs_start;			   // The start of the filesystem
extern unsigned char *rootfs_end;			   // The end of the filesystem
extern unsigned short current_file_descriptor; // The integer of the next file's file descriptor (ID)
unsigned char *index_start;					   // The start of the index area

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
void rmdir(char *dirname);

char *get_entry_name_by_offset(char *pos);
unsigned char *get_offset_by_entry_name(char *name);

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

// 512 bytes
struct superblock
{
	uint8_t reserved0[11];		   // reserved for compatibility (boot code)
	uint8_t reserved1[21];		   // reserved for compatibility (legacy BIOS parameter block)
	uint8_t reserved2[372];		   // reserved for compatibility (boot code)
	uint64_t last_alteration_time; // timestamp
	uint64_t data_area_size;	   // size of data area in blocks
	uint64_t index_area_size;	  // size of index area in bytes
	uint8_t magic[3];			   // magic number (0x534653 ['SFS'])
	uint8_t version;			   // SFS version number (0x10 = 1.0)
	uint64_t total_blocks;		   // total number of blocks
	uint32_t reserved_blocks;	  // number of reserved blocks (including the superblock)
	uint8_t block_size;			   // block size
	uint8_t checksum;			   // checksum
	uint8_t reserved3[64];		   // reserved for compatibility (partition table)
	uint16_t reserved4;			   // reserved for compatibility (boot signature)
} __attribute__((packed));

// Directory entry - specifies that the entry is a directory
struct directory_entry
{
	uint8_t type;			  // entry type (0x11 for directory entry)
	uint8_t num_cont_entries; // number of continuation entries following this entry
	uint64_t timestamp;		  // timestamp
	uint8_t name[54];		  // directory name
} __attribute__((packed));

// File entry - specifies that the entry is a file
struct file_entry
{
	uint8_t type;			  // entry type (0x12 for file entry)
	uint8_t num_cont_entries; // number of continuation entries following this entry
	uint64_t timestamp;		  // timestamp
	uint64_t start_block;	 // starting block number in the data area
	uint64_t end_block;		  // ending block number in the data area
	uint64_t size;			  // file size in bytes
	uint8_t filename[30];	 // filename in UTF-8
} __attribute__((packed));

// Unusable entry
struct unusable_entry
{
	uint8_t type;		   // entry type (0x18 for unusable)
	uint8_t reserved0[9];  // unused
	uint64_t start_block;  // the starting block number in the data area
	uint64_t end_block;	// the ending block number in the data area
	uint8_t reserved1[38]; // unused
};

// Deleted directory entry - specifies that the current entry was a directory, but has been deleted
struct deleted_directory_entry
{
	uint8_t type;			  // entry type (0x19 for deleted directory)
	uint8_t num_cont_entries; // number of continuation entries following this entry
	uint64_t timestamp;		  // the time when the directory was deleted
	uint8_t reserved[54];	 // unused
} __attribute__((packed));

// Deleted file entry - specifies that the current entry was a file, but has been deleted
struct deleted_file_entry
{
	uint8_t type;			  // entry type (0x1A for deleted file)
	uint8_t num_cont_entries; // number of continuation entries following this entry
	uint64_t timestamp;		  // the time when the file was deleted
	uint64_t start_block;	 // the starting block number in the data area
	uint64_t end_block;		  // the ending block number in the data area
	uint64_t size;			  // the length of the deleted file
	uint8_t filename[30];	 // the name of the deleted file
} __attribute__((packed));

// Continuation entry - used when a filename or directory name is too long
// to fit into one file or directory entry
struct continuation_entry
{
	uint8_t name[64]; // continued name of the file or directory name that was too long
} __attribute__((packed));

// Unused entry
struct unused_entry
{
	uint8_t type;		  // entry type (0x010 for unused)
	uint8_t reserved[63]; // unused
} __attribute__((packed));

// Starting marker entry - used for making the start of the index area
struct starting_marker_entry
{
	uint8_t type;		  // entry type (0x02 for starting marker - marks start of index area)
	uint8_t reserved[63]; // unused
} __attribute__((packed));

// Volume identifier entry - used to identify the end of the volume
struct volume_identifier_entry
{
	uint8_t type;			 // entry type (0x01 for volume identifier)
	uint8_t reserved[3];	 // reserved
	uint64_t timestamp;		 // timestamp
	uint8_t volume_name[52]; // volume name in UTF-8, null terminated
} __attribute__((packed));

// The superblock structure used for determining information about the SFS (rootfs) filesystem
struct superblock rootfs_superblock;
unsigned int total_rootfs_entries;

// Holds any type of entry and its associated entry type value
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

#endif