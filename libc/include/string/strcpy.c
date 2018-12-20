#include "string.h"

char *strcpy(char *dst, const char *src)
{
	return memcpy(dst, src, strlen(src) + 1);
}