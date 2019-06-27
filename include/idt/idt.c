#include <stdint.h>
#include "../../libc/include/stdlib/stdlib.h"
#include "idt.h"

extern int kprintf(const char *fmt, ...);
extern void *memset(void *ptr, int value, size_t size);

int interrupt_number;

struct idt_descriptor {
	uint16_t base_low;			/* first 16 bits of the base address of the IR */
	uint16_t selector;			/* segment selector */
	uint8_t  reserved;			/* reserved: always 0 */
	uint8_t  flags;				/* bits 39-41: flags, bits 42-45: DPL */
	uint16_t base_high;			/* bits 16-31 of the base address of the IR */
} __attribute__ ((packed));

struct idt_pointer {
	uint16_t limit;				/* size of the IDT */
	uint32_t base;				/* base address of the IDT */
} __attribute__ ((packed));

struct idt_descriptor idt_desc[MAX_INTERRUPTS];
struct idt_pointer idt_p;

/* default handler to catch unhandled system interrupts */

void interrupt_handler(void)
{
	kprintf("Unhandled exception ");

	while (1) {

	}
}

void set_interrupt_number(int num)
{
	interrupt_number = num;
}

void idt_set_gate(uint32_t i, uint64_t base, uint16_t selector, uint8_t flags)
{
	if (i > MAX_INTERRUPTS)
		return;

	/* The interrupt routine's base address */
    idt_desc[i].base_low 	= (base & 0xFFFF);
    idt_desc[i].base_high 	= (base >> 16) & 0xFFFF;

    /* The segment or 'selector' that this IDT entry will use
     * is set here, along with any access flags */
    idt_desc[i].selector 	= selector;
    idt_desc[i].reserved 	= 0;
    idt_desc[i].flags 		= flags;
}

void init_idt(void)
{
	int size = sizeof(idt_desc) * MAX_INTERRUPTS - 1;

	idt_p.limit = size;
	idt_p.base  = (uint32_t) &idt_desc;

	/* null the entire IDT */
	memset(&idt_desc, 0, sizeof(idt_desc) * MAX_INTERRUPTS);

	/* install the new IDT */
	_init_idt();
}
