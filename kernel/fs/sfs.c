#include <stdbool.h>

#include "../../libc/include/_null.h"
#include "../../libc/include/size_t.h"
#include "fs.h"
#include "sfs.h"

extern int strcmp(const char* str1, const char* str2);
extern int strncmp(const char* str1, const char* str2, int n);
extern void* memcpy(void* dest, const void* src, size_t count);
extern void dump_memory(void* ptr, int n);

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

    // Copy the magicc
    strncpy((char*) rootfs_superblock.magic, rootfs_start+0x1AC, 3);

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
}

/*
 * Take in a filename and the mode (read, write, append) and return a FILE* type.
 */
FILE* open(const char* filename)
{
    FILE* fp = find_entry_by_name(filename);

    // ### temporary! ###
    /* 
     * Calculate the total number of entries which will be stored in the array.
     * Exclude the starting marker and volume identifier entries
     */
    int total_entries = (rootfs_superblock.index_area_size / ENTRY_SIZE);
    struct entry entries[total_entries];

    find_all_entries(entries);

    for (unsigned int i=0; i<4; i++) {
        //printf("name=%s\n", entries[i].entry.dir_ent.name);
    }

    /* 
     * Only set the file's attributes if a FILE was returned.
     * Return NULL if there are the maximum number of files open, or set the file's descriptor
     */
    if (fp != NULL) {
        if (current_file_descriptor == MAX_OPEN_FILES)
            return NULL;
        else
            fp->fd = current_file_descriptor++;
    }
    
    return fp;
}

// Close the file
// TODO: implement
void close()
{
    
}

// Read from a file
// TODO: implement
void read()
{

}

// Write to a file
// TODO: implement
void write()
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
    printf("Unmounting SFS filesystem as rootfs...");
}

// Given a filename, search through each entry in the index area for the entry matching the name.
// Return a FILE* object, or NULL if no entry was found matching that name
FILE* find_entry_by_name(const char* entry_name)
{
    // Calculate beginning of index area. Superblock + size of data area
    char* pos = rootfs_start + (1 * BYTES_PER_BLOCKS)
                + (rootfs_superblock.data_area_size * BYTES_PER_BLOCKS);

    unsigned int current_entry = 0;
    bool entry_found = false;
    int entry_type = -1; // Indicates no entry
    unsigned int num_cont_entries;

    // Hold the file and directory entry information for a file/directory that matched the entry name
    FILE* fp = kmalloc(sizeof(FILE));

    // The start of the index area is correctly marked with a starting marker entry
    if (*pos == STARTING_MARKER_ENTRY) {
        pos += ENTRY_SIZE; // Skip over the starting marker entry
        current_entry++; // Increment for starting marker entry

        // Controls which entry we are on and exits the loop if we have reached the end of the index area
        unsigned int total_index_entries = rootfs_superblock.index_area_size / ENTRY_SIZE;

        while (current_entry < total_index_entries) {
            // Indicate what type of entry was encountered
            entry_type = *pos;

            // Is the entry a directory entry or a file entry?
            if (*pos == DIRECTORY_ENTRY || *pos == FILE_ENTRY) {
                num_cont_entries = *(pos+1); // Number of continuation entries for the directory

                // The name of the directory
                char* name = kcalloc((num_cont_entries * ENTRY_SIZE), 1); // TODO: change hardcoded values

                // Copy the specified number of characters, depending on what type of entry it is
                if (*pos == DIRECTORY_ENTRY) {
                    strncpy(name, pos+DIRECTORY_NAME_OFFSET, DIRECTORY_NAME_ENTRY_LEN);
                } else if (*pos == FILE_ENTRY) {
                    strncpy(name, pos+FILENAME_OFFSET, FILENAME_ENTRY_LEN);
                }
                
                // Advance to the next entry
                pos += ENTRY_SIZE;

                // Add on the rest of the directory/file name if it was too many characters to fit in one entry
                if (num_cont_entries > 0) {
                    for (unsigned int i=0; i<num_cont_entries; i++) {
                        char cont_name[ENTRY_SIZE+2];
                        
                        strncpy(cont_name, pos, ENTRY_SIZE-1);
                        cont_name[ENTRY_SIZE] = '\0';
                        
                        strcat(name, cont_name);
                        
                        // Advance to the next entry
                        pos += ENTRY_SIZE;
                    }
                }

                // Check if the current entry's name is the name of the entry that was passed to the function
                if (strcmp(entry_name, name) == 0) {
                    // An entry was found, so we can break out of loop, and return a FILE structure
                    entry_found = true;
                    kfree(name);

                    break;
                }
            }

            current_entry++;
        }
    } else {
        set_error("Failed to find starting marker entry for SFS index area!");
    }

    // Did we find an entry matching the entry name passed in?
    if (entry_found) {
        // TODO: check if a file is already open in the list of open files table!
        // 
        //


        // Create a directory entry
        if (entry_type == FILE_ENTRY) {
            /* 
             * Copy over the directory information into a structure and return it
             */
            memcpy(&fp->size, pos-ENTRY_SIZE-(num_cont_entries*ENTRY_SIZE)+26, sizeof(uint64_t));
            memcpy(&fp->start_block, pos-ENTRY_SIZE-(num_cont_entries*ENTRY_SIZE)+10, sizeof(uint64_t));

            fp->eof = (fp->size == 0) ? 1 : 0;
            fp->offset = pos-64;

            return fp;
        } else if (entry_type == DIRECTORY_ENTRY) {
            /* 
             * Copy over the directory information into a structure and return it
             */

            fp->size = 0;
            fp->eof = 1;
            fp->position = 0;
            fp->start_block = 0;
            fp->offset = pos-ENTRY_SIZE-(num_cont_entries*ENTRY_SIZE);

            return fp;
        } else {
            /* 
             * The entry was not a directory or a file, so return nothing
             */
            return NULL;
        }
    } else {
        return NULL;
    }
}

// Given an offset to the start of an entry, find and return the entry's name
char* get_entry_name_by_offset(char* pos)
{
    /* Check if the position contains a valid entry
     * (don't include starting marker and volume identifier entries)
     */
    if (*pos == UNUSED_ENTRY || * pos == DIRECTORY_ENTRY || *pos == FILE_ENTRY || 
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
    char* pos = rootfs_start + (1 * BYTES_PER_BLOCKS)
                + (rootfs_superblock.data_area_size * BYTES_PER_BLOCKS); // Pointer to the index area so we can search for entries
    int cur_entry = 0; // Used to keep track of the next index of the struct in the array
    int num_cont_entries;

    if (*pos == STARTING_MARKER_ENTRY) {
        pos += ENTRY_SIZE; // Skip over the starting marker entry

        // Total entries in the filesystem
        int total_entries = (rootfs_superblock.index_area_size / ENTRY_SIZE);

        while (cur_entry < total_entries) {
            // Check if the entry is a directory entry
            if (*pos == DIRECTORY_ENTRY) {
                num_cont_entries = *(pos+1); // Number of continuation entries for the directory

                // dump_memory(pos, 32);
                
                char name[8];
                name[0] = *(pos+DIRECTORY_NAME_OFFSET-1);
                name[1] = *(pos+DIRECTORY_NAME_OFFSET);
                name[2] = *(pos+DIRECTORY_NAME_OFFSET+1);
                name[3] = *(pos+DIRECTORY_NAME_OFFSET+2);
                name[4] = '\0';

                printf("num cont entries=0x%x\n", *(pos+1));
                printf("name[0]=%c\n", name[0]);
                printf("c=0x%x\n", *(pos+DIRECTORY_NAME_OFFSET-1));
                break;

                struct directory_entry dir_ent;

                // Store the directory's information in a struct
                dir_ent.type = DIRECTORY_ENTRY;
                dir_ent.num_cont_entries = *(pos+1);
                dir_ent.timestamp = *(pos+2);
                strncpy((char*) dir_ent.name, (const char*) (pos+DIRECTORY_NAME_OFFSET-1),
                        DIRECTORY_NAME_ENTRY_LEN);

                // Add the directory entry struct to the array of structs
                entries[cur_entry].type = DIRECTORY_ENTRY;
                entries[cur_entry].entry.dir_ent = dir_ent;

                cur_entry++;

                // If there are continuation entries, create a new struct for each one and add it to the array
                if (num_cont_entries > 0) {
                    for (int i=0; i<num_cont_entries; i++) {
                        // Create a new continuation entry struct and copy over the name into it
                        struct continuation_entry cont_entry;

                        strncpy((char*) cont_entry.name, pos, ENTRY_SIZE-1);
                        cont_entry.name[ENTRY_SIZE] = '\0';

                        memcpy(&entries[cur_entry], &cont_entry, sizeof(cont_entry));
                        cur_entry++;
                        
                        // Advance to the next entry
                        pos += ENTRY_SIZE;
                    }

                    cur_entry++;
                }
            } else {
                cur_entry++;
            }
        }
    }
}