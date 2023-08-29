section .bss
    digit resb 100
    digitPos resb 8
section .text
    global _start
_start:
    push rbp
    mov rbp,rsp
    mov rax, 0
    cmp rax,0
