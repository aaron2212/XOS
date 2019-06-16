#include "system.h"

// Convert values coded in BCD to binary (integer) numbers
int bcd2bin(int bcd)
{
    return ((bcd & 0xF0) >> 1) + ( (bcd & 0xF0) >> 3) + (bcd & 0xf);
}