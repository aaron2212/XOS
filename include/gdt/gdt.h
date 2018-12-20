#ifndef _GDT_H
#define _GDT_H

#define MAX_DESCRIPTORS 3

/* function in boot.asm
 * reloads segment registers
 */

void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t granularity);
extern int _init_gdt();
void init_gdt();

#endif
