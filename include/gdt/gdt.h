#ifndef _GDT_H
#define _GDT_H

#define MAX_DESCRIPTORS 6

/* function in boot.asm
 * reloads segment registers
 */

struct tss_entry
{
    uint32_t prevTss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap;
} __attribute__((packed));

struct tss_entry tss_entry;

void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t granularity);

void init_gdt();
void install_tss(uint32_t i, uint16_t kernel_ss, uint16_t kernel_esp);
void set_kernel_stack(uint32_t esp);

extern void install_gdt();

#endif