#include <stdbool.h>

#include "../../libc/include/_null.h"
#include "../../libc/include/size_t.h"
#include "../../libc/include/stdint.h"
#include "../../libc/include/stdio/stdio.h"
#include "../errors/errors.h"
#include "fs.h"
#include "sfs.h"

// Initialize the superblock by filling its contents with the 512 bytes from rootfs_start
void init_sfs()
{
    /*
     * Copy over the first 512 bytes if memory from `rootfs_start` into the superblock structure
     * Copy the 
     */

    // Fill the reserved areas with 0's
    memset(rootfs_superblock.reserved0, 0, sizeof(rootfs_superblock.reserved0));
    memset(rootfs_superblock.reserved1, 0, sizeof(rootfs_superblock.reserved1));
    memset(rootfs_superblock.reserved2, 0, sizeof(rootfs_superblock.reserved2));
    
    // Should be using offsetof(), but sizeof(struct superblock) is incorrect
    memcpy(&rootfs_superblock.last_alteration_time, rootfs_start+0x194, 8);
    memcpy(&rootfs_superblock.data_area_size, rootfs_start+0x19C, 8);
    memcpy(&rootfs_superblock.index_area_size, rootfs_start+0x1A4, 8);

    // Copy the magic into the superblock structure
    strncpy((char*) rootfs_superblock.magic, (char*) rootfs_start+0x1AC, 3);

    memcpy(&rootfs_superblock.version, rootfs_start+0x1AF, 8);
    memcpy(&rootfs_superblock.total_blocks, rootfs_start+0x1B0, 8);
    memcpy(&rootfs_superblock.reserved_blocks, rootfs_start+0x1B8, 8);
    memcpy(&rootfs_superblock.block_size, rootfs_start+0x1BC, 8);
    memcpy(&rootfs_superblock.checksum, rootfs_start+0x1BD, 8);

    // Fill the reserved areas with 0's
    memset(&rootfs_superblock.reserved3, 0, sizeof(rootfs_superblock.reserved3));
    memset(&rootfs_superblock.reserved4, 0, sizeof(rootfs_superblock.reserved4));

    // Determine the start of the index area
    index_start = rootfs_start + (1 * BYTES_PER_BLOCKS)
                + (rootfs_superblock.data_area_size * BYTES_PER_BLOCKS);
    
    // Determine the total number of entries in the filesystem
    total_entries = (rootfs_superblock.index_area_size / ENTRY_SIZE);
    
    // Count all files and folders in rootfs filesystem
    count_files_and_dirs();
}

/*
 * Take in a filename and the mode (read, write, append) and return a FILE* type.
 */
FILE* open(const char* filename /* , const char* mode */) // TODO: check mode and existence of file/folder
{    
    struct entry entries[total_entries];
    char** entry_names = kmalloc(total_entries);

    // Get a list of the names of all the entries in the filesystem
    get_entry_names(entry_names, entries);

    bool entry_found = false;
    int cur_entry_index = 0;

    /* Loop through the list of entry names and set `entry_found` to true
     * if the requested filename matched the entry name.
     * Only allow the opening of files (no directories or deleted entries)
     */
    for (unsigned int i=0; i<total_entries; i++) {
        if (strcmp(entry_names[i], filename) == 0 && entries[i].type == FILE_ENTRY) {
            entry_found = true;
            cur_entry_index = i;
        }
    }

    kfree(entry_names); // Deallocate the memory allocated for the array of entry names

    // Create a FILE pointer and return it if an entry was found matching `filename`
    if (entry_found) {
        FILE* fp = kmalloc(sizeof(FILE));

        // Check if memory was allocated for the FILE pointer
        if (fp == NULL) {
            set_error(MEMORY_ALLOC_FAILED);
            return NULL;
        }

        /*
         * Populate the FILE structure with the necessary information.
         * The position, eof and flags will be set in the call to fopen()
         */
        
        // Check if there are already the maximum number of opened files
        if (current_file_descriptor == MAX_OPEN_FILES)
            return NULL;
        
        // The file entry containing the information about the file
        struct file_entry file = entries[cur_entry_index].entry.file_ent;

        fp->fd = current_file_descriptor++;
        fp->size = file.size;
        fp->start_block = file.start_block;
        fp->offset = index_start + (cur_entry_index * ENTRY_SIZE);

        return fp;
    }
    
    // An entry was not found matching `filename`
    return NULL;
}

// Close the file
// TODO: implement
void close()
{
}

extern void dump_memory(void* ptr, size_t size);

// Read from a file
int read(FILE* stream, char* buf, size_t len)
{
    unsigned int i = 0; // The number of bytes read from the file. Also used as an index into `buf`
    unsigned int num_bytes_to_read = 0;

    // Check if we have reached the end of the file
    if (stream->eof == 0) {
        // Obtain a pointer to the beginning of the block where the file's contents are stored
        unsigned char* pos = rootfs_start + (stream->start_block * BYTES_PER_BLOCKS) + stream->position;

        /* Determine the number of bytes to read from the file.
         * If the file's size - cursor position - `len` < 0, then the user requested
         * to read more bytes than there are in the file
         */
        if ((int64_t) (stream->size - stream->position) - (int64_t) len >= 0) // size=87; pos=10; len=1 // size=87; pos=7; len=100
            num_bytes_to_read = len;
        else
            num_bytes_to_read = (stream->size - stream->position);

        // Ensure that we do not read past the end of the file
        while (i < num_bytes_to_read) {
            buf[i++] = *pos++;
        }
        
        // Return the number of bytes read
        return i;
    } else {
        return EOF;
    }
}

// Write to a file
// TODO: implement
void write()
{
}

// Open a directory for reading
// TODO: implement
void opendir()
{
}

// Read a directory and return its subdirectories
// TODO: implement
void readdir()
{
}

// Mount the filesystem
// TODO: implement
void mount()
{
    printf("Mounting SFS filesystem as rootfs...");
}

// Unmount the filesystem
// TODO: implement
void umount()
{
    printf("Unmounting SFS filesystem...");
}

// Given an offset to the start of an entry, find and return the entry's name
char* get_entry_name_by_offset(char* pos)
{
    /* Check if the position contains a valid entry
     * (don't include starting marker and volume identifier entries)
     */
    if (*pos == UNUSED_ENTRY || *pos == DIRECTORY_ENTRY || *pos == FILE_ENTRY || 
        *pos == UNUSABLE_ENTRY || *pos == DELETED_DIRECTORY_ENTRY || *pos == DELETED_FILE_ENTRY) {
        unsigned int num_cont_entries = *(pos+1);
        unsigned int buffer_size = 0;

        // Set the intial buffer size before checking the number of continuation entries
        if (*pos == DIRECTORY_ENTRY || *pos == DELETED_DIRECTORY_ENTRY) {
            buffer_size = DIRECTORY_NAME_ENTRY_LEN;
        } else if (*pos == FILE_ENTRY || *pos == DELETED_FILE_ENTRY) {
            buffer_size = FILENAME_ENTRY_LEN;
        }

        // Add to the length of the filename by adding the length of all the entry's continuation entries
        buffer_size += (num_cont_entries * ENTRY_SIZE) + 1;

        char filename[buffer_size];

        // Copy the filename in the entry into the actual filename before any continuation entries
        if (*pos == DIRECTORY_ENTRY || *pos == DELETED_DIRECTORY_ENTRY) {
            strncpy(filename, (pos+DIRECTORY_NAME_OFFSET), DIRECTORY_NAME_ENTRY_LEN);
        } else if (*pos == FILE_ENTRY || *pos == DELETED_FILE_ENTRY) {
            strncpy(filename, (pos+FILENAME_OFFSET), FILENAME_ENTRY_LEN);
        }

        // Advance to the continuation entry, if there is one
        if (num_cont_entries > 0) {
            pos += ENTRY_SIZE;
        }

        // Append the filename from the continuation entries to the actual filename
        for (unsigned int i=0; i<num_cont_entries; i++) {
            char cont_name[ENTRY_SIZE+2];
                        
            strncpy(cont_name, pos, ENTRY_SIZE-1);
            //cont_name[ENTRY_SIZE] = '\0';
            
            strcat(filename, cont_name);
            
            // Advance to the next entry
            pos += ENTRY_SIZE;
        }
    }

    return NULL;
}

// Populate an array of entry structs with a list of entries found in the filesystem
void find_all_entries(struct entry entries[])
{
    unsigned char* pos = rootfs_start + (1 * BYTES_PER_BLOCKS)
                + (rootfs_superblock.data_area_size * BYTES_PER_BLOCKS); // Pointer to the index area so we can search for entries
    unsigned int cur_entry = 0; // Used to keep track of the next index of the struct in the array

    if (*pos == STARTING_MARKER_ENTRY) {
        pos += ENTRY_SIZE; // Skip over the starting marker entry

        uint8_t entry_type;
        uint8_t num_cont_entries;
        uint8_t entry_name_len;
        uint8_t entry_name_offset;
        uint64_t timestamp;

        while (cur_entry < (total_entries)) { // total_entries-1
            // Determine the type of the entry and the number of continuation entries that follow it
            entry_type = *pos;
            num_cont_entries = *(pos+1);
            timestamp = *(pos+2);

            // Determine the type of entry and set the length of the entry's name (for files and directories only)
            switch (entry_type) {
                case DIRECTORY_ENTRY:
                    entry_name_len = DIRECTORY_NAME_ENTRY_LEN;
                    entry_name_offset = DIRECTORY_NAME_OFFSET;
                    break;
                case FILE_ENTRY:
                    entry_name_len = FILENAME_ENTRY_LEN;
                    entry_name_offset = FILENAME_OFFSET;
                    break;
            }
            
            // Store the name of the entry (for directories and files)
            char name[entry_name_len];

            strncpy(name, (const char*) pos+entry_name_offset, sizeof(name));
            name[entry_name_len] = '\0';

            // Check if the entry is a directory entry
            if (entry_type == DIRECTORY_ENTRY) {
                struct directory_entry dir_ent;

                dir_ent.type = DIRECTORY_ENTRY;
                dir_ent.num_cont_entries = num_cont_entries;
                dir_ent.timestamp = timestamp;
                
                strncpy((char*) dir_ent.name, (const char*) name, sizeof(name));

                // Add the directory entry to the list of entries
                entries[cur_entry].type = DIRECTORY_ENTRY;
                entries[cur_entry].entry.dir_ent = dir_ent;
            } else if (entry_type == FILE_ENTRY) {
                struct file_entry file_ent;

                file_ent.type = FILE_ENTRY;
                file_ent.num_cont_entries = num_cont_entries;
                file_ent.timestamp = timestamp;
                file_ent.start_block = *(pos+10);
                file_ent.end_block = *(pos+18);
                file_ent.size = *(pos+26);

                strncpy((char*) file_ent.filename, (const char*) name, sizeof(name));

                // Add the file entry to the list of entries
                entries[cur_entry].type = FILE_ENTRY;
                entries[cur_entry].entry.file_ent = file_ent;

                // printf("__start_block=%d     ", entries[cur_entry].entry.file_ent.start_block);
            }

            cur_entry++;
            pos += ENTRY_SIZE;

            // If there are continuation entries, create a new struct for each one and add it to the array
            if (num_cont_entries > 0) {
                for (uint8_t i=0; i<num_cont_entries; i++) {
                    // Create a new continuation entry struct and copy over the name into it
                    struct continuation_entry cont_entry;

                    strncpy((char*) cont_entry.name, (const char*) pos, ENTRY_SIZE-1);
                    cont_entry.name[ENTRY_SIZE] = '\0';

                    memcpy(&entries[cur_entry].entry.cont_entry, &cont_entry, sizeof(cont_entry));
                    cur_entry++;
                    
                    // Advance to the next entry
                    pos += ENTRY_SIZE;
                }
            }
        }
    }
}

// Get a list of names of all the entries in the filesystemm
void get_entry_names(char** entry_names, struct entry entries[])
{
    int i = 0; // Keep track of the name of the current entry to add to the array

    // Get a list of all the filesystem entries
    find_all_entries(entries);

    char* name = ""; // The name of the file or directory
    struct entry cur_entry;
    uint8_t entry_type;

    for (unsigned int j=0; j<total_entries; j++) {
        uint8_t num_cont_entries = 0;
        uint8_t entry_name_len = 0;
        
        cur_entry = entries[j];
        entry_type = cur_entry.type;

        /*
         * Determine the number of continuation entries for the current entry.
         * Determine the length of the entry's name (applicable to files and folders only)
         */
        if (entry_type == DIRECTORY_ENTRY) {
            num_cont_entries = cur_entry.entry.dir_ent.num_cont_entries;
            entry_name_len = DIRECTORY_NAME_ENTRY_LEN;
        } else if (entry_type == FILE_ENTRY) {
            num_cont_entries = cur_entry.entry.file_ent.num_cont_entries;
            entry_name_len = FILENAME_ENTRY_LEN;
        }

        int name_len = entry_name_len + (num_cont_entries * ENTRY_SIZE);
        // Store the name of the entry (files and folders only)
        name = (char*) kcalloc(name_len, 1);

        // Set an error and return NULL if we could not allocate enough memory for the file or folder name
        if (name == NULL) {
            set_error(MEMORY_ALLOC_FAILED);
            return;
        }

        // Copy the entry name in to the `name` pointer
        if (entry_type == DIRECTORY_ENTRY) {
            strncpy(name, (const char*) cur_entry.entry.dir_ent.name, sizeof(cur_entry.entry.dir_ent.name));
        } else if (entry_type == FILE_ENTRY) {
            strncpy(name, (const char*) cur_entry.entry.file_ent.filename, sizeof(cur_entry.entry.dir_ent.name));
        }

        name[name_len] = '\0'; // Add the null byte to the end of the string

        /*
         * If there are continuation entries following the current entry,
         * add the name of the continuation entry onto the end of
         * the current entry's name
         */
        if (num_cont_entries > 0) {
            for (uint8_t k=0; k<num_cont_entries; k++) {
                char cont_name[ENTRY_SIZE];
                memset(cont_name, 0, sizeof(cont_name));
                        
                strncpy((char*) cont_name,
                        (const char*) entries[j+k+1].entry.cont_entry.name, ENTRY_SIZE-1);
                cont_name[ENTRY_SIZE] = '\0';
                
                strncat(name, cont_name, sizeof(cont_name));

                // Skip over the continuation entries, since they were not added
                // to the array of entry names, but only concatenated on
                i++;
            }
        }

        // Add the name of the entry to the array
        entry_names[i++] = name;

        // Skip over all the continuation entries to avoid reading them again.
        j += (num_cont_entries);
    }
}

// Get a count of all the files and directories currently in the filesystem
void count_files_and_dirs()
{
    struct entry entries[total_entries];

    // Get a list of the names of all the entries in the filesystem
    find_all_entries(entries);

    // Loop through all the entries in the filesystem and determine if each entry is a file or folder
    for (unsigned int i=0; i<total_entries; i++) {
        switch (entries[i].type) {
            case DIRECTORY_ENTRY:
                total_rootfs_dirs++;
                break;
            case FILE_ENTRY:
                total_rootfs_files++;
                break;
        }
    }
}