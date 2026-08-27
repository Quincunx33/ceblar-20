BITS 32
section .text
global context_switch
context_switch:
    mov eax,[esp+4]
    mov [eax+44],esp
    mov [eax+40],ebp
    mov [eax+20],ebx
    mov [eax+36],esi
    mov [eax+32],edi
    mov eax,[esp+8]
    mov esp,[eax+44]
    mov ebp,[eax+40]
    mov ebx,[eax+20]
    mov esi,[eax+36]
    mov edi,[eax+32]
    ret
section .note.GNU-stack noalloc noexec nowrite progbits
