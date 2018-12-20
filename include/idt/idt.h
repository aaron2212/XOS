#ifndef _IDT_H
#define _IDT_H

#define MAX_INTERRUPTS 256

extern void _init_idt(void);
extern int _generate_interrupt(int num);
void init_idt(void);
void interrupt_handler(void);
void generate_interrupt(int num);
void idt_set_gate(uint32_t i, uint64_t base, uint16_t selector, uint8_t flags);

#endif
