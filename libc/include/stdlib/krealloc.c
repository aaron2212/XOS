#include "stdint.h"
#include "stdlib.h"

extern int kprintf(const char* fmt, ...);
extern void *memcpy(void *dest, const void *src, size_t size);

void* krealloc(void* ptr, size_t size)
{
	unsigned char* new_ptr = (unsigned char*) kmalloc(size);

	// Check if we got a valid pointer from kmalloc()
	if (new_ptr != NULL) {
		unsigned char* char_ptr = (unsigned char*) ptr;

		/*
		* if the old block is in use (allocated bit is at beginning of header
		*  and pointer points to 3 bytes after header), set it as unused
		*/

		if (*(char_ptr-5) == 1)
		{
			kfree(char_ptr);

			unsigned int blocks_allocated = *(char_ptr-4);

			memcpy(new_ptr, char_ptr, (blocks_allocated*BLOCK_SIZE));
		}

		return new_ptr;
	}

	return NULL;
}