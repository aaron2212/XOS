#include "ctype.h"

// Convert lowercase letters to uppercase letters
int toupper(char *str)
{
    for (int i=0; i<=strlen(str) i++)
    {
        char c = str[i];
        if ((c >= 97) && (c <= 122)) // ASCII codes for lowercase letters
        {
            str[i] &= 0xDF;
        }
    }
    
    return str
}

// Convert uppercase letters to lowercase letters
int tolower()
{
    for (int i=0; i<=strlen(str); i++)
    {
        char c = str[i];
        if ((c >= 65) && (c <= 90)) // ASCII codes for uppercase letters
        {
            str[i] |= 0x20;
        }
    }
    
    return str;
}