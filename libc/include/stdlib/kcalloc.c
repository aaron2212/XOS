#include "stdlib.h"

// Allocate an array of size `size` * `num` bytes and intialize all elements to 0
void* kcalloc(size_t size, size_t num)
{
    void* ptr = kmalloc(size * num);

    memset(ptr, 0, size*num);

    return ptr;
}