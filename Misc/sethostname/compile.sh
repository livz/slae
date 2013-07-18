#!/bin/bash

# 
# Usage:
# $ ./compile.sh
# [+] Assembling with Nasm ... 
# [+] Linking ...
# [+] Cleaning ...
# [+] Done!
# $ sudo ./sethostn 
# $ hostname 
# M4tr|X

PROG_NAME=sethostn

echo '[+] Assembling with Nasm ... '
nasm -f elf32 -o $PROG_NAME.o $PROG_NAME.nasm

echo '[+] Linking ...'
ld -z execstack -o $PROG_NAME $PROG_NAME.o

echo '[+] Cleaning ...'
#rm -f temp.* *~

echo '[+] Done!'

