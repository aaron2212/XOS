#include <stdint.h>
#include <stdarg.h>
#include "isrs.h"

extern int kprintf(const char *fmt, ...);
extern void idt_set_gate(uint32_t i, uint64_t base, uint16_t selector, uint8_t flags);
unsigned char outb(uint16_t port, uint8_t val);
unsigned char inb(uint16_t port);

void init_isrs()
{
	idt_set_gate(0, (uint32_t) isr0, 0x08, 0x8E);
	idt_set_gate(1, (uint32_t) isr1, 0x08, 0x8E);
	idt_set_gate(2, (uint32_t) isr2, 0x08, 0x8E);
	idt_set_gate(3, (uint32_t) isr3, 0x08, 0x8E);
	idt_set_gate(4, (uint32_t) isr4, 0x08, 0x8E);
	idt_set_gate(5, (uint32_t) isr5, 0x08, 0x8E);
	idt_set_gate(6, (uint32_t) isr6, 0x08, 0x8E);
	idt_set_gate(7, (uint32_t) isr7, 0x08, 0x8E);
	idt_set_gate(8, (uint32_t) isr8, 0x08, 0x8E);
	idt_set_gate(9, (uint32_t) isr9, 0x08, 0x8E);
	idt_set_gate(10, (uint32_t) isr10, 0x08, 0x8E);
	idt_set_gate(11, (uint32_t) isr11, 0x08, 0x8E);
	idt_set_gate(12, (uint32_t) isr12, 0x08, 0x8E);
	idt_set_gate(13, (uint32_t) isr13, 0x08, 0x8E);
	idt_set_gate(14, (uint32_t) isr14, 0x08, 0x8E);
	idt_set_gate(15, (uint32_t) isr15, 0x08, 0x8E);
	idt_set_gate(16, (uint32_t) isr16, 0x08, 0x8E);
	idt_set_gate(17, (uint32_t) isr17, 0x08, 0x8E);
	idt_set_gate(18, (uint32_t) isr18, 0x08, 0x8E);
	idt_set_gate(19, (uint32_t) isr19, 0x08, 0x8E);
	idt_set_gate(20, (uint32_t) isr20, 0x08, 0x8E);
	idt_set_gate(21, (uint32_t) isr21, 0x08, 0x8E);
	idt_set_gate(22, (uint32_t) isr22, 0x08, 0x8E);
	idt_set_gate(23, (uint32_t) isr23, 0x08, 0x8E);
	idt_set_gate(24, (uint32_t) isr24, 0x08, 0x8E);
	idt_set_gate(25, (uint32_t) isr25, 0x08, 0x8E);
	idt_set_gate(26, (uint32_t) isr26, 0x08, 0x8E);
	idt_set_gate(27, (uint32_t) isr27, 0x08, 0x8E);
	idt_set_gate(28, (uint32_t) isr28, 0x08, 0x8E);
	idt_set_gate(29, (uint32_t) isr29, 0x08, 0x8E);
	idt_set_gate(30, (uint32_t) isr30, 0x08, 0x8E);
	idt_set_gate(31, (uint32_t) isr31, 0x08, 0x8E);
}

char *exception_message[] =
{
	"Division By Zero",								// 0
	"Debug Exception",								// 1
	"Non Maskable Interrupt Exception",				// 2
	"Breakpoint Exception",							// 3
	"Into Detected Overflow Exception",				// 4
	"Out of Bounds Exception",						// 5
	"Invalid Opcode Exception",						// 6
	"No Coprocessor Exception",						// 7
	"Double Fault Exception",						// 8
	"Coprocessor Segment Overrun Exception",		// 9
	"Bad TSS Exception",							// 10
	"Segment Not Present Exception",				// 11
	"Stack Fault Exception",						// 12
	"General Protection Fault Exception",			// 13
	"Page Fault Exception",							// 14
	"Unknown Interrupt Exception",					// 15
	"Coprocessor Fault Exception",					// 16
	"Alignment Check Exception",					// 17
	"Machine Check Exception",						// 18
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

void dump_registers(struct regs r)
{
	kprintf("eax=0x%x\n", r.eax);
	kprintf("ebx=0x%x\n", r.ebx);
	kprintf("ecx=0x%x\n", r.ecx);
	kprintf("edx=0x%x\n", r.edx);
	kprintf("ebp=0x%x\n", r.ebp);
	kprintf("esp=0x%x\n", r.esp);
	kprintf("edi=0x%x\n", r.edi);
	kprintf("esi=0x%x\n", r.esi);
	kprintf("eip=0x%x\n", r.eip);
	kprintf("eflags=0x%x\n", r.eflags);
	kprintf("useresp=0x%x\n", r.useresp);
	kprintf("ss=0x%x\n", r.ss);
	kprintf("cs=0x%x\n", r.cs);
	kprintf("ds=0x%x\n", r.ds);
	kprintf("fs=0x%x\n", r.fs);
	kprintf("es=0x%x\n", r.es);
	kprintf("gs=0x%x\n", r.gs);
}

void isr_fault_handler(struct regs r)
{
	if (r.int_no < 32)
	{
		/* display description of exception that has occured */
		kprintf("Caught exception 0x%x [%s]. Dumping registers\n", r.int_no, exception_message[r.int_no]);
		dump_registers(r);
	}
}