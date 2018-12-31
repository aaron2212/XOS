#include "../../libc/include/_null.h"
#include "fs.h"
#include "vfs.h"

// Set the position to add the filesystem to in the array
void init_mounts()
{
    mount_index = 0;
}

// Register the filesystem by adding it to the list of mount points
int register_fs(struct filesystem fs)
{
    // Too many filesystems have already been mounted
    if (mount_index >= MAX_MOUNTS)
        return set_fs_error(TOO_MANY_MOUNTS);

    // Filesystem name too long
    if (strlen(fs.name) > FS_NAME_LEN || strlen(fs.name) == 0)
        return set_fs_error(NAME_TOO_LONG);


    // Filesystem type too long
    if (strlen(fs.type) > FS_TYPE_LEN || strlen(fs.type) == 0)
        return set_fs_error(TYPE_TOO_LONG);
    
    // Filesystem mount point too long
    if (strlen(fs.mount_point) > FS_MOUNT_POINT_LEN || strlen(fs.mount_point) == 0)
        return set_fs_error(MOUNT_POINT_TOO_LONG);

    // Filesystem function operation(s) not set
    if (fs.open == NULL || fs.close == NULL || fs.read == NULL || fs.write == NULL ||
            fs.readdir == NULL || fs.mount == NULL || fs.umount == NULL)
        return set_fs_error(IO_OPERATION_NOT_SET);
    
    // Check if a filesystem is already mounted at the same mount point
    for (int i=0; i<MAX_MOUNTS; i++) {
        if (strcmp(mount_points[i].mount_point, fs.mount_point) == 0 && strlen(mount_points[i].mount_point) > 0) {
            return set_fs_error(FS_ALREADY_MOUNTED);
        }
    }
    
    // There are no errors with the filesystem structure, register it
    mount_points[mount_index++] = fs;

    return 0;
}

// Unregister the filesystem by remove it from the list of mount points
void unregister_fs(struct filesystem fs)
{
    for (int i=0; i<MAX_MOUNTS; i++) {
        // Unmount the filesystem only if it is already mounted.
        // Do not return an error if the filesystem was not already mounted
        if (strcmp(mount_points[i].mount_point, fs.mount_point) == 0) {
            // Set the filesystem properties to empty strings
            strcpy(mount_points[i].name, "");
            strcpy(mount_points[i].mount_point, "");
            strcpy(mount_points[i].type, "");

            // Set the filesystem I/O operations to NULL
            mount_points[i].open     = NULL;
            mount_points[i].close    = NULL;
            mount_points[i].read     = NULL;
            mount_points[i].write    = NULL;
            mount_points[i].readdir  = NULL;
            mount_points[i].mount    = NULL;
            mount_points[i].umount   = NULL;
            mount_points[i].readonly = false;

            // So that the next filesystem registered will be registered where the
            // current filesystem was removed from the array
            mount_index = i;
        }
    }
}

// Print the filesystem error message from the error code received when the error was encountered
int set_fs_error(enum fs_errors error_code)
{
    // Set the error message based on the error code
    switch (error_code) {
        case TOO_MANY_MOUNTS:
            set_error("too many filesystems already registered");
            break;
        case INVALID_STRUCTURE:
            set_error("invalid filesystem structure");
            break;
        case NAME_TOO_LONG:
            set_error("filesystem name is too long or has not been set");
            break;
        case TYPE_TOO_LONG:
            set_error("filesystem type is too long or has not been set");
            break;
        case MOUNT_POINT_TOO_LONG:
            set_error("filesystem mount point is too long or has not been set");
            break;
        case IO_OPERATION_NOT_SET:
            set_error("1 or more I/O operations have not been set");
            break;
        case FS_ALREADY_MOUNTED:
            set_error("a filesystem has already been mounted at that mount point");
            break;
        case PATH_NOT_FOUND:
            set_error("path not found");
            break;
    }

    // Failed to perform the requested action
    return -1;
}

// Looks at the filename passed in and compares its substring to that of each mount point in the list
int find_fs_by_filename(const char* filename)
{
    /*
     * Iterate over each filesystem, except for the root filesystem, because its mount point
     * will always match filenames starting with a '/', even if the mount point refers to a
     * different filesystem, and compare its mount point with the filename.
     * If the mount point matches the first few bytes of the filename,
     * return the index into the mount point list
     */
    for (int i=1; i<MAX_MOUNTS; i++) {
        if (strlen(mount_points[i].mount_point) > 0) {
            if (strncmp(filename, mount_points[i].mount_point, strlen(mount_points[i].mount_point)) == 0) {
                return i;
            }
        }
    }

    // If there was no match, match the filename against the mount point of the rootfs filesystem
    if (strlen(mount_points[0].mount_point) > 0) {
        if (strncmp(filename, mount_points[0].mount_point, strlen(mount_points[0].mount_point)) == 0) {
            return 0;
        }
    }

    // No filesystem was found when matching the mount point with the filename
    return -1;
}

// Attempt to open the file by `filename` on the correct filesystem. Return a FILE stream
FILE* vfs_open(const char* filename, const char* mode)
{
    int fs_index = find_fs_by_filename(filename);

    // If a filesystem matching the filename was not found, assume it was a relative path.
    if (fs_index == -1) {
        int full_path_len = strlen(filename) + 1 + strlen(current_dir);
        // Length of current filename + 1 for path separator + lenght of the current directory
        char* full_path = kcalloc(full_path_len+1, 1);

        // Add on the current directory to convert the path to an absolute path and try to find the filesystem again
        strcpy(full_path, current_dir);

        // Do not add a leading '/' if the directory begins with a '/'
        if (current_dir[strlen(current_dir)-1] != '/')
            strcat(full_path, PATH_SEPARATOR); // wrong? should be current_dir[0] != '/'??
        
        strcat(full_path, filename);

        int fs_index = find_fs_by_filename(full_path);

        /*
         * Tried to find filesystem by mount point using relative and absolute paths.
         * Return NULL if no filesystem was found, otherwise a FILE stream
         * from the filesystem's open function()
         */
        if (fs_index == -1) {
            set_fs_error(PATH_NOT_FOUND);
        } else {
            kfree(full_path);

            // Return the opened file/directory
            /*
             * The path is a relative path.
             * Prevent opening the file for writing if the filesystem is marked as read-only
             */
            if (strchr(mode, 'w') && mount_points[fs_index].readonly) {
                set_error("Unable to open file: filesystem is readonly");

                return NULL;
            }
        }

        kfree(full_path);
        //return; // TODO: return a file stream
    }
    
    /*
     * The path is a relative path.
     * Prevent opening the file for writing if the filesystem is marked as read-only
     */
    if (strchr(mode, 'w') && mount_points[fs_index].readonly) {
        set_error("Unable to open file: filesystem is readonly");

        return NULL;
    }

    // Add the filesystem structure corresponding to the open FILE stream to the list of open files
    memcpy(&file_streams[current_file_descriptor],
            &mount_points[fs_index], sizeof(mount_points[fs_index]));

    return mount_points[fs_index].open(filename);
}

// Read `len` bytes from the file into `buf`
int vfs_read(FILE* stream, char* buf, size_t len)
{
    return file_streams[stream->fd].read(stream, buf, len);
}

// Close the file stream `stream` by removing it from the list of open file streams
void vfs_close(FILE* stream)
{
    // An empty mount point string indicates that there is no currently open file stream
    strcpy(file_streams[stream->fd].mount_point, "");
}