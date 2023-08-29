section .data
minus_sign db '-'
section .bss
    digit resb 100
    digitPos resb 8
section .text
    global _start
_start:
    push rbp
    mov rbp,rsp
    mov qword [rbp+16],11
    mov rax, 7,
    add rax,[rbp+16]
    mov [rbp+24],rax
    mov rax, 2,
    add rax,[rbp+24]
    mov [rbp+32],rax
    mov rax,[rbp+32]
    mov [rbp+40],rax
    mov rax, 4,
    add rax,[rbp+40]
    mov [rbp+48],rax
    mov rax, 33,
    add rax,[rbp+48]
    mov [rbp+56],rax
    mov rax, 57,
    sub [rbp+56],rax
    mov rax,[rbp+56]
    mov [rbp+64],rax
    mov rax,[rbp+64]
    mov [rbp+8],rax
    mov rax, qword [rbp+8]
    call _print_number
    call _print_newline
    mov rax,60
    mov rdi, 0
    syscall
_print_number:
    mov rbx,rax
    push rbx
    test rax,rax
    js _is_negative
    _jmp_back:
    mov rcx,digit
    mov rbx,10
    mov [rcx],rbx
    inc rcx
    mov [digitPos],rcx
_get_number_loop:
    mov rdx,0
    mov rbx,10
    div rbx
    push rax
    add rdx,48
    mov rcx,[digitPos]
    mov [rcx],dl
    inc rcx
    mov [digitPos],rcx
    pop rax
    cmp rax,0
    jne _get_number_loop
_print_number_loop:
    mov rcx, [digitPos]
    mov rax,1
    mov rdi,1
    mov rsi,rcx
    mov rdx,1
    syscall
    mov rcx, [digitPos]
    dec rcx
    mov [digitPos],rcx
    cmp rcx,digit
    jge _print_number_loop
    pop rbx
    mov rax,rbx
    ret
_is_negative:
    push rax
    mov rcx, [digitPos]
    mov rax, 1
    mov rdi, 1
    mov rsi, minus_sign
    mov rdx, 1
    syscall
    pop rax
    neg rax
    jmp _jmp_back
_generate_number:
    xor rax,rax
    xor rbx,rbx
_get_next_digit:
    movzx rcx,byte [rsi]
    cmp rcx,10
    je _return
    sub rcx,48
    imul rax,rax,10
    add rax,rcx
    inc rsi
    inc rbx
    cmp rbx,10
    jl _get_next_digit
_return:
    ret
_print_newline:
    mov rcx, 0x0a
    mov rax,1
    mov rdi,1
    mov rsi,rcx
    mov rdx,1
    syscall
    ret
