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

global syscall

extern isr_fault_handler
extern irq_fault_handler

; Divide By Zero Exception
isr0:
	cli
	push byte 0				; a normal ISR stub that pushes a dummy error code to keep a uniform stack frame
	push 0
	jmp isr_handler

; Debug Exception
isr1:
	cli
	push byte 0
	push 1
	jmp isr_handler

; Non-maskable Interrupt
isr2:
	cli
	push byte 0
	push 2
	jmp isr_handler

; Breakpoint
isr3:
	cli
	push byte 0
	push 3
	jmp isr_handler

; Overflow
isr4:
	cli
	push byte 0
	push 4
	jmp isr_handler

; Bound Range Exceeded
isr5:
	cli
	push byte 0
	push 5
	jmp isr_handler

; Invalid opcode
isr6:
	cli
	push byte 0
	push 6
	jmp isr_handler

; Device Not Availables
isr7:
	cli
	push byte 0
	push 7
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
	push 9
	jmp isr_handler

; Invalid TSS
isr10:
	cli
	push 10
	jmp isr_handler

; Segment Not Present
isr11:
	cli
	push 11
	jmp isr_handler

; Stack Segment Fault
isr12:
	cli
	push 12
	jmp isr_handler

; General Protection Fault
isr13:
	cli
	push 13
	jmp isr_handler

; Page Fault
isr14:
	cli
	push 14
	jmp isr_handler

; Reserved
isr15:
	cli
	push byte 0
	push 15
	jmp isr_handler

; x87 Floating-Point Exception
isr16:
	cli
	push byte 0
	push 16
	jmp isr_handler

;  Alignment Check
isr17:
	cli
	push 17
	jmp isr_handler

;  Machine Check
isr18:
	cli
	push byte 0
	push 18
	jmp isr_handler

;  SIMD Floating-Point Exception
isr19:
	cli
	push byte 0
	push 19
	jmp isr_handler

;  Virtualization Exception
isr20:
	cli
	push byte 0
	push 20
	jmp isr_handler

; Reserved
isr21:
	cli
	push byte 0
	push 21
	jmp isr_handler

; Reserved
isr22:
	cli
	push byte 0
	push 22
	jmp isr_handler

; Reserved
isr23:
	cli
	push byte 0
	push 23
	jmp isr_handler

; Reserved
isr24:
	cli
	push byte 0
	push 24
	jmp isr_handler

; Reserved
isr25:
	cli
	push byte 0
	push 25
	jmp isr_handler

; Reserved
isr26:
	cli
	push byte 0
	push 26
	jmp isr_handler

; Reserved
isr27:
	cli
	push byte 0
	push 27
	jmp isr_handler

; Reserved
isr28:
	cli
	push byte 0
	push 28
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
	push 30
	jmp isr_handler

; Reserved
isr31:
	cli
	push byte 0
	push 31
	jmp isr_handler

irq0:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 32
	jmp irq_handler

irq1:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 33
	jmp irq_handler

irq2:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 34
	jmp irq_handler

irq3:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 35
	jmp irq_handler

irq4:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 36
	jmp irq_handler

irq5:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 37
	jmp irq_handler

irq6:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 38
	jmp irq_handler

irq7:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 39
	jmp irq_handler

irq8:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 40
	jmp irq_handler

irq9:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 41
	jmp irq_handler

irq10:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 42
	jmp irq_handler

irq11:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 43
	jmp irq_handler

irq12:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 44
	jmp irq_handler

irq13:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 45
	jmp irq_handler

irq14:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 46
	jmp irq_handler

irq15:
	cli
	push byte 0				; IRQs don't push error codes: use dummy error code
	push 47
	jmp irq_handler

syscall:
	cli
	push 0x80
	push byte 0
	; mov eax, 0x1
	; mov ebx, 0x2
	; mov ecx, 0x3
	; mov edx, 0x4
	; mov edi, 0x5
	; mov esi, 0x6
	; mov ebp, 0x7
	jmp isr_handler

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
	mov eax, isr_fault_handler
	call eax
	pop eax
	popa
	pop gs
	pop fs
	pop es
	pop ds
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
	mov eax, esp
	push eax
	call irq_fault_handler
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8				; cleans up the pushed error code and ISR number
	iret					; pop CS, EIP, EFLAGS, SS, ESP