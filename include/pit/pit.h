#ifndef _PIT_H
#define _PITH_

struct regs {
	uint32_t gs, fs, es, ds;							// push segments last
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;	// pushed by 'pusha'
	uint32_t int_no, err_code;							// 'push byte #' and error code
	uint32_t eip, cs, eflags, useresp, ss;				// pushed by CPU automatically
};

void init_timer(int frequency);

// void timer_handler();
// void timer_install();
// void timer_wait(unsigned int ticks);

#endif