#include "modules.h"

/*
	Func: load_module
	Get the address of a module loaded by GRUB

	Parameters:
		mbinfo - a structure representing the multiboot information received from GRUB
		module - the name of the module to load
	
	Returns:
		The address of the loaded GRUB module, or NULL if the module could not be loaded
*/
int* load_module(multiboot_info_t* mbinfo, const char* module)
{
	// Load the filesystem module
	if (strcmp(module, "filesystem") == 0) {
		return load_module_by_magic(mbinfo, FS_MODULE_MAGIC, FS_MODULE_MAGIC_OFFSET);
	}

	set_error("failed to load %s module", module);

	return NULL;
}

/*
	Func: load_module_by_magic
	Load a GRUB module given the magic signature to search for

	Parameters:
		mbinfo - a structure representing the multiboot information received from GRUB
		magic - the magic signature to search for in the module
		offset - the offset of the magic number within in the GRUB module
	
	Returns:
		The address of the loaded GRUB module, or NULL if the module could not be loaded
*/
int* load_module_by_magic(multiboot_info_t* mbinfo, const char* magic, int offset)
{
	// Get the pointer to the struct of GRUB loaded modules
	multiboot_module_t* modules = (multiboot_module_t*) mbinfo->mods_addr;

	for (unsigned int i=0; i<mbinfo->mods_count; i++) {
		// Create an array to store what the magic number for the module
		char module_magic[strlen(magic)+1];

		memcpy(module_magic, (char*) modules[i].mod_start+offset, strlen(magic));
		module_magic[strlen(magic)+1] = '\0';

		// If the module magic number is found, return the address of the module
		if (strcmp(module_magic, magic) == 0) {
			// Return the starting address of the loaded module
			return (int*) modules[i].mod_start;
		}
	}

	set_error("failed to load module by magic");
	return NULL;
}