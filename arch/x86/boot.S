section .multiboot
align 8
mb2_header:
    dd 0xe85250d6
    dd 0
    dd mb2_end - mb2_header
    dd -(0xe85250d6 + 0 + (mb2_end - mb2_header))
    dw 0
    dw 0
    dd 8
mb2_end:

section .bss
align 16
stack_bottom: resb 16384
stack_top:

section .text
global _start
extern kmain
_start:
    cli
    mov esp, stack_top
    push ebx
    push eax
    call kmain
.hang:
    cli
    hlt
    jmp .hang

section .note.GNU-stack noalloc noexec nowrite progbits
