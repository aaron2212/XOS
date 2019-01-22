#include "../../libc/include/stdint.h"
#include "pit.h"

extern int kprintf(const char* fmt, ...);
extern unsigned char outb(unsigned short port, unsigned char val);

unsigned int ticks = 0;

// static void timer_callback(struct regs regs)
// {
// 	ticks++;

// 	kprintf("Tick: %d\n", ticks);
// }

// void init_timer(int frequency)
// {
// 	irq_install_handler(0, &timer_callback);

// 	unsigned int divisor = 1193180 / frequency;

// 	outb(0x43, 0x36);

// 	char low  = (char) divisor & 0xFF;
// 	char high = (char) (divisor >> 8) & 0xFF;

// 	outb(0x40, low);
// 	outb(0x40, high);
// }

// extern int kprintf(const char* fmt, ...);
// extern void irq_install_handler(int irq, void (*handler)(struct regs *s));
// unsigned char outb(uint16_t port, uint8_t val);
// unsigned char inb(uint16_t port);

// unsigned int timer_ticks = 0;		// Keep track of number of ticks that the system has been running for

// /*
//  * Increment timer_ticks every time the timer fires.
//  * The timer fires 18.222 times per second
// */

// void timer_handler()
// {
// 	timer_ticks++;

// 	/* Every 18 clock ticks (~1 second) display a message */

// 	if (timer_ticks % 18 == 0)
// 	{
// 		kprintf("One second has passed");
// 	}
// }

// /*
//  * Set the system clock by installing
//  * the timer interrupt into IRQ0
// */

// void timer_install()
// {
//     irq_install_handler(0, timer_handler);
// }

// /*
//  * This will loop until the
//  * given time has been reached
//  */

// void timer_wait(unsigned int ticks)
// {
// 	unsigned int eticks;

// 	eticks = timer_ticks + ticks;

// 	while (timer_ticks < eticks);
// }