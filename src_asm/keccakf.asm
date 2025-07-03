; SPDX-License-Identifier: Apache-2.0

section .note.GNU-stack,"",@progbits
section .text
align 64
keccakf_rndc:
    dq 0x0000000000000001, 0x0000000000008082
    dq 0x800000000000808a, 0x8000000080008000
    dq 0x000000000000808b, 0x0000000080000001
    dq 0x8000000080008081, 0x8000000000008009
    dq 0x000000000000008a, 0x0000000000000088
    dq 0x0000000080008009, 0x000000008000000a
    dq 0x000000008000808b, 0x800000000000008b
    dq 0x8000000000008089, 0x8000000000008003
    dq 0x8000000000008002, 0x8000000000000080
    dq 0x000000000000800a, 0x800000008000000a
    dq 0x8000000080008081, 0x8000000000008080
    dq 0x0000000080000001, 0x8000000080008008

%macro ROTATE_ASSIGN 2
    mov r13, [rdi + %1*8]
    rol rax, %2
    mov [rdi + %1*8], rax
    mov rax, r13
%endmacro

%macro THETA_MICROSTEP 3
    mov rax, %1
    rol rax, 1
    xor rax, %2
    xor [rdi + 8*%3], rax
    xor [rdi + 8*(%3+5)], rax
    xor [rdi + 8*(%3+10)], rax
    xor [rdi + 8*(%3+15)], rax
    xor [rdi + 8*(%3+20)], rax
%endmacro

%macro THETA_LOAD 2
    mov %1, [rdi + 8*%2]
    xor %1, [rdi + 8*(%2+5)]
    xor %1, [rdi + 8*(%2+10)]
    xor %1, [rdi + 8*(%2+15)]
    xor %1, [rdi + 8*(%2+20)]
%endmacro

%macro CHI_LINE 1
    mov r10, [rdi + (%1+0)*8]
    mov r11, [rdi + (%1+1)*8]
    mov rdx, [rdi + (%1+2)*8]
    mov r9, [rdi + (%1+3)*8]
    mov r8, [rdi + (%1+4)*8]

    andn rax, r11, rdx
    xor [rdi + (%1+0)*8], rax

    andn rax, rdx, r9
    xor [rdi + (%1+1)*8], rax

    andn rax, r9, r8
    xor [rdi + (%1+2)*8], rax

    andn rax, r8, r10
    xor [rdi + (%1+3)*8], rax

    andn rax, r10, r11
    xor [rdi + (%1+4)*8], rax
%endmacro

%macro ROUND 1
    ; theta step
    THETA_LOAD r10, 0
    THETA_LOAD r11, 1
    THETA_LOAD rdx, 2
    THETA_LOAD r9, 3
    THETA_LOAD r8, 4

    THETA_MICROSTEP r11, r8, 0
    THETA_MICROSTEP rdx, r10, 1
    THETA_MICROSTEP r9, r11, 2
    THETA_MICROSTEP r8, rdx, 3
    THETA_MICROSTEP r10, r9, 4

    ; rho and pi steps
    mov rax, [rdi + 1*8]
    ROTATE_ASSIGN 10, 1
    ROTATE_ASSIGN 7, 3
    ROTATE_ASSIGN 11, 6
    ROTATE_ASSIGN 17, 10
    ROTATE_ASSIGN 18, 15
    ROTATE_ASSIGN 3, 21
    ROTATE_ASSIGN 5, 28
    ROTATE_ASSIGN 16, 36
    ROTATE_ASSIGN 8, 45
    ROTATE_ASSIGN 21, 55
    ROTATE_ASSIGN 24, 2
    ROTATE_ASSIGN 4, 14
    ROTATE_ASSIGN 15, 27
    ROTATE_ASSIGN 23, 41
    ROTATE_ASSIGN 19, 56
    ROTATE_ASSIGN 13, 8
    ROTATE_ASSIGN 12, 25
    ROTATE_ASSIGN 2, 43
    ROTATE_ASSIGN 20, 62
    ROTATE_ASSIGN 14, 18
    ROTATE_ASSIGN 22, 39
    ROTATE_ASSIGN 9, 61
    ROTATE_ASSIGN 6, 20
    ROTATE_ASSIGN 1, 44

    ; chi step
    CHI_LINE 0
    CHI_LINE 5
    CHI_LINE 10
    CHI_LINE 15
    CHI_LINE 20

    ; iota step
    mov rax, %1
    xor [rdi + 0 * 8], rax
%endmacro

global keccakf
keccakf:
    bits 64
    prefetchnta [keccakf_rndc + 192]
    push r12
    push r13
    push r14
    push r15

    ROUND [keccakf_rndc + 0 * 8]
    ROUND [keccakf_rndc + 1 * 8]
    ROUND [keccakf_rndc + 2 * 8]
    ROUND [keccakf_rndc + 3 * 8]
    ROUND [keccakf_rndc + 4 * 8]
    ROUND [keccakf_rndc + 5 * 8]
    ROUND [keccakf_rndc + 6 * 8]
    ROUND [keccakf_rndc + 7 * 8]
    ROUND [keccakf_rndc + 8 * 8]
    ROUND [keccakf_rndc + 9 * 8]
    ROUND [keccakf_rndc + 10 * 8]
    ROUND [keccakf_rndc + 11 * 8]
    ROUND [keccakf_rndc + 12 * 8]
    ROUND [keccakf_rndc + 13 * 8]
    ROUND [keccakf_rndc + 14 * 8]
    ROUND [keccakf_rndc + 15 * 8]
    ROUND [keccakf_rndc + 16 * 8]
    ROUND [keccakf_rndc + 17 * 8]
    ROUND [keccakf_rndc + 18 * 8]
    ROUND [keccakf_rndc + 19 * 8]
    ROUND [keccakf_rndc + 20 * 8]
    ROUND [keccakf_rndc + 21 * 8]
    ROUND [keccakf_rndc + 22 * 8]
    ROUND [keccakf_rndc + 23 * 8]

    pop r15
    pop r14
    pop r13
    pop r12
    ret
