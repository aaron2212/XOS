#ifndef _ISRS_H
#define _ISRS_H

#include <stdint.h>

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

struct regs
{
	uint32_t gs; // segment registers
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	uint32_t reserved; // don't know what this is?
	uint32_t edi;	  // general purpose registers
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx; // 48 bytes
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t err_code; // push error code
	uint32_t int_no;   // push byte #
	uint32_t eip;	  // pushed by CPU automatically
	uint32_t cs;	   // pushed by CPU automatically
	uint32_t eflags;   // pushed by CPU automatically
	uint32_t useresp;  // pushed by CPU automatically
	uint32_t ss;	   // pushed by CPU automatically
} __attribute__((packed));

extern void syscall();
extern void handle_syscall(struct regs r);

void init_isrs();
void isr_fault_handler(struct regs r);
void dump_registers(struct regs r);

#endif
