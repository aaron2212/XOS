#include "kernel.h"
#include "../../include/multiboot.h"
#include "../fs/fs.h"
#include "../fs/sfs.h"
#include "../fs/vfs.h"
#include "../include/shell.c"
#include "../include/tty.c"
#include "../modules/modules.c"
#include "../system/system.h"

/* main part of kernel code */

// TODO: move all kprintf() status messages to the appropriate functions

void k_main(void* mb_struct) {
    terminal_init();
    kprintf("XOS booting...\n\n");

    // Store multiboot information in a structure
    multiboot_info_t* mbinfo = (multiboot_info_t*)mb_struct;

    /*
	 * Determine the amount of memory available and set aside a percent of memory for the heap.
	 * GRUB returns the available memory in kilobytes, store it in bytes.
	 */
    available_memory = (mbinfo->mem_upper * 1024);
    heap_size = (available_memory * HEAP_SIZE_ALLOC_PERCENT);

    /*
	 * NOTE: the filesystem has to be initialized before setting up the GDT
	 * 		 and interrupts, as those will cause the GRUB structures to be invalidated.
	 * Determine whether the filesystem was register successfully.
	 * Initialize and setup the root filesystem
	 */
    kprintf("Initializing the root filesystem... ");
    if (init_fs(mbinfo) != 0) {
        kprintf("Failed to initialize filesystem: %s\n\n", get_last_error());
    } else {
        kprintf("OK\n");
        is_fs_loaded = true;
    }

    // Setup the IDT, GDT, timer and keyboard
    kprintf("Initializing the GDT... ");
    init_gdt();
    kprintf("OK\n");
    kprintf("Initializing the IDT and interrupts... ");
    init_idt();
    init_isrs();
    init_irqs();
    kprintf("\nInitializing the timer... ");
    init_timer();
    kprintf("OK\n\n");

    // Status information
    kprintf("Kernel ends at addresss 0x%x\n", kernel_end);
    kprintf("Installed memory (RAM): %dMB\n", (available_memory / 1024 / 1024));
    kprintf("Heap size: %dMB\n", (heap_size / 1024 / 1024)); // Convert heap size to megabytes

    // If the filesystem was loaded successfully (enough memory, mounted filesystem etc)
    if (is_fs_loaded)
        kprintf("Filesystem loaded at address 0x%x: %d directories, %d files\n",
                rootfs_start, total_rootfs_dirs, total_rootfs_files);
    else
        kprintf("Could not load filesystem: %s\n", get_last_error());

    // Initialize system configurations
    init_system();

    kprintf("Entering user mode...");
    // enter_usermode();
    kprintf("OK\n");

    // kprintf("\n");

    // asm("mov $0x23, %eax");
    
    while (1) {
        shell();
    }
}