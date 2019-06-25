#include "system.h"

void enter_usermode()
{
    asm(
        "cli\n"
        "mov $0x23, %ax\n" // User mode data selector is 0x20. Add 3 for user mode (ring 3)
        "mov %ax, %ds\n"
        "mov %ax, %es\n"
        "mov %ax, %fs\n"
        "mov %ax, %gs\n"
        ""
        "mov %esp, %eax\n"
        "pushl $0x23\n"
        "pushl %eax\n"
        "pushf\n"
        "pushl $0x1b\n"
        "push $1f\n"
        "iret\n"
        "1:\n"
        ""
    );
}