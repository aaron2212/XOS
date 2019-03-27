#include "../../libc/include/_null.h"
#include "fs.h"
#include "vfs.h"

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
    if (fs.open == NULL || fs.read == NULL || fs.write == NULL ||
        fs.readdir == NULL)
        return set_fs_error(IO_OPERATION_NOT_SET);

    // Check if a filesystem is already mounted at the same mount point
    for (int i = 0; i < MAX_MOUNTS; i++)
    {
        if (strcmp(mount_points[i].mount_point, fs.mount_point) == 0 && strlen(mount_points[i].mount_point) > 0)
        {
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
    for (int i = 0; i < MAX_MOUNTS; i++)
    {
        // Unmount the filesystem only if it is already mounted.
        // Do not return an error if the filesystem was not already mounted
        if (strcmp(mount_points[i].mount_point, fs.mount_point) == 0)
        {
            // Set the filesystem properties to empty strings
            strcpy(mount_points[i].name, "");
            strcpy(mount_points[i].mount_point, "");
            strcpy(mount_points[i].type, "");

            // Set the filesystem I/O operations to NULL
            mount_points[i].open = NULL;
            mount_points[i].read = NULL;
            mount_points[i].write = NULL;
            mount_points[i].readdir = NULL;
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
    switch (error_code)
    {
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
    case INVALID_FILENAME:
        set_error("Invalid characters in filename");
        break;
    }

    // Failed to perform the requested action
    return -1;
}

// Looks at the filename passed in and compares its substring to that of each mount point in the list
int find_fs_by_filename(const char *filename)
{
    /*
     * Iterate over each filesystem, except for the root filesystem, because its mount point
     * will always match filenames starting with a '/', even if the mount point refers to a
     * different filesystem, and compare its mount point with the filename.
     * If the mount point matches the first few bytes of the filename,
     * return the index into the mount point list
     */
    for (int i = 1; i < MAX_MOUNTS; i++)
    {
        if (strlen(mount_points[i].mount_point) > 0)
        {
            if (strncmp(filename, mount_points[i].mount_point, strlen(mount_points[i].mount_point)) == 0)
            {
                return i;
            }
        }
    }

    // If there was no match, match the filename against the mount point of the rootfs filesystem
    if (strlen(mount_points[0].mount_point) > 0)
    {
        if (strncmp(filename, mount_points[0].mount_point, strlen(mount_points[0].mount_point)) == 0)
        {
            return 0;
        }
    }

    // No filesystem was found when matching the mount point with the filename
    return -1;
}

// Return the filesystem associated with the file
struct filesystem get_fs(const char *filename)
{
    int fs_index = find_fs_by_filename(filename);

    // If a filesystem matching the filename was not found, assume it was a relative path.
    if (fs_index == -1)
    {
        int full_path_len = strlen(filename) + 1 + strlen(current_dir);
        // Length of current filename + 1 for path separator + lenght of the current directory
        char *full_path = kcalloc(full_path_len + 1, 1);

        // Add on the current directory to convert the path to an absolute path and try to find the filesystem again
        strcpy(full_path, current_dir);

        // Do not add a leading '/' if the directory begins with a '/'
        if (current_dir[strlen(current_dir) - 1] != '/')
            strcat(full_path, PATH_SEPARATOR); // wrong? should be current_dir[0] != '/'??

        strcat(full_path, filename);

        int fs_index = find_fs_by_filename(full_path);

        /*
         * Tried to find filesystem by mount point using relative and absolute paths.
         * Return NULL if no filesystem was found, otherwise a FILE stream
         * from the filesystem's open function()
         */
        if (fs_index == -1)
        {
            set_fs_error(PATH_NOT_FOUND);
        }
        else
        {
            kfree(full_path);
        }

        kfree(full_path);
    }

    return mount_points[fs_index];
}

// Attempt to open the file by `filename` on the correct filesystem. Return a FILE stream
FILE *vfs_open(char *filename, const char *mode)
{
    char *path = filename;

    // Make the path `filename` an absolute path
    make_full_path(path);

    struct filesystem fs = get_fs(filename);

    if (strchr(mode, 'w') && fs.readonly)
    {
        set_error("Unable to open file: filesystem is readonly");

        return NULL;
    }

    // Check for first free file descriptor in the open files table. Exclude standard streams
    for (unsigned int i = 3; i < MAX_OPEN_FILES; i++)
    {
        if (strlen(file_streams[i].mount_point) == 0)
        {
            current_file_descriptor = i;
            break;
        }
        // Cannot open more files than the system allows (MAX_OPEN_FILES)
        else if (i == MAX_OPEN_FILES - 1)
            return NULL;
    }

    // Add the filesystem structure corresponding to the open FILE stream to the list of open files
    memcpy(&file_streams[current_file_descriptor], &fs, sizeof(fs));

    return fs.open(filename);
}

// Read `len` bytes from the file into `buf`
int vfs_read(FILE *stream, char *buf, size_t len)
{
    return file_streams[stream->fd].read(stream, buf, len);
}

// Close the file stream `stream` by removing it from the list of open file streams
void vfs_close(FILE *stream)
{
    // An empty mount point string indicates that there is no currently open file stream
    strncpy(file_streams[stream->fd].mount_point, "", 0);

    for (unsigned int i = 3; i < MAX_OPEN_FILES; i++)
    {
        if (strlen(file_streams[i].mount_point) == 0)
        {
            current_file_descriptor = i;

            return;
        }
    }
}

// Create new empty file
FILE *vfs_create(char *filename)
{
    struct filesystem fs = get_fs(filename);

    // Prevent creating a file on a readonly filesystem
    if (fs.readonly)
    {
        set_error("Unable to create file: filesystem is readonly");

        return NULL;
    }

    // Add the filesystem structure corresponding to the open FILE stream to the list of open files
    memcpy(&file_streams[current_file_descriptor], &fs, sizeof(fs));

    return fs.create(filename);
}

// Change the current working directory
int vfs_changedir(char *dirname)
{
    // If the user passed in a `dirname`
    if (strlen(dirname) > 0)
    {
        // There exists no entry in the filesystem matching '/', so change the directory before checking the name
        if (strlen(dirname) == 1 && dirname[0] == ROOT_DIR)
        {
            current_dir = PATH_SEPARATOR;

            return 0;
        }

        char *full_path = "";

        strcpy(full_path, dirname);

        // Parse the directory name
        make_full_path(full_path);

        // Call the appropriate filesystem's function to change the current directory
        return get_fs(full_path).changedir(full_path);
    }
    else
    {
        return -1;
    }
}

// Open a directory and return a DIR struct
DIR *vfs_opendir(char *dirname)
{
    // Make the path `filename` an absolute path
    make_full_path(dirname);

    struct filesystem fs = get_fs(dirname);

    // Check for first free file descriptor in the open files table. Exclude standard streams
    for (unsigned int i = 3; i < MAX_OPEN_FILES; i++)
    {
        if (strlen(file_streams[i].mount_point) == 0)
        {
            current_file_descriptor = i;
            break;
        }
        // Cannot open more files than the system allows (MAX_OPEN_FILES)
        else if (i == MAX_OPEN_FILES - 1)
            return NULL;
    }

    // Add the filesystem structure corresponding to the open FILE stream to the list of open files
    memcpy(&file_streams[current_file_descriptor], &fs, sizeof(fs));

    return fs.opendir(dirname);
}

// Read the contents of a directory
struct dirent *vfs_readdir(DIR *dir)
{
    return file_streams[dir->fd].readdir(dir);
}

// Close a previously opened directory
int vfs_closedir(DIR *dir)
{
    // Return an error if `dir` is NULL, indicating there there was an error opening it
    if (!dir)
        return -1;

    // Free the memory allocated to the directory struct and the memory allocated the the directory's name
    kfree(dir);
    kfree(dir->dirname);

    // An empty mount point string indicates that there is no currently open file stream
    strncpy(file_streams[dir->fd].mount_point, "", 0);

    // Find the first free file stream to reset `current_file_descriptor` to
    for (unsigned int i = 3; i < MAX_OPEN_FILES; i++)
    {
        if (strlen(file_streams[i].mount_point) == 0)
        {
            current_file_descriptor = i;
        }
    }

    // Deallocate all the memory allocated for each `dirent` struct and its `name` member
    for (unsigned int i = 0; i < dir->total_entries; i++)
    {
        kfree(dir->entries[i]);
        kfree(dir->entries[i]->name);
    }

    kfree(dir->entries);
    kfree(dir->dirname);
    kfree(dir);

    // Deallocate memory from the list of sub directory names
    // kfree(sub_dirnames);

    return 0;
}

// Attempt to remove (delete) a file
bool vfs_rm(char *name)
{
    char *path = name;

    // Make the path `filename` an absolute path
    make_full_path(path);

    struct filesystem fs = get_fs(path);

    // Check for first free file descriptor in the open files table. Exclude standard streams
    for (unsigned int i = 3; i < MAX_OPEN_FILES; i++)
    {
        if (strlen(file_streams[i].mount_point) == 0)
        {
            current_file_descriptor = i;
            break;
        }
        // Cannot open more files than the system allows (MAX_OPEN_FILES)
        else if (i == MAX_OPEN_FILES - 1)
            return false;
    }

    return fs.rm(path);
}

// Attempt to remove (delete) a directory
void vfs_rmdir(char *dirname)
{
    char *path = dirname;

    // Make the path `filename` an absolute path
    make_full_path(path);

    struct filesystem fs = get_fs(path);

    // Check for first free file descriptor in the open files table. Exclude standard streams
    for (unsigned int i = 3; i < MAX_OPEN_FILES; i++)
    {
        if (strlen(file_streams[i].mount_point) == 0)
        {
            current_file_descriptor = i;
            break;
        }
        // Cannot open more files than the system allows (MAX_OPEN_FILES)
        else if (i == MAX_OPEN_FILES - 1)
            return;
    }

    fs.rmdir(path);
}

/*
 * Parse the path into a proper path.
 * Parse "../" correctly by removing the previous part of the path.
 * Ignore any references to "."
 */
void make_proper_path(char *path)
{
    char *old_current_dir = kmalloc(strlen(current_dir));

    // Save the current directory, because for some reason, it gets overwritten with some other values
    strcpy(old_current_dir, current_dir);

    // Nothing needed to be done if the path is '/'
    if (strlen(path) == 1 && path[0] == PATH_SEPARATOR_CHAR)
        return;

    unsigned int num_path_separators = 0;
    bool starts_with_path_separator = (path[0] == '/');

    // Count the number of '/' in the path
    for (unsigned int i = 0; i < strlen(path); i++)
    {
        if (path[i] == '/')
            num_path_separators++;
    }

    char **dirs = kmalloc(num_path_separators);

    char *dir = strtok(path, PATH_SEPARATOR);
    unsigned int i = 0;        // An index into the `dirs` array. Also the number of directories in the path
    unsigned int dirs_len = 0; // The total length of all the directories

    // Split the path by each '/' and add it to the `dirs` array
    while (dir != NULL)
    {
        dir[strlen(dir)] = '\0';
        dirs[i++] = dir;

        dir = strtok(NULL, PATH_SEPARATOR);
    }

    dirs[i] = NULL;
    unsigned int new_array_size = i;

    /*
     * Iterate over the `dirs` array and parse each directory. 
     * ".." is removed and the previous element to.
     * "." is removed
     */
    unsigned int j = 0;

    while (j < i)
    {
        if (strcmp(dirs[j], ".") == 0)
        {
            strncpy(dirs[j], "", 0);
            new_array_size = remove_empty_strings(dirs, new_array_size);
        }
        else if (strcmp(dirs[j], "..") == 0)
        {
            strncpy(dirs[j], "", 0);

            if (j > 0)
                strncpy(dirs[j - 1], "", 0);

            new_array_size = remove_empty_strings(dirs, new_array_size);
            j = 0;
        }
        else
        {
            j++;
        }
    }

    // Create a new path by joining all
    unsigned int new_path_len = (starts_with_path_separator ? 1 : 0) + dirs_len + num_path_separators;
    char new_path[new_path_len * 2];

    // Initialize `new_path` to an empty string or to a '/',
    // based on if the original path started with a '/'
    if (starts_with_path_separator)
        strcpy(new_path, PATH_SEPARATOR);
    else
        strcpy(new_path, "");

    /*
     * Iterate over each element in the array and join them together into the string `new_path`
    */
    for (unsigned int j = 0; j < new_array_size; j++)
    {
        strcat(new_path, dirs[j]);

        // Do not add a trailing '/'
        if (j != i - 1)
            strcat(new_path, PATH_SEPARATOR);
    }

    new_path[strlen(new_path)] = '\0';

    // Set the path passed in to the new parsed path
    strcpy(path, new_path);

    kfree(dirs);

    // Restore the current directory
    strcpy(current_dir, old_current_dir);

    kfree(old_current_dir);
}

// Remove an empty string from the array by shifting everything after the empty string back once
int remove_empty_strings(char **array, unsigned int array_size)
{
    // Stores all the elements except for the empty strings
    char **new_array = kmalloc(array_size - 1);
    unsigned int j = 0; // An index into `new_array`
    unsigned int new_array_size = 0;

    for (unsigned int i = 0; i < array_size; i++)
    {
        if (strlen(array[i]) > 0)
        {
            new_array[j++] = array[i];
            new_array_size++;
        }
    }

    new_array[j] = NULL;

    memcpy(array, new_array, new_array_size * sizeof(char *));

    return new_array_size;
}

// Turns a relative path into an absolute path. Does nothing to absolute paths
void make_full_path(char *path)
{
    if (strlen(path) > 0)
    {
        unsigned int full_path_len = strlen(current_dir) + strlen(path) + 3;
        char full_path[full_path_len];

        // Determine if the path is absolute or relative
        if (path[0] == PATH_SEPARATOR_CHAR)
        {
            // Absolute
            strcpy(full_path, path);
        }
        else
        {
            // Relative
            strcpy(full_path, current_dir);
            strcat(full_path, PATH_SEPARATOR);
            strcat(full_path, path);
        }

        // Make the path a proper path, ie. parse "." and ".."
        make_proper_path(full_path);

        if (full_path[strlen(full_path) - 1] == PATH_SEPARATOR_CHAR)
            full_path[strlen(full_path) - 1] = '\0';

        strcpy(path, full_path);

        full_path[strlen(full_path)] = '\0';
    }
}