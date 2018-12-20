global inb
global outb

	; === outb ===
	; stack: [esp+8] the data byte
	;        [esp+4] the I/O port
	;        [esp  ] return address

	; === inb ===
	; stack: [esp+4] the I/O port
	; 		 [esp  ] return address

inb:
	mov dx, [esp+4]
	in al, dx
	ret

outb:
	mov al, [esp+8]    ; move the data to be sent into the al register
    mov dx, [esp+4]    ; move the address of the I/O port into the dx register
    out dx, al         ; send the data to the I/O port
    ret                ; return to the calling function
