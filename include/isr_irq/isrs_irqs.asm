global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

extern isr_fault_handler
extern irq_fault_handler

; Divide By Zero Exception

isr0:
	cli
	push byte 0				; a normal ISR stub that pushes a dummy error code to keep a uniform stack frame
	push byte 0
	jmp isr_handler

; Debug Exception

isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_handler

; Non-maskable Interrupt

isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_handler

; Breakpoint

isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_handler

; Overflow

isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_handler

; Bound Range Exceeded

isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_handler

; Invalid opcode

isr6:
	cli
	push byte 0
	push byte 6
	jmp isr_handler

; Device Not Availables

isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_handler

; Double Fault

isr8:
	cli
	push byte 8
	jmp isr_handler

; Coprocessor Segment Overrun

isr9:
	cli
	push byte 0
	push byte 9
	jmp isr_handler

; Invalid TSS

isr10:
	cli
	push byte 10
	jmp isr_handler

; Segment Not Present

isr11:
	cli
	push byte 11
	jmp isr_handler

; Stack Segment Fault

isr12:
	cli
	push byte 12
	jmp isr_handler

; General Protection Fault

isr13:
	cli
	push byte 13
	jmp isr_handler

; Page Fault

isr14:
	cli
	push byte 14
	jmp isr_handler

; Reserved

isr15:
	cli
	push byte 0
	push byte 15
	jmp isr_handler

; x87 Floating-Point Exception

isr16:
	cli
	push byte 0
	push byte 16
	jmp isr_handler

;  Alignment Check

isr17:
	cli
	push byte 17
	jmp isr_handler

;  Machine Check

isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_handler

;  SIMD Floating-Point Exception

isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_handler

;  Virtualization Exception

isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_handler

; Reserved

isr21:
	cli
	push byte 0
	push byte 21
	jmp isr_handler

; Reserved

isr22:
	cli
	push byte 0
	push byte 22
	jmp isr_handler

; Reserved

isr23:
	cli
	push byte 0
	push byte 23
	jmp isr_handler

; Reserved

isr24:
	cli
	push byte 0
	push byte 24
	jmp isr_handler

; Reserved

isr25:
	cli
	push byte 0
	push byte 25
	jmp isr_handler

; Reserved

isr26:
	cli
	push byte 0
	push byte 26
	jmp isr_handler

; Reserved

isr27:
	cli
	push byte 0
	push byte 27
	jmp isr_handler

; Reserved

isr28:
	cli
	push byte 0
	push byte 28
	jmp isr_handler

; Reserved

isr29:
	cli
	push byte 0
	push byte 29
	jmp isr_handler

; Security Exception

isr30:
	cli
	push byte 30
	jmp isr_handler

; Reserved

isr31:
	cli
	push byte 0
	push byte 31
	jmp isr_handler

irq0:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 32
	jmp irq_handler

irq1:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 33
	jmp irq_handler

irq2:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 34
	jmp irq_handler

irq3:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 35
	jmp irq_handler

irq4:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 36
	jmp irq_handler

irq5:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 37
	jmp irq_handler

irq6:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 38
	jmp irq_handler

irq7:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 39
	jmp irq_handler

irq8:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 40
	jmp irq_handler

irq9:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 41
	jmp irq_handler

irq10:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 42
	jmp irq_handler

irq11:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 43
	jmp irq_handler

irq12:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 44
	jmp irq_handler

irq13:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 45
	jmp irq_handler

irq14:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 46
	jmp irq_handler

irq15:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push byte 47
	jmp irq_handler

isr_handler:
	pusha					; push eax, ecx, edx, ebx, esp, ebp, esi, edi
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10			; load the kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	call isr_fault_handler
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8				; cleans up the pushed error code and ISR number
	iret					; pop CS, EIP, EFLAGS, SS, ESP

irq_handler:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10			; load the kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call irq_fault_handler
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8				; cleans up the pushed error code and ISR number
	iret					; pop CS, EIP, EFLAGS, SS, ESP
