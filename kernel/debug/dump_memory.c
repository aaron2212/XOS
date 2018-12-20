#include "../../libc/include/size_t.h"

#include "debug.h"

void dump_memory(void* ptr, size_t size)
{
    unsigned int line_len = 0;
    char* ptr2 = (char*) ptr;

    for (unsigned int i=0; i<size; i++)  {
        unsigned int /*j,*/ len = 0;
        char* str = "";

        // if (!((line_len+len) >= 80)) {
        printf("%x ", *(ptr2+i));
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

    printf("\n\n");
}