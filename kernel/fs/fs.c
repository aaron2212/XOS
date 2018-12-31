#include "fs.h"
#include "sfs.h"
#include "vfs.h"

#include "../../libc/include/_null.h"

// Perform initialization of all filesystem related tasks
int init_fs(multiboot_info_t* mbinfo)
{
    /*
     * Invalidate each file stream
     */
    for (unsigned int i=0; i<sizeof(file_streams); i++) {
        strcpy(file_streams[i].mount_point, "");
    }

    return init_rootfs(mbinfo);
}

// Initialize and setup the root filesystem
int init_rootfs(multiboot_info_t* mbinfo)
{
    // Initialize the list of mount points to NULL values
    init_mounts();

    // 0, 1, 2 for stdin, stdout, stderr respectively
    current_file_descriptor = 3;

    // Set the current directory to '/'
    current_dir = kcalloc(CURRENT_DIR_BUF, 1);
    strcpy(current_dir, "/");

    struct filesystem rootfs = {0};
    
    // The name and the path that the rootfs filesystem is mounted at, as well as its type
    strcpy(rootfs.name, ROOTFS_NAME);
    strcpy(rootfs.mount_point, ROOTFS_MOUNT_POINT);
    strcpy(rootfs.type, ROOTFS_TYPE);

    // Functional to call when the rootfs file operation methods are called
    rootfs.open    = open;
    rootfs.close   = close;
    rootfs.read    = read;
    rootfs.write   = write;
    rootfs.readdir = readdir;
    rootfs.mount   = mount;
    rootfs.umount  = umount;

    // Set the rootfs filesystem as read-only (temporary)
    rootfs.readonly = false;

    // Register the rootfs filesystem
    int result = register_fs(rootfs);

    // Copy over the rootfs filesystem loaded into memory through a GRUB module, after the heap
    if (mbinfo->mods_count > 0) {
        // Get the address of the filesystem loaded as a GRUB module
        int* fs_mod_start = load_module(mbinfo, "filesystem");

        if (fs_mod_start != NULL) {
            // The location after the heap where the filesystem will be copied to
            rootfs_start = (unsigned char*) kernel_end + heap_size + 1;

            // Retrieve the total number of blocks the filesystem uses, as stored in the superblock
			unsigned long total_blocks = *((char*) fs_mod_start+432);

            //Copy over the filesystem, loaded by GRUB, after the heap
			unsigned int fs_size = (total_blocks * BYTES_PER_BLOCKS);

            // Determine the minimum amount of memory required to run the system
            unsigned int required_memory = ((kernel_end+(heap_size)+fs_size+1));

            if (available_memory < required_memory) {
                set_error("Not enough memory. Minimum required memory is %dMB\n", (required_memory/1024/1024));
            }

			memcpy(rootfs_start, fs_mod_start, fs_size);

            // Determine the ending memory address of the filesystem
            rootfs_end = rootfs_start + fs_size;

            // perhaps TODO: create init_superblock() for each filesystem and call upon register_fs()
            init_sfs();
        } else {
            set_error("Failed to load filesystem from GRUB module");
            return -1;
        }
    } else {
        set_error("Failed to load GRUB modules");
        return -1;
    }

    // Return the result of the register_fs() function call
    return result;
}