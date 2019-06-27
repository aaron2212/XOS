global install_gdt
extern gdt_ptr

install_gdt:
	lgdt [gdt_ptr]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	jmp 0x08:flush2

flush2:
	ret

; global _init_gdt

; _init_gdt:
; 	cli
; 	pusha
; 	lgdt [gdt]
; 	sti
; 	popa
; 	ret

; ; null descriptor

; gdt_start:
; 	dd 0x00
; 	dd 0x00

; 	; kernel mode code descriptor: indicates readable and writable segment, and is at ring 0
; 	dw 0xFFFF		; limit low: segment limit; each segment is 64K
; 	dw 0x00			; base low (starting address of segment): code selector can access every byte between 0x00 and 0xFFFF
; 	db 0x00			; base middle and base low is 3 bytes (bit 0-23 of base address)
; 	db 10011010b	; access byte: bits 40-47 (change bits 40 and 47 when using virtual memory)
; 	db 11001111b	; granularity bit: bits 48-55
; 	db 0x00			; base high (bits 24-32 of the base address)

; 	; kernel mode data descriptor except for bit 43, indicating this is a code descriptor
; 	dw 0xFFFF		; limit low
; 	dw 0x00			; base low
; 	db 0x00			; base middle
; 	db 10010010b	; access byte
; 	db 11001111b	; granularity byte
; 	db 0x00			; base high

; 	; user mode code descriptor: indicates readable and writable segment, and is at ring 3
; 	dw 0xFFFF		; limit low: segment limit; each segment is 64K
; 	dw 0x00			; base low (starting address of segment): code selector can access every byte between 0x00 and 0xFFFF
; 	db 0x00			; base middle and base low is 3 bytes (bit 0-23 of base address)
; 	db 11111010b	; access byte: bits 40-47 (change bits 40 and 47 when using virtual memory)
; 	db 11001111b	; granularity bit: bits 48-55
; 	db 0x00			; base high (bits 24-32 of the base address)

; 	; ; kernel mode data descriptor except for bit 43, indicating this is a code descriptor
; 	; dw 0xFFFF		; limit low
; 	; dw 0x00			; base low
; 	; db 0x00			; base middle
; 	; db 11110010b	; access byte
; 	; db 11001111b	; granularity byte
; 	; db 0x00			; base high

; gdt_end:

; gdt:
; 	dw gdt_end - gdt_start - 1 ; limit (size of GDT)
; 	dd gdt_start			   ; base address of GDT