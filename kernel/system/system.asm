global execute_syscall

execute_syscall:
    push ebp
    mov ebp, esp

    mov eax, [ebp+8]    ; load the function pointer from the first argument
    push dword [ebp+52] ; edx
    push dword [ebp+56] ; ecx
    push dword [ebp+48] ; ebx
    call eax

    mov esp, ebp
    pop ebp

    ret 12