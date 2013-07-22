/*
  
    Shellcode tester

*/

#include <stdio.h>
#include <string.h>

// Shellcode encoded with custom insertion encoder
unsigned char shellcode[] = 
        "<SHELLCODE>";

// Function pointer
typedef int (*func_ptr)();

void main() {
    printf("Shellcode Length:  %d bytes\n", strlen(shellcode));

    ((func_ptr) shellcode)();
}
