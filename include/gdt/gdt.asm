global _init_gdt

_init_gdt:
	cli
	pusha
	lgdt [gdt]
	sti
	popa
	ret

; null descriptor

gdt_start:
	dd 0x00
	dd 0x00

; code descriptor: indicates readable and writable segment, and is at ring 0

	dw 0xFFFF		; limit low: segment limit; each segment is 64K
	dw 0x00			; base low (starting address of segment): code selector can access every byte between 0x00 and 0xFFFF
	db 0x00			; base middle and base low is 3 bytes (bit 0-23 of base address)
	db 10011010b	; access byte: bits 40-47 (change bits 40 and 47 when using virtual memory)
	db 11001111b	; granularity bit: bits 48-55
	db 0x00			; base high (bits 24-32 of the base address)

; same as code descriptor, except for bit 43, indicating this is a code descriptor

	dw 0xFFFF		; limit low
	dw 0x00			; base low
	db 0x00			; base middle
	db 10010010b	; access byte
	db 11001111b	; granularity byte
	db 0x00			; base high

gdt_end:

gdt:
	dw gdt_end - gdt_start - 1 ; limit (size of GDT)
	dd gdt_start			   ; base address of GDT