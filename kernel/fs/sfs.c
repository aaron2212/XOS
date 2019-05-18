#include <stdbool.h>

#include "../../libc/include/_null.h"
#include "../../libc/include/math/math.h"
#include "../../libc/include/size_t.h"
#include "../../libc/include/stdint.h"
#include "../../libc/include/stdio/stdio.h"
#include "../errors/errors.h"
#include "fs.h"
#include "sfs.h"

extern void dump_memory(void* ptr, size_t size);

// Initialize the superblock by filling its contents with the 512 bytes from rootfs_start
void init() {
    /*
     * Copy over the first 512 bytes if memory from `rootfs_start` into the superblock structure
     * Copy the 
     */

    // Fill the reserved areas with 0's
    memset(rootfs_superblock.reserved0, 0, sizeof(rootfs_superblock.reserved0));
    memset(rootfs_superblock.reserved1, 0, sizeof(rootfs_superblock.reserved1));
    memset(rootfs_superblock.reserved2, 0, sizeof(rootfs_superblock.reserved2));

    // Should be using offsetof(), but sizeof(struct superblock) is incorrect
    memcpy(&rootfs_superblock.last_alteration_time, rootfs_start + 0x194, 8);
    memcpy(&rootfs_superblock.data_area_size, rootfs_start + 0x19C, 8);
    memcpy(&rootfs_superblock.index_area_size, rootfs_start + 0x1A4, 8);

    // Copy the magic into the superblock structure
    strncpy((char*)rootfs_superblock.magic, (char*)rootfs_start + 0x1AC, 3);

    memcpy(&rootfs_superblock.version, rootfs_start + 0x1AF, 8);
    memcpy(&rootfs_superblock.total_blocks, rootfs_start + 0x1B0, 8);
    memcpy(&rootfs_superblock.reserved_blocks, rootfs_start + 0x1B8, 8);
    memcpy(&rootfs_superblock.block_size, rootfs_start + 0x1BC, 8);
    memcpy(&rootfs_superblock.checksum, rootfs_start + 0x1BD, 8);

    // Fill the reserved areas with 0's
    memset(&rootfs_superblock.reserved3, 0, sizeof(rootfs_superblock.reserved3));
    memset(&rootfs_superblock.reserved4, 0, sizeof(rootfs_superblock.reserved4));

    // Determine the start of the index area
    index_start = rootfs_start + (1 * BYTES_PER_BLOCKS) + (rootfs_superblock.data_area_size * BYTES_PER_BLOCKS);

    // Determine the total number of entries in the filesystem
    total_rootfs_entries = (rootfs_superblock.index_area_size / ENTRY_SIZE);

    // Count all files and folders in rootfs filesystem
    count_files_and_dirs();
}

/*
 * Take in a filename and the mode (read, write, append) and return a FILE* type.
 */
FILE* open(char* filename /* , const char* mode */) { // TODO: check mode and existence of file/folder
    // Get a list of every file with its filename in the filesystem
    struct entry entries[total_rootfs_entries];
    char** entry_names = kmalloc(total_rootfs_files);
    get_entry_names(entry_names, entries, FILE_ENTRY);

    bool entry_found = false;
    unsigned int cur_entry_index = 0; // The index of the found entry in the list of entries (used for the offset)

    // The file entry containing the information about the file, if the file was found
    struct file_entry file_ent;

    /* Loop through the list of entries and set `entry_found` to true
     * if the requested filename matched the entry name.
     * Only allow the opening of files (no directories or deleted entries)
     */
    for (unsigned int i = 0; i < (total_rootfs_entries); i++) {
        // Check if the current entry is a file entry, so we can compare the passed in filename with the entry's filename
        if (entries[i].type == FILE_ENTRY) {
            if (strcmp((const char*)entries[i].entry.file_ent.filename, filename) == 0) {
                file_ent = entries[i].entry.file_ent;
                entry_found = true;
                cur_entry_index = i;
            }
        }
    }

    kfree(entry_names); // Deallocate the memory allocated for the array of entry names

    // Create a FILE pointer and return it if an entry was found matching `filename`
    if (entry_found) {
        FILE* stream = kmalloc(sizeof(FILE));

        // Check if memory was allocated for the FILE pointer
        if (stream == NULL) {
            // set_error(MEMORY_ALLOC_FAILED);
            return NULL;
        }

        /*
         * Populate the FILE structure with the necessary information.
         * The position, eof and flags will be set in the call to fopen()
         */

        // Check if there are already the maximum number of opened files
        if (current_file_descriptor == MAX_OPEN_FILES)
            return NULL;

        stream->size = file_ent.size;
        stream->fd = current_file_descriptor++;
        stream->start_block = file_ent.start_block;
        stream->offset = index_start + ((cur_entry_index + 1) * ENTRY_SIZE);

        return stream;
    }

    // An entry was not found matching `filename`
    return NULL;
}

// Read from a file
int read(FILE* stream, char* buf, size_t len) {
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
        if ((int64_t)(stream->size - stream->position) - (int64_t)len >= 0) // size=87; pos=10; len=1 // size=87; pos=7; len=100
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

// TODO: check if path is absolute or relative
// TODO: check if file already exists
// Create a new empty file on the filesystem with the name `filename`
FILE* create(char* filename) {
    // Check if the filename of the file to be created is valid. Replace NBSP characters with a space
    if (!is_filename_valid(filename)) {
        set_fs_error(INVALID_FILENAME);
        return NULL;
    }

    make_full_path(filename);

    // Get a list of all the files and their filenames in the filesystem
    struct entry entries[total_rootfs_entries];
    char** entry_names = kmalloc(total_rootfs_files);
    get_entry_names(entry_names, entries, FILE_ENTRY);

    for (unsigned int i = 0; i < (total_rootfs_dirs + total_rootfs_files); i++) {
        if (strcmp(entry_names[i], filename) == 0)
            return NULL;
    }

    uint64_t num_cont_entries = 0; // Has to match struct file_entry uint64_t

    /*
     * Calculate the number of continuation entries needed to store the filename.
     * Subtract the length of the filename which will be stored in a file entry,
     * and then divide that by the size of an entry and round it up
     */
    if (strlen(filename) > FILENAME_ENTRY_LEN - 1) {
        num_cont_entries = ceil((strlen(filename) - FILENAME_ENTRY_LEN - 1) / ENTRY_SIZE);
    }

    // Create the file entry and set its information (it does not yet occupy any space in the data area)
    struct file_entry file_ent;

    file_ent.type = FILE_ENTRY;
    file_ent.num_cont_entries = num_cont_entries;
    file_ent.timestamp = 0; // TODO: change in future when time.h is working
    file_ent.start_block = 0;
    file_ent.end_block = 0;
    file_ent.size = 0;

    // A list of continuation entries containing the continued filename
    struct continuation_entry cont_entries[num_cont_entries];

    // The position in the string to starting reading from into the continuation entry name
    unsigned int pos = FILENAME_ENTRY_LEN - 1;

    // Create a list of continuation entries that contain the filename's continued name
    if (num_cont_entries > 0) {
        // Copy the first part of the filename into the file entry structure
        memcpy((char*)&file_ent.filename, filename, FILENAME_ENTRY_LEN - 1);

        for (unsigned int i = 0; i < num_cont_entries; i++) {
            // Create a new continuation entry struct and copy over the name into it
            struct continuation_entry cont_entry;

            strncpy((char*)cont_entry.name, &filename[pos], ENTRY_SIZE);
            cont_entry.name[ENTRY_SIZE] = '\0';

            memcpy(&cont_entries[i].name, &cont_entry.name, sizeof(cont_entry.name));

            pos += ENTRY_SIZE + 1;
        }
    } else {
        // Copy the first FILENAME_ENTRY_LEN bytes of the filename into the file entry structure
        strcpy((char*)file_ent.filename, filename);
    }

    // Ensure there is enough memory to add the file entry, continuation entries and volume identifier entry
    if ((unsigned int)rootfs_end + ENTRY_SIZE + (num_cont_entries * ENTRY_SIZE) <= available_memory) {
        unsigned char* pos = rootfs_end - ENTRY_SIZE;

        // Overwrite the volume identifier entry by copying the file entry in its place
        memcpy(pos, &file_ent, ENTRY_SIZE);

        pos += ENTRY_SIZE;

        // Copy over the continuation entries, if any, into memory
        if (num_cont_entries > 0) {
            for (unsigned int i = 0; i < num_cont_entries; i++) {
                memcpy(pos, &cont_entries[i], ENTRY_SIZE);

                pos += ENTRY_SIZE; // Advance to the next position to add the entry to
            }
        }

        // Create a volume identifier entry to add after the newly created entries
        struct volume_identifier_entry vol_entry;

        vol_entry.type = VOLUME_IDENTIFIER_ENTRY;
        memset(&vol_entry.reserved, 0, sizeof(vol_entry.reserved));
        vol_entry.timestamp = 0; // TODO: change in future when time.h is working
        strcpy((char*)vol_entry.volume_name, VOLUME_NAME);

        // Add volume identifier entry to the end
        memcpy(pos, &vol_entry, sizeof(vol_entry));

        // Change `rootfs_end` to point to the end of the last entry created.
        // file entry + continuation entries + volume identifier entry
        rootfs_end = pos + ENTRY_SIZE;

        // Recalculate the new size of the index area and total number of blocks set in the superblock
        rootfs_superblock.index_area_size += ENTRY_SIZE + (num_cont_entries * ENTRY_SIZE);
        rootfs_superblock.total_blocks = 1 + rootfs_superblock.data_area_size + ceil(rootfs_superblock.index_area_size / BYTES_PER_BLOCKS);
    } else {
        set_error("Not enough memory");
    }

    // Increment the total number of entries (count) in the filesystem
    total_rootfs_entries++;
    total_rootfs_files++;

    // Create a FILE stream for the file entry and return it
    FILE* stream = kmalloc(sizeof(FILE));

    // Set the file's attributes. The file's mode will be set in the call to fopen()
    stream->size = 0;
    stream->fd = current_file_descriptor++;
    stream->eof = EOF;
    stream->position = 0;
    stream->start_block = 0;

    // To determine the offset of the file, take the end of the filesystem, go back past the volume
    // identifier entry and back to the beginning of the file entry (2 entries in total)
    stream->offset = rootfs_end - (ENTRY_SIZE * 2);

    return stream;
}

// Write to a file
// TODO: implement
void write() {
}

// Open a directory for reading and return a structure containing information about it
DIR* opendir(char* dirname) {
    DIR* dir = kmalloc(sizeof(DIR));
    dir->entries = kcalloc(sizeof(struct dirent) * (total_rootfs_dirs + total_rootfs_entries), 1);

    // for (unsigned int i = 0; i < total_rootfs_dirs + total_rootfs_files; i++) {
    //     dir->entries[i] = kcalloc(sizeof(struct dirent), 1);
    // }

    // Get a list of every directory with its name in the filesystem
    struct entry entries[total_rootfs_entries];
    char** entry_names = kmalloc(total_rootfs_dirs + total_rootfs_files);
    get_entry_names(entry_names, entries, DIRECTORY_ENTRY | FILE_ENTRY);

    // Replace '.' with the current directory
    if (strlen(dirname) > 0 && dirname[0] == '.')
        strcpy(dirname, current_dir);

    // If `dirname` was the root directory (ie. a '/'), then list all files in the filesystem (depth=1)
    if (strcmp(dirname, PATH_SEPARATOR) == 0) {
        // Holds the entries within the opened directory
        unsigned int total_entries = 0;

        // Get a list of the directory's entries
        for (unsigned int i = 0; i < total_rootfs_dirs + total_rootfs_files; i++) {
            // Strip the first character from the path name (always a '/')
            char* entry_name = kcalloc(strlen(entry_names[i]), 1);
            strcpy(entry_name, entry_names[i] + 1);

            // Only add to the array those entries which are in the root directory
            if (strchr(entry_name, '/') == NULL) {
                // kprintf("%s  ", entry_name);
                struct dirent* dirent = kcalloc(sizeof(struct dirent), 1);

                // Allocate memory to store the name of the entry inside the struct
                dirent->name = kcalloc(strlen(entry_name), 1);

                strncpy((char*)dirent->name, entry_name, strlen(entry_name));
                // memcpy(dir->entries[i], dirent, sizeof(dirent));
                dir->entries[i] = dirent;

                // kprintf("ss=%s\n", dirent->name);
                // kprintf("ss=%s\n", dir->entries[i]->name);

                kfree(dirent);

                total_entries++;
            }
        }

        dir->fd = current_file_descriptor++;

        // Allocate memory for the directory name and store the directory name in the struct
        dir->dirname = kmalloc(strlen(dirname) + 1);
        strcpy(dir->dirname, dirname);
        dir->total_entries = total_entries;
        // strncpy(dir->dirname, dirname, strlen(dirname));
        // dir->offset = get_offset_by_entry_name(dirname);

        kfree(entry_names);

        return dir;
    } else {
        // Remove any trailing forward slash
        if (dirname[strlen(dirname) - 1] == PATH_SEPARATOR_CHAR && strcmp(dirname, PATH_SEPARATOR) != 0)
            dirname[strlen(dirname) - 1] = '\0';

        bool dir_found = false;

        // Try and find a directory in the filesystem matching `dirname`
        for (unsigned int i = 0; i < total_rootfs_dirs; i++) {
            if (strcmp(entry_names[i], dirname) == 0) {
                dir_found = true;
            }
        }

        // If a directory was found, set the DIR file descriptor and return it, or NULL otherwise
        if (dir_found) {
            // Holds the subdirectories within the opened directory
            unsigned int i = 0;

            // Get a list of the directory's subdirectories
            for (i = 0; i < total_rootfs_dirs + total_rootfs_files; i++) {
                // Do not store `dir->dirname` in the list
                if (strcmp(dirname, entry_names[i]) != 0) {
                    if (strncmp(dirname, entry_names[i], strlen(dirname)) == 0) {
                        // Strip away `dirname` from each directory name
                        char* s = kmalloc(strlen(entry_names[i]) - strlen(dirname));
                        memcpy(s, entry_names[i] + strlen(dirname) + 1, strlen(entry_names[i]) - strlen(dirname));

                        // Do not include directory names that include a '/'
                        if (strchr(s, PATH_SEPARATOR_CHAR) == NULL) {
                            struct dirent dirent;
                            strcpy((char*)dirent.name, s);
                            memcpy(&dir->entries[i], &dirent, sizeof(dirent));
                        }

                        kfree(s);
                    }
                }
            }

            dir->fd = current_file_descriptor++;
            dir->total_entries = i;

            // Allocate memory for the directory name and store the directory name in the struct
            dir->dirname = kmalloc(strlen(dirname));
            strncpy(dir->dirname, dirname, strlen(dirname));

            kfree(entry_names);

            return dir;
        } else {
            return NULL;
        }
    }
}

// Read the contents of a directory
struct dirent* readdir(DIR* dir) {
    return dir->entries[dir->current_dir++];

    // return dir->current_dir < dir->total_entries ? dir->entries[dir->current_dir++] : NULL;
}

// Alternative function to read the contents of a directory. Prints out all
// entries without storing them in a DIR struct
bool readdir_alternative(char* dirname) {
    make_full_path(dirname);

    if (strlen(dirname) == 0) {
        strcpy(dirname, PATH_SEPARATOR);
    }

    struct entry entries[total_rootfs_entries];
    char** entry_names = kmalloc(total_rootfs_dirs + total_rootfs_files);
    get_entry_names(entry_names, entries, DIRECTORY_ENTRY | FILE_ENTRY);
    bool _dir_found = false;

    // Replace '.' with the current directory
    if (strlen(dirname) > 0 && dirname[0] == '.')
        strcpy(dirname, current_dir);

    // If `dirname` was the root directory (ie. a '/'), then list all files in the filesystem (depth=1)
    if (strcmp(dirname, PATH_SEPARATOR) == 0) {
        // Get a list of the directory's entries
        for (unsigned int i = 0; i < total_rootfs_dirs + total_rootfs_files; i++) {
            // Strip the first character from the path name (always a '/')
            char* entry_name = kcalloc(strlen(entry_names[i]), 1);
            strcpy(entry_name, entry_names[i] + 1);

            // Only add to the array those entries which are in the root directory
            if (strchr(entry_name, '/') == NULL) {
                kprintf("%s\n", entry_name);
                _dir_found = true;
            }
        }

        return _dir_found;
    } else {
        // Remove any trailing forward slash
        if (dirname[strlen(dirname) - 1] == PATH_SEPARATOR_CHAR && strcmp(dirname, PATH_SEPARATOR) != 0)
            dirname[strlen(dirname) - 1] = '\0';

        bool dir_found = false;

        // Try and find a directory in the filesystem matching `dirname`
        for (unsigned int i = 0; i < total_rootfs_dirs; i++) {
            if (strcmp(entry_names[i], dirname) == 0) {
                dir_found = true;
            }
        }

        // If a directory was found, set the DIR file descriptor and return it, or NULL otherwise
        if (dir_found) {
            // Holds the subdirectories within the opened directory
            unsigned int i = 0;

            // Get a list of the directory's subdirectories
            for (i = 0; i < total_rootfs_dirs + total_rootfs_files; i++) {
                // Do not store `dir->dirname` in the list
                if (strcmp(dirname, entry_names[i]) != 0) {
                    if (strncmp(dirname, entry_names[i], strlen(dirname)) == 0) {
                        // Strip away `dirname` from each directory name
                        char* s = kmalloc(strlen(entry_names[i]) - strlen(dirname));
                        memcpy(s, entry_names[i] + strlen(dirname) + 1, strlen(entry_names[i]) - strlen(dirname));

                        // Do not include directory names that include a '/'
                        if (strchr(s, PATH_SEPARATOR_CHAR) == NULL) {
                            kprintf("%s  ", s);
                        }

                        kfree(s);
                    }
                }
            }

            kfree(entry_names);

            return true;
        }
    }

    return false;
}

// Change the current working directory
int changedir(char* dirname) {
    // Get a list of all the files and their filenames in the filesystem
    struct entry entries[total_rootfs_entries];
    char** entry_names = kmalloc(total_rootfs_files);
    get_entry_names(entry_names, entries, DIRECTORY_ENTRY);

    // Go through all the directories and check if there exists a directory by `dirname`
    // TODO: only allow changing to directories: modify get_entry_names
    for (unsigned int i = 0; i < (total_rootfs_dirs + total_rootfs_files); i++) {
        if (strcmp(entry_names[i], dirname) == 0) {
            strcpy(current_dir, dirname);
            current_dir[strlen(current_dir)] = '\0';

            kfree(entry_names);

            return 0;
        }
    }

    kfree(entry_names);

    // Failed to find a directory by `dirname`
    return -1;
}

// Close a previously opened directory
int closedir(DIR* dir) {

    return vfs_closedir(dir);
}

// Attempt to remove (delete) a single file from the filesystem.
// This is done by setting the entry to a deleted file entry
bool rm(char* filename) {
    // make_full_path(filename);

    unsigned char* offset = get_offset_by_entry_name(filename);

    if (*offset == FILE_ENTRY) {
        *offset = DELETED_FILE_ENTRY;
        return true;
    }

    return false;
}

// Attempt to remove (delete) a directory from the filesystem
bool rmdir(char* dirname) {
    // kprintf("path exists: %d\n", path_exists(dirname));
    // if (!path_exists(dirname)) {
    //     return true;
    // }

    // Get a list of all the files and their filenames in the filesystem
    struct entry entries[total_rootfs_entries];
    char** entry_names = kmalloc(total_rootfs_files + total_rootfs_dirs);

    if (entry_names == NULL) {
        return false;
    }

    get_entry_names(entry_names, entries, DIRECTORY_ENTRY | FILE_ENTRY);

    bool entries_deleted = false;

    // Iterate over each entry in the filesystem, and delete it if it is
    // a sub entry of the directory being deleted
    for (unsigned int i = 0; i < total_rootfs_dirs + total_rootfs_files; i++) {
        if (strncmp(entry_names[i], dirname, strlen(dirname)) == 0) {
            unsigned char* offset = get_offset_by_entry_name(entry_names[i]);

            if (*offset == DIRECTORY_ENTRY) {
                *offset = DELETED_DIRECTORY_ENTRY;
            } else if (*offset == FILE_ENTRY) {
                *offset = DELETED_FILE_ENTRY;
            }

            entries_deleted = true;
        }
    }

    kfree(entry_names);

    return entries_deleted;
}

// Given an offset to the start of an entry, find and return the entry's name
char* get_entry_name_by_offset(char* pos) {
    /* Check if the position contains a valid entry
     * (don't include starting marker and volume identifier entries)
     */
    if (*pos == UNUSED_ENTRY || *pos == DIRECTORY_ENTRY || *pos == FILE_ENTRY ||
        *pos == UNUSABLE_ENTRY || *pos == DELETED_DIRECTORY_ENTRY || *pos == DELETED_FILE_ENTRY) {
        unsigned int num_cont_entries = *(pos + 1);
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
            strncpy(filename, (pos + DIRECTORY_NAME_OFFSET), DIRECTORY_NAME_ENTRY_LEN);
        } else if (*pos == FILE_ENTRY || *pos == DELETED_FILE_ENTRY) {
            strncpy(filename, (pos + FILENAME_OFFSET), FILENAME_ENTRY_LEN);
        }

        // Advance to the continuation entry, if there is one
        if (num_cont_entries > 0) {
            pos += ENTRY_SIZE;
        }

        // Append the filename from the continuation entries to the actual filename
        for (unsigned int i = 0; i < num_cont_entries; i++) {
            char cont_name[ENTRY_SIZE + 2];

            strncpy(cont_name, pos, ENTRY_SIZE - 1);
            strcat(filename, cont_name);

            // Advance to the next entry
            pos += ENTRY_SIZE;
        }
    }

    return NULL;
}

// Return a pointer to the start of the index entry corresponding to `name`
unsigned char* get_offset_by_entry_name(char* name) {
    // Pointer to the index area so we can search for entries.
    // Used to keep track of the next index of the struct in the array
    unsigned char* pos = rootfs_start + (1 * BYTES_PER_BLOCKS) + (rootfs_superblock.data_area_size * BYTES_PER_BLOCKS);

    while (pos < rootfs_end) {
        if (*pos == DIRECTORY_ENTRY || *pos == FILE_ENTRY || *pos == DELETED_FILE_ENTRY) {
            char* full_name = get_full_name(pos);
            if (strcmp(full_name, name) == 0) {
                kfree(full_name);
                return pos;
            }

            pos += *(pos + 1) * ENTRY_SIZE;
        }

        pos += ENTRY_SIZE;
    }

    return NULL;
}

// Get the full name of an entry given its position in the filesystem
char* get_full_name(unsigned char* pos) {
    unsigned int num_cont_entries = *(pos + 1);
    unsigned int name_len = ENTRY_SIZE + (num_cont_entries * ENTRY_SIZE);
    char* name = kcalloc(name_len, 1);

    // Only directory entries, file entries and deleted file entries have names
    if (*pos == DIRECTORY_ENTRY) {
        memcpy(name, (char*)pos + DIRECTORY_NAME_OFFSET, DIRECTORY_NAME_ENTRY_LEN);
    } else if (*pos == FILE_ENTRY || *pos == DELETED_FILE_ENTRY) {
        memcpy(name, (char*)pos + FILENAME_OFFSET, FILENAME_ENTRY_LEN);
    } else {
        return NULL;
    }

    pos += ENTRY_SIZE;

    for (unsigned int i = 0; i < num_cont_entries; i++) {
        strncat(name, (char*)pos, ENTRY_SIZE);
        pos += ENTRY_SIZE;
    }

    name[strlen(name)] = '\0';

    return name;
}

// Populate an array of entry structs with a list of entries found in the filesystem
void find_all_entries(struct entry entries[]) {
    unsigned char* pos = rootfs_start + (1 * BYTES_PER_BLOCKS) + (rootfs_superblock.data_area_size * BYTES_PER_BLOCKS); // Pointer to the index area so we can search for entries
    unsigned int cur_entry = 0;                                                                                         // Used to keep track of the next index of the struct in the array

    if (*pos == STARTING_MARKER_ENTRY) {
        pos += ENTRY_SIZE; // Skip over the starting marker entry

        uint8_t entry_type;
        uint8_t num_cont_entries;
        uint8_t entry_name_len;
        uint8_t entry_name_offset;
        uint64_t timestamp;

        while (cur_entry < (total_rootfs_entries)) { // total_entries-1
            // Determine the type of the entry and the number of continuation entries that follow it
            entry_type = *pos;
            num_cont_entries = *(pos + 1);
            timestamp = *(pos + 2);

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
            case DELETED_DIRECTORY_ENTRY:
                entry_name_len = DIRECTORY_NAME_ENTRY_LEN;
                entry_name_offset = DIRECTORY_NAME_OFFSET;
                break;
            case DELETED_FILE_ENTRY:
                entry_name_len = FILENAME_ENTRY_LEN;
                entry_name_offset = FILENAME_OFFSET;
                break;
            }

            // Store the name of the entry (for directories and files)
            char name[entry_name_len];

            strncpy(name, (const char*)pos + entry_name_offset, sizeof(name));
            name[entry_name_len] = '\0';

            // Check if the entry is a directory entry
            if (entry_type == DIRECTORY_ENTRY) {
                struct directory_entry dir_ent;

                // Set the directory entry's information
                dir_ent.type = DIRECTORY_ENTRY;
                dir_ent.num_cont_entries = num_cont_entries;
                dir_ent.timestamp = timestamp;

                strncpy((char*)dir_ent.name, (const char*)name, sizeof(name));

                // Add the directory entry to the list of entries
                entries[cur_entry].type = DIRECTORY_ENTRY;
                entries[cur_entry].entry.dir_ent = dir_ent;
            } else if (entry_type == FILE_ENTRY) {
                struct file_entry file_ent;

                // Set the file entry's information
                file_ent.type = FILE_ENTRY;
                file_ent.num_cont_entries = num_cont_entries;
                file_ent.timestamp = timestamp;
                file_ent.start_block = *(pos + 10);
                file_ent.end_block = *(pos + 18);
                file_ent.size = *(pos + 26);

                strncpy((char*)file_ent.filename, (const char*)name, sizeof(name));

                // Add the file entry to the list of entries
                entries[cur_entry].type = FILE_ENTRY;
                entries[cur_entry].entry.file_ent = file_ent;
            } else if (entry_type == DELETED_DIRECTORY_ENTRY) {
                struct deleted_directory_entry del_dir_ent;

                // Set the deleted directory entry's information
                del_dir_ent.type = DELETED_DIRECTORY_ENTRY;
                del_dir_ent.num_cont_entries = num_cont_entries;
                del_dir_ent.timestamp = timestamp;
                memset(&del_dir_ent.reserved, 0, sizeof(del_dir_ent.reserved));

                // Add the directory entry to the list of entries
                entries[cur_entry].type = DELETED_DIRECTORY_ENTRY;
                entries[cur_entry].entry.del_dir_ent = del_dir_ent;
            } else if (entry_type == DELETED_FILE_ENTRY) {
                struct deleted_file_entry del_file_ent;

                // Set the deleted file entry's information
                del_file_ent.type = DELETED_FILE_ENTRY;
                del_file_ent.num_cont_entries = num_cont_entries;
                del_file_ent.timestamp = timestamp;
                del_file_ent.start_block = *(pos + 10);
                del_file_ent.end_block = *(pos + 18);
                del_file_ent.size = *(pos + 26);

                strncpy((char*)del_file_ent.filename, (const char*)name, sizeof(name));

                // Add the file entry to the list of entries
                entries[cur_entry].type = DELETED_FILE_ENTRY;
                entries[cur_entry].entry.del_file_ent = del_file_ent;
            }

            cur_entry++;
            pos += ENTRY_SIZE;

            // If there are continuation entries, create a new struct for each one and add it to the array
            if (num_cont_entries > 0) {
                for (uint8_t i = 0; i < num_cont_entries; i++) {
                    // Create a new continuation entry struct and copy over the name into it
                    struct continuation_entry cont_entry;

                    strncpy((char*)cont_entry.name, (const char*)pos, ENTRY_SIZE - 1);
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

// Get a list of names of all the entries in the filesystem
// TODO: check `_entry_type` to determine the size of the `entry_names` array
void get_entry_names(char** entry_names, struct entry entries[], int _entry_type) {
    unsigned int i = 0; // Keep track of the name of the current entry to add to the array

    // Get a list of all the filesystem entries
    find_all_entries(entries);

    char* name; // The name of the entry
    struct entry cur_entry;
    uint8_t entry_type;

    for (unsigned int j = 0; j < total_rootfs_entries; j++) {
        uint8_t num_cont_entries = 0;
        uint8_t entry_name_len = 0;

        cur_entry = entries[j];
        entry_type = cur_entry.type;

        /*
         * Determine the number of continuation entries for the current entry.
         * Determine the length of the entry's name
         */
        // Directory entry
        if ((_entry_type & entry_type) == DIRECTORY_ENTRY) {
            num_cont_entries = cur_entry.entry.dir_ent.num_cont_entries;
            entry_name_len = DIRECTORY_NAME_ENTRY_LEN;
        }
        // File entry
        else if ((_entry_type & entry_type) == FILE_ENTRY) {
            num_cont_entries = cur_entry.entry.file_ent.num_cont_entries;
            entry_name_len = FILENAME_ENTRY_LEN;
        }

        int name_len = entry_name_len + (num_cont_entries * ENTRY_SIZE);
        // Store the name of the entry (files and folders only)
        name = (char*)kcalloc(name_len, 1);

        // Set an error and return NULL if we could not allocate enough memory for the file or folder name
        if (name == NULL) {
            // ERROR: causing "error message is too long"
            // set_error(MEMORY_ALLOC_FAILED);
            continue;
        }

        // Copy the entry name in to the `name` array.
        // Deleted directory entries do not have a name
        if ((_entry_type & entry_type) == DIRECTORY_ENTRY) {
            strncpy(name, (const char*)cur_entry.entry.dir_ent.name,
                    sizeof(cur_entry.entry.dir_ent.name));
        } else if ((_entry_type & entry_type) == FILE_ENTRY) {
            strncpy(name, (const char*)cur_entry.entry.file_ent.filename,
                    sizeof(cur_entry.entry.file_ent.filename));
        }

        /*
         * If there are continuation entries following the current entry,
         * add the name of the continuation entry onto the end of
         * the current entry's name
         */
        if (num_cont_entries > 0) {
            for (uint8_t k = 0; k < num_cont_entries; k++) {
                char cont_name[ENTRY_SIZE];
                memset(cont_name, 0, sizeof(cont_name));

                strncpy((char*)cont_name,
                        (const char*)entries[j + k + 1].entry.cont_entry.name, ENTRY_SIZE - 1);
                cont_name[ENTRY_SIZE] = '\0';

                strncat(name, cont_name, sizeof(cont_name));
            }
        }

        name[name_len] = '\0'; // Add the null byte to the end of the string

        // Add the name of the entry to the array
        if (((_entry_type & DIRECTORY_ENTRY) == DIRECTORY_ENTRY && entry_type == DIRECTORY_ENTRY) || ((_entry_type & FILE_ENTRY) == FILE_ENTRY && entry_type == FILE_ENTRY)) {
            // kprintf("%s ", name);
            //memcpy(entry_names[i++], name, strlen(name));
            entry_names[i++] = name;
        }

        // Skip over all the continuation entries to avoid reading them again.
        j += (num_cont_entries);

        kfree(name);
    }
}

// Get a count of all the files and directories currently in the filesystem
void count_files_and_dirs() {
    struct entry entries[total_rootfs_entries];

    // Get a list of the names of all the entries in the filesystem
    find_all_entries(entries);

    // Loop through all the entries in the filesystem and determine if each entry is a file or folder
    for (unsigned int i = 0; i < total_rootfs_entries; i++) {
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

// Determine whether the filename contains invalid characters or if it is valid
bool is_filename_valid(char* filename) {
    /* 
     * Check for any invalid characters in the filename.
     * Invalid characters are:
     *   control characters (< 0x20)
     *   " (0x22)
     *   * (0x2A)
     *   : (0x3A)
     *   < (0x3C)
     *   > (0x3E)
     *   ? (0x3F)
     *   \ (0x5C)
     *   DEL (0x7F)
     *   NBSP (0xA0, replaced with a space)
     */
    for (unsigned int i = 0; i < strlen(filename); i++) {
        unsigned char c = filename[i];

        // List of invalid characters. Return NULL
        if (c < 0x20 || c == '"' || c == '*' || c == ':' || c == '<' || c == '>' || c == '?' || c == '\\' || c == 0x7F) {
            return false;
        } else if (c == 0xA0) {
            filename[i] = ' '; // Replace no break space character (NBSP) with a space
        }
    }

    return true;
}

// Determine if a file or directory exists in the filesystem
// ERROR: NOT WORKING
bool path_exists(char* path) {
    // Get a list of all the file and directories names in the filesystem
    struct entry entries[total_rootfs_entries];
    char** entry_names = kmalloc(total_rootfs_files + total_rootfs_dirs);

    if (entry_names == NULL) {
        return false;
    }

    get_entry_names(entry_names, entries, DIRECTORY_ENTRY | FILE_ENTRY);

    for (unsigned int i = 0; i < total_rootfs_dirs + total_rootfs_files; i++) {
        if (strcmp(entry_names[i], path) == 0) {
            // kfree(entry_names);
            // kprintf("%s!", entry_names[i]);
            return true;
        }
    }

    kfree(entry_names);
    return false;
}

// Determine if an entry is a directory by checking it's type
bool is_dir(char* path) {
    make_full_path(path);
    unsigned char* offset = get_offset_by_entry_name(path);

    return *offset == DIRECTORY_ENTRY;
}

// Determine if an entry is a file by checking it's type
bool is_file(char* path) {
    make_full_path(path);
    unsigned char* offset = get_offset_by_entry_name(path);

    return *offset == FILE_ENTRY;
}