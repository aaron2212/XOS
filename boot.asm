[bits 32]

MAGIC equ 0x1BADB002			; magic number for multiboot
FLAGS equ (1<<0 | 1<<1)
CHECKSUM equ -(MAGIC+FLAGS)

section .multiboot

align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .text

%include "include/gdt/gdt.asm"
%include "include/idt/idt.asm"
%include "include/isr_irq/isrs_irqs.asm"

global start
extern k_main

start:
	mov esp, stack_space
	push ebx					; pass multiboot structure to kernel_main

	call k_main
	hlt

section .bss

resb 65536

stack_space:
