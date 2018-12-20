#include <stdint.h>
#include "../../libc/include/stdlib/stdlib.h"
#include "gdt.h"

extern void* memset(void *ptr, int value, size_t size);

/* defines a GDT entry */

struct gdt_descriptor {
	uint16_t limit_low;			/* bits 0-15 of segment limit */
	uint16_t base_low;			/* bits 0-23 */
	uint8_t base_middle;		/* of base address */
	uint8_t access;				/* access byte */
	uint8_t granularity;			/* granularity */
	uint8_t base_high;			/* bits 24-32 of base address */
} __attribute__ ((packed));

/* defined GDT pointer including limit:
 * the max bytes taken up by the GDT - 1
*/

struct gdt_pointer {
	uint16_t limit;				/* size of GDT */
	uint32_t base;				/* base address of GDT */
} __attribute__ ((packed));

struct gdt_descriptor	   gdt_desc[MAX_DESCRIPTORS];
struct gdt_pointer 	   gdt_p;

void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t granularity)
{
	/* null out the descriptor */

	memset((void *) gdt_desc, 0, sizeof(gdt_desc));

	/* Setup the descriptor base address */
    gdt_desc[i].base_low     = (base & 0xFFFF);
    gdt_desc[i].base_middle  = (base >> 16) & 0xFF;
    gdt_desc[i].base_high    = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt_desc[i].limit_low    = (limit & 0xFFFF);
    gdt_desc[i].granularity  = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt_desc[i].granularity |= (granularity & 0xF0);
    gdt_desc[i].access       = access;
}

void init_gdt()
{
	gdt_p.limit = (sizeof(gdt_desc) * MAX_DESCRIPTORS) - 1;
	gdt_p.base  = (uint32_t) &gdt_desc;

	/* set null descriptor */

	gdt_set_descriptor(0, 0, 0, 0, 0);

	/* set code descriptor */

	gdt_set_descriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	/* set data descriptor */

	gdt_set_descriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	/* install the GDT */

	_init_gdt();
}
