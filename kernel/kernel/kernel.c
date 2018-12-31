#include "kernel.h"
#include "../include/tty.c"
#include "../include/shell.c"
#include "../../include/multiboot.h"
#include "../modules/modules.c"
#include "../fs/fs.h"
#include "../fs/sfs.h"
#include "../fs/vfs.h"

/* main part of kernel code */

// TODO: move all printf() status messages to the appropriate functions

void k_main(void* mb_struct)
{
	terminal_init();
	printf("XOS booting...\n\n");

	// Store multiboot information in a structure
	multiboot_info_t* mbinfo = (multiboot_info_t*) mb_struct;

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
	printf("Initializing the root filesystem... ");
	if (init_rootfs(mbinfo) != 0) {
		printf("Failed to initialize filesystem: %s\n\n", get_last_error());
	} else {
		printf("OK\n");
		is_fs_loaded = true;
	}

	// Setup the IDT, GDT, timer and keyboard
	printf("Initializing the GDT... ");
	init_gdt();
	printf("OK\n");
	printf("Initializing the IDT and interrupts... ");
	init_idt();
	init_isrs();
	init_irqs();
	//init_timer(); /* not working */
	init_keyboard();
	printf("OK\n\n");
	
	// Status information
	printf("Kernel ends at address 0x%x\n", kernel_end);
	printf("Installed memory (RAM): %dMB\n", (available_memory/1024/1024));
	printf("Heap size: %dMB\n", (heap_size/1024/1024));

	// If the filesystem was loaded successfully (enough memory, mounted filesystem etc)
	if (is_fs_loaded)
		printf("Filesystem loaded at address 0x%x: %d directories, %d files\n", 
				rootfs_start, total_rootfs_dirs, total_rootfs_files);
	else
		printf("Could not load filesystem: %s\n", get_last_error());

	//const char* filename = "/home/aaron/documents/DILE Assignment Q1.docx";
	  const char* filename = "/bin/ls";

	FILE* fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Failed to open file\n");
	}

	printf("File %s has start_block=%d and size=%d\n\n", filename, fp->start_block, fp->size);

	printf("ftell=%d\n", ftell(fp));

	int c = fgetc(fp);

	printf("c=%c\n", c);

	printf("ftell=%d\n", ftell(fp));

	char buf[41];

	fread(buf, sizeof(buf), 1, fp);

	buf[41] = '\0';
	
	printf("buf=%s\n", buf);

	printf("ftell=%d\n\n", ftell(fp));

	while (1) {
		shell();
	}
}