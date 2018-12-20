#include "string.h"

void* memcpy(void *dest, const void *src, size_t size)
{
	unsigned char *dst = (unsigned char *) dest;
	const unsigned char *sc = (const unsigned char *) src;

	for (size_t i=0; i<size; i++)
		dst[i] = sc[i];

	return dest;
}
