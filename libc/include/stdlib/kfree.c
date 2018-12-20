#include "stdlib.h"

void kfree(void* ptr)
{
	/* if allocated bit is set,
	 * unset it and advance 4096 bytes
	 * if more that 1 block is needed to be freed
	 */

	unsigned char* pointer  = (unsigned char *) ptr;			/* cast to char pointer for checking bytes in memory */

	/* The block is allocated, deallocate it
	 * Leave the rest of the data in memory; it will be overwritten
	 */
	if (*(pointer-5) == 1)
	{
		*(pointer-5) = 0;
	}
}
