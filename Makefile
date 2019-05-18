OBJECTS := lib/*.o
KERNEL_OBJECTS := objs/*.o

CC 	:= gcc
CFLAGS  := -m32 -c -g -nostdlib -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra
LDFLAGS := -T link.ld -m elf_i386
AS	:= nasm
ASFLAGS := -f elf

all: prepare boot _kernel run

prepare:
	@clear
	@mkdir -p lib
	@mkdir -p objs

boot:
	@$(AS) $(ASFLAGS) boot.asm
	@$(AS) $(ASFLAGS) include/io.s
	@mv include/io.o lib/io.o

_kernel:
	@$(CC) $(CFLAGS) kernel/kernel/kernel.c			-o kernel.o
	@$(CC) $(CFLAGS) kernel/debug/dump_memory.c		-o objs/dump_memory.o
	@$(CC) $(CFLAGS) kernel/include/kbd.c			-o lib/kbd.o
	@$(CC) $(CFLAGS) kernel/fs/vfs.c				-o objs/vfs.o
	@$(CC) $(CFLAGS) kernel/fs/sfs.c				-o objs/sfs.o
	@$(CC) $(CFLAGS) kernel/fs/fs.c					-o objs/fs.o
	@$(CC) $(CFLAGS) kernel/errors/errors.c			-o objs/errors.o
	@$(CC) $(CFLAGS) kernel/system/system.c			-o objs/system.o
	@$(CC  $(CFLAGS) kernel/system/users.c			-o objs/users.o
	@$(CC) $(CFLAGS) include/gdt/gdt.c				-o objs/gdt.o
	@$(CC) $(CFLAGS) include/idt/idt.c				-o objs/idt.o
	@$(CC) $(CFLAGS) include/isr_irq/isrs.c			-o objs/isrs.o
	@$(CC) $(CFLAGS) include/isr_irq/irqs.c			-o objs/irqs.o
	@$(CC) $(CFLAGS) include/pit/pit.c				-o objs/pit.o
	@$(CC) $(CFLAGS) libc/include/string/strcat.c	-o lib/strcat.o
	@$(CC) $(CFLAGS) libc/include/string/strncat.c	-o lib/strncat.o
	@$(CC) $(CFLAGS) libc/include/string/strcpy.c	-o lib/strcpy.o
	@$(CC) $(CFLAGS) libc/include/string/strncpy.c	-o lib/strncpy.o
	@$(CC) $(CFLAGS) libc/include/string/strlen.c	-o lib/strlen.o
	@$(CC) $(CFLAGS) libc/include/string/strcmp.c	-o lib/strcmp.o
	@$(CC) $(CFLAGS) libc/include/string/strncmp.c  -o lib/strncmp.o
	@$(CC) $(CFLAGS) libc/include/string/strrev.c	-o lib/strrev.o
	@$(CC) $(CFLAGS) libc/include/string/strchr.c	-o lib/strchr.o
	@$(CC) $(CFLAGS) libc/include/string/memcmp.c	-o lib/memcmp.o
	@$(CC) $(CFLAGS) libc/include/string/memcpy.c	-o lib/memcpy.o
	@$(CC) $(CFLAGS) libc/include/string/memset.c	-o lib/memset.o
	@$(CC) $(CFLAGS) libc/include/string/strtok.c	-o lib/strtok.o
	@$(CC) $(CFLAGS) libc/include/stdio/putc.c		-o lib/putc.o
	@$(CC) $(CFLAGS) libc/include/stdio/puts.c		-o lib/puts.o
	@$(CC) $(CFLAGS) libc/include/stdio/kprintf.c	-o lib/kprintf.o
	@$(CC) $(CFLAGS) libc/include/stdio/vprintf.c	-o lib/vprintf.o
	@$(CC) $(CFLAGS) libc/include/stdio/getchar.c	-o lib/getchar.o
	@$(CC) $(CFLAGS) libc/include/stdio/gets.c		-o lib/gets.o
	@$(CC) $(CFLAGS) libc/include/stdio/fopen.c		-o lib/fopen.o
	@$(CC) $(CFLAGS) libc/include/stdio/fread.c		-o lib/fread.o
	@$(CC) $(CFLAGS) libc/include/stdio/feof.c		-o lib/feof.o
	@$(CC) $(CFLAGS) libc/include/stdio/fgetc.c		-o lib/fgetc.o
	@$(CC) $(CFLAGS) libc/include/stdio/ftell.c		-o lib/ftell.o
	@$(CC) $(CFLAGS) libc/include/stdio/fseek.c		-o lib/fseek.o
	@$(CC) $(CFLAGS) libc/include/stdio/fclose.c	-o lib/fclose.o
	@$(CC) $(CFLAGS) libc/include/stdlib/itoa.c		-o lib/itoa.o
	@$(CC) $(CFLAGS) libc/include/stdlib/kmalloc.c	-o lib/kmalloc.o
	@$(CC) $(CFLAGS) libc/include/stdlib/kcalloc.c	-o lib/kcalloc.o
	@$(CC) $(CFLAGS) libc/include/stdlib/kfree.c	-o lib/kfree.o
	@$(CC) $(CFLAGS) libc/include/stdlib/krealloc.c	-o lib/krealloc.o
	@$(CC) $(CFLAGS) libc/include/math/ceil.c		-o lib/ceil.o

	ld $(LDFLAGS) boot.o kernel.o $(KERNEL_OBJECTS) $(OBJECTS) -o kernel.bin

os.iso: kernel.bin
	@cp kernel.bin iso/boot/kernel.bin
	@genisoimage -R								\
				-b boot/grub/stage2_eltorito	\
				-no-emul-boot					\
				-boot-load-size 4				\
				-A os							\
				-input-charset utf8				\
				-quiet							\
				-boot-info-table				\
				-o os.iso						\
				iso

run: os.iso
	@qemu-system-i386 -m 1024M -cdrom os.iso


#%.o: %.c
#	$(CC) $(CFLAGS)  $< -o $@
#
#%.o: %.s
#	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.bin os.iso
