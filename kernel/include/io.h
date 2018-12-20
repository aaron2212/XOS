#ifndef _IO_H
#define _IO_H

#include "../../libc/include/stdint.h"

unsigned char inb(uint16_t port);
unsigned char outb(uint16_t port, uint8_t val);

#endif