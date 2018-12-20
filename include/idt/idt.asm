global _init_idt
extern idt_p

_init_idt:
    lidt [idt_p]
    ret
