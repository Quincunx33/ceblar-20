BITS 32
section .text

global isr0
isr0:
    push dword 0
    push dword 0
    jmp isr_common

global isr1
isr1:
    push dword 0
    push dword 1
    jmp isr_common

global isr2
isr2:
    push dword 0
    push dword 2
    jmp isr_common

global isr3
isr3:
    push dword 0
    push dword 3
    jmp isr_common

global isr4
isr4:
    push dword 0
    push dword 4
    jmp isr_common

global isr5
isr5:
    push dword 0
    push dword 5
    jmp isr_common

global isr6
isr6:
    push dword 0
    push dword 6
    jmp isr_common

global isr7
isr7:
    push dword 0
    push dword 7
    jmp isr_common

global isr8
isr8:
    push dword 8
    jmp isr_common

global isr9
isr9:
    push dword 0
    push dword 9
    jmp isr_common

global isr10
isr10:
    push dword 10
    jmp isr_common

global isr11
isr11:
    push dword 11
    jmp isr_common

global isr12
isr12:
    push dword 12
    jmp isr_common

global isr13
isr13:
    push dword 13
    jmp isr_common

global isr14
isr14:
    push dword 14
    jmp isr_common

global isr15
isr15:
    push dword 0
    push dword 15
    jmp isr_common

global isr16
isr16:
    push dword 0
    push dword 16
    jmp isr_common

global isr17
isr17:
    push dword 17
    jmp isr_common

global isr18
isr18:
    push dword 0
    push dword 18
    jmp isr_common

global isr19
isr19:
    push dword 0
    push dword 19
    jmp isr_common

global isr20
isr20:
    push dword 0
    push dword 20
    jmp isr_common

global isr21
isr21:
    push dword 0
    push dword 21
    jmp isr_common

global isr22
isr22:
    push dword 0
    push dword 22
    jmp isr_common

global isr23
isr23:
    push dword 0
    push dword 23
    jmp isr_common

global isr24
isr24:
    push dword 0
    push dword 24
    jmp isr_common

global isr25
isr25:
    push dword 0
    push dword 25
    jmp isr_common

global isr26
isr26:
    push dword 0
    push dword 26
    jmp isr_common

global isr27
isr27:
    push dword 0
    push dword 27
    jmp isr_common

global isr28
isr28:
    push dword 0
    push dword 28
    jmp isr_common

global isr29
isr29:
    push dword 0
    push dword 29
    jmp isr_common

global isr30
isr30:
    push dword 0
    push dword 30
    jmp isr_common

global isr31
isr31:
    push dword 0
    push dword 31
    jmp isr_common

global irq0
irq0:
    push dword 0
    push dword 32
    jmp isr_common

global irq1
irq1:
    push dword 0
    push dword 33
    jmp isr_common

global irq2
irq2:
    push dword 0
    push dword 34
    jmp isr_common

global irq3
irq3:
    push dword 0
    push dword 35
    jmp isr_common

global irq4
irq4:
    push dword 0
    push dword 36
    jmp isr_common

global irq5
irq5:
    push dword 0
    push dword 37
    jmp isr_common

global irq6
irq6:
    push dword 0
    push dword 38
    jmp isr_common

global irq7
irq7:
    push dword 0
    push dword 39
    jmp isr_common

global irq8
irq8:
    push dword 0
    push dword 40
    jmp isr_common

global irq9
irq9:
    push dword 0
    push dword 41
    jmp isr_common

global irq10
irq10:
    push dword 0
    push dword 42
    jmp isr_common

global irq11
irq11:
    push dword 0
    push dword 43
    jmp isr_common

global irq12
irq12:
    push dword 0
    push dword 44
    jmp isr_common

global irq13
irq13:
    push dword 0
    push dword 45
    jmp isr_common

global irq14
irq14:
    push dword 0
    push dword 46
    jmp isr_common

global irq15
irq15:
    push dword 0
    push dword 47
    jmp isr_common

global isr128
isr128:
    push dword 0
    push dword 128
    jmp isr_common

extern interrupt_dispatch
isr_common:
    pusha
    push esp
    call interrupt_dispatch
    add esp, 4
    mov esp,eax
    popa
    add esp, 8
    iretd

section .note.GNU-stack noalloc noexec nowrite progbits
