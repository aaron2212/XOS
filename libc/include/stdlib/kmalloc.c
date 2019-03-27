#include <stdbool.h>

#include "../stdint.h"
#include "stdlib.h"
#include "../math/math.h"

/* `end` is the end of the kernel (symbol from linker)
 * pos is of type unsigned char*
 */

// TODO: https://codereview.stackexchange.com/questions/151019/implementing-realloc-in-c

extern unsigned int kernel_end;
extern unsigned int available_memory;
extern unsigned int heap_size;

unsigned char *pos = (unsigned char *)1024; // The beginning position of memory to allocate

void *kmalloc(size_t size)
{
	// If the user requested a size of 0, return a NULL pointer.
	// TODO: If the heap is full, return a NULL pointer
	if (size == 0)
	{
		return (void *)NULL;
	}

	// TODO: implement
	// WARNING: breaks os
	// if (pos+size > (unsigned char*) heap_size)
	// {
	// 	// if pos is at the end of the heap, find the first free block from
	// 	// the kernel_end, where subsequent searches will begin
	// 	return (void*) NULL;
	// }

	if (pos == 0)
		pos = (unsigned char *)kernel_end + 1;

	bool block_found = false;

	/* keep looping over the blocks until we find a free block.
	 * if the block is not free, increase pos, otherwise
	 * return a pointer to the beginning of the block (after the header)
	 */

	while (!block_found)
	{
		/* If the allocated bit is not set, set it and store the number of blocks needed as a short */
		if (*pos != 1)
		{
			*pos = 1;

			/* calculate the number of blocks needed based on the requested size, and the store i
             * if the size requested is a multiple of BLOCK_SIZE, the do not round up the number
             * of blocks needed, otherwise round it up
             */
			unsigned int blocks_needed = (size % 4096 == 0) ? (size / 4096) : ceil(size / 4096);

			*(pos + 1) = blocks_needed;

			/* we have found a block, exit the loop */
			block_found = true;
		}
		else
		{
			/* retrieve the number of allocated blocks */
			unsigned int blocks_allocated = *(pos + 1);

			/* increase pos by the number of allocated blocks * BLOCK_SIZE */
			pos += (blocks_allocated * BLOCK_SIZE) + 5;
		}
	}

	/* returns a pointer to the beginning of the block (after header) */

	return pos + 5;
}
