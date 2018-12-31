#include <stdint.h>
#include "irqs.h"

extern unsigned char outb(uint16_t port, uint8_t val);
extern void idt_set_gate(uint32_t i, uint64_t base, uint16_t selector, uint8_t flags);

void* irq_routines[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_handler(int irq, void (*handler)(/*struct regs *r*/))
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
	irq_routines[irq] = 0;
}

/* Master PIC uses port 0x20
 * and slave uses port 0xA0
 * remapping IRQ0-15 to IDT
 * entries 32-47
*/

void irq_remap(void)
{
    outb(0x20, 0x11); // 17
    outb(0xA0, 0x11); // 17
    outb(0x21, 0x20); // 32
    outb(0xA1, 0x28); // 40
    outb(0x21, 0x04); // 4
    outb(0xA1, 0x02); // 2
    outb(0x21, 0x01); // 1
    outb(0xA1, 0x01); // 1
    outb(0x21, 0x0);  // 0
    outb(0xA1, 0x0);  // 0
}

void init_irqs(void)
{
	irq_remap();

	idt_set_gate(32, (unsigned) irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned) irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned) irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned) irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned) irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned) irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned) irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned) irq7, 0x08, 0x8E);

	idt_set_gate(40, (unsigned) irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned) irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned) irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned) irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned) irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned) irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned) irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned) irq15, 0x08, 0x8E);
}

/*
 * Need to send End of Interrupt (0x20)
 * to tell the IRQ controllers we are done
 * EOI needs to be sent to both controllers
*/

void irq_fault_handler(struct regs *r)
{
    /* This is a blank function pointer */
    void (*handler)(struct regs *r);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no-32];

    if (handler)
    {
    	handler(r);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outb(0x20, 0x20);
}
