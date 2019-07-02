#include "../../libc/include/size_t.h"

#include "debug.h"

/*
    Func: dump_memory
    Display the contents of memory

    Parameters:
        ptr - the position in memory to start displaying from
        size - the number of bytes of memory to display
*/
void dump_memory(void* ptr, size_t size)
{
    //unsigned int line_len = 0;
    char* ptr2 = (char*) ptr;

    for (unsigned int i=0; i<size; i++)  {
        //unsigned int /*j,*/ len = 0;
        //char* str = "";

        // if (!((line_len+len) >= 80)) {
        kprintf("%x ", *(ptr2+i));
        // } else {
        //     printf("\n");
        //     line_len = 0;

        //     while (j<(3-len)) {
        //         printf(" ");
        //         j++;
        //     }
        // }

        //if (j == 0)

        // line_len += len;
    }

    kprintf("\n\n");
}