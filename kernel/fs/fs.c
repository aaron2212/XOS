#include "fs.h"
#include "sfs.h"
#include "vfs.h"

#include "../../libc/include/_null.h"

// Perform initialization of the rootfs filesystem
int init_fs(multiboot_info_t *mbinfo)
{
    /*
     * Invalidate each file stream by setting the mount point to ""
     * TODO: change to allow for not overriding stdint, stdout, stderr
     */
    for (unsigned int i = 0; i < MAX_OPEN_FILES; i++)
    {
        strcpy(file_streams[i].mount_point, "");
    }

    /*
     * Invalidate all filesystems upon boot by settings each mount point to an empty string
     */
    for (unsigned int i = 0; i < MAX_MOUNTS; i++)
    {
        strcpy(mount_points[i].mount_point, "");
    }

    // Check if GRUB was able to load the filesystem module correctly
    if (mbinfo->mods_count > 0)
    {
        // Get the address of the filesystem loaded as a GRUB module
        int *fs_mod_start = load_module(mbinfo, "filesystem");

        if (fs_mod_start != NULL)
        {
            // The structure containig the root filesystem information and functions
            struct filesystem rootfs;

            // The name and the path that the rootfs filesystem is mounted at, as well as its type
            strcpy(rootfs.name, ROOTFS_NAME);
            strcpy(rootfs.mount_point, ROOTFS_MOUNT_POINT);
            strcpy(rootfs.type, ROOTFS_TYPE);

            // Functions to call when the rootfs file operation methods are called
            rootfs.init = init;
            rootfs.open = open;
            rootfs.read = read;
            rootfs.write = write;
            rootfs.create = create;
            rootfs.opendir = opendir;
            rootfs.readdir = readdir;
            rootfs.changedir = changedir;
            rootfs.closedir = closedir;
            rootfs.create = create;
            rootfs.rm = rm;
            rootfs.rmdir = rmdir;

            // Set the rootfs filesystem as read/write
            rootfs.readonly = false;

            mount_index = 0;

            // 0, 1, 2 for stdin, stdout, stderr respectively
            current_file_descriptor = 3;

            // Set the current directory that the shell launches to
            strcpy(current_dir, "/");

            // Register the rootfs filesystem and check if it was successfully registered or not
            if (register_fs(rootfs) == -1)
                return -1;

            // The location after the heap where the filesystem will be copied to
            rootfs_start = (unsigned char *)kernel_end + heap_size + 1;

            // Retrieve the total number of blocks the filesystem uses, as stored in the superblock
            unsigned long data_blocks;
            unsigned long index_size;

            // Determine the total number of blocks and the size of the index area
            memcpy(&data_blocks, (unsigned char *)fs_mod_start + 412, sizeof(unsigned long));
            memcpy(&index_size, (unsigned char *)fs_mod_start + 420, sizeof(unsigned long));

            //Copy over the filesystem, loaded by GRUB, after the heap
            unsigned int fs_size = ((1 + data_blocks) * BYTES_PER_BLOCKS) + index_size; //47232

            // Determine the minimum amount of memory required to run the system
            unsigned int required_memory = ((kernel_end + (heap_size) + fs_size + 1));

            if (available_memory < required_memory)
            {
                set_error("Not enough memory. Minimum required memory is %dMB\n", (required_memory / 1024 / 1024));
            }

            // Copy the filesystem from the area GRUB loaded the filesystem as a module into another area in memory
            memcpy(rootfs_start, fs_mod_start, fs_size);

            // Determine the ending memory address of the filesystem
            rootfs_end = rootfs_start + fs_size;

            // kprintf("fs_size=%d\n", fs_size);

            rootfs.init();

            return 0;
        }

        // Failed to load the filesystem module from GRUB
        return -1;
    }

    // GRUB failed to load ALL modules
    return -1;
}