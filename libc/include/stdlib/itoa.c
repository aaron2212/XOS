#include "stdlib.h"
#include <stdbool.h>

extern char *strrev(char* str);

char itoa(int value, char* str, int base)
{
    int i = 0;
    //bool is_negative;
    
    if (value == 0)
    {
        str[i++] = 0;
        return (int) str;
    }
    
    if (value < 0 && base == 10)
    {
        //is_negative = true;
        value = -value;
    }
    
    while (value != 0)
    {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem-10) + 'a' : rem + '0';
        value /= base;
    }
    
    str[i] = '\0';
    
    strrev(str);
    
    return (int) str;
}
