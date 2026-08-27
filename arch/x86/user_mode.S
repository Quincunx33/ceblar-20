BITS 32
section .text
global enter_user_mode
enter_user_mode:
    cli
    mov ecx,[esp+4]
    mov edx,[esp+8]
    mov ax,0x23
    mov ds,ax
    mov es,ax
    push dword 0x23
    push edx
    push dword 0x202
    push dword 0x1b
    push ecx
    iretd
section .note.GNU-stack noalloc noexec nowrite progbits
