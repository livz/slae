#!/bin/bash

EGGHUNTER=egghunter-access
SHELLCODE=shellcode

echo '  [+] Assembling egghunter'
nasm -f elf32 -o $EGGHUNTER.o $EGGHUNTER.nasm

echo '  [+] Linking egghunter'
ld -z execstack -o $EGGHUNTER $EGGHUNTER.o

echo '  [+] Compile shellcode tester'
gcc -z execstack -o $SHELLCODE  $SHELLCODE.c

echo '  [+] House cleaning'
rm -rf *.o

echo '  [+] Done!'

