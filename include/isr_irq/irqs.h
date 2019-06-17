#ifndef _IQRS_H
#define _IRQS_H

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

struct regs {
	uint32_t gs, fs, es, ds;							// push segments last
	uint32_t edi, esi, ebp, ebx, edx, ecx, eax;			// pushed by 'pusha'
	uint32_t err_code, int_no;							// 'push byte #' and error code
	uint32_t eip, cs, eflags;							// pushed by CPU automatically
};

void irq_fault_handler(struct regs *r);
void irq_install_handler(int irq, void (*handler)(/*struct regs *r*/));
void irq_uninstall_handler(int irq);
void init_irqs(void);

// Interrupt handlers
void handle_timer();
void handle_keyboard();
void handle_rtc();

#endif
