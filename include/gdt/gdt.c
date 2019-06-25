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

struct gdt_descriptor		gdt_desc[MAX_DESCRIPTORS];
struct gdt_pointer			gdt_ptr;

void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t granularity)
{
	/* null out the descriptor */
	memset(&gdt_desc[i], 0, sizeof(gdt_desc));

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
	gdt_ptr.limit = (sizeof(struct gdt_descriptor) * MAX_DESCRIPTORS) - 1;
	gdt_ptr.base  = (uint32_t) &gdt_desc;

	gdt_set_descriptor(0, 0, 0, 0, 0); // Null descriptor
	gdt_set_descriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code descriptor
	gdt_set_descriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data descriptor
	gdt_set_descriptor(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code descriptor
	gdt_set_descriptor(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data descriptor

	// Install the TSS to selector 5
	install_tss(5, 0x10, 0x0);

	/* install the GDT */
	install_gdt();
}

void install_tss(uint32_t i, uint16_t kernel_ss, uint16_t kernel_esp)
{
	uint32_t base = (uint32_t) &tss_entry;
	uint32_t limit = sizeof(tss_entry);
	gdt_set_descriptor(i, base, base+limit, 0x89, 0x00);

	memset(&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0 = kernel_ss;
	tss_entry.esp0 = kernel_esp;

	tss_entry.cs = 0x0b;
	tss_entry.ss = 0x13;
	tss_entry.es = 0x13;
	tss_entry.ds = 0x13;
	tss_entry.fs = 0x13;
	tss_entry.gs = 0x13;

	asm(
		"mov $0x2b, %ax\n"
		"ltr %ax"
	);
}

void set_kernel_stack(uint32_t esp)
{
	tss_entry.esp0 = esp;
}