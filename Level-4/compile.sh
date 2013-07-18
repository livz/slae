#!/bin/bash

DECODER=custom-decoder
SHELLCODE=shellcode

echo '  [+] Assembling egghunter'
nasm -f elf32 -o $DECODER.o $DECODER.nasm

echo '  [+] Linking egghunter'
# Link with stack execution permissions! (-N)
ld -N -z execstack -o $DECODER $DECODER.o

#echo '  [+] Compile shellcode tester'
ENC_SHELLCODE=$(echo `objdump -d $DECODER  | grep -v file | grep '[0-9a-f]:' | cut  -f 2 ` | sed 's/ /\\x/g' | sed 's/^/\\x/g')
echo $ENC_SHELLCODE
ENC_SHELLCODE=$(echo $ENC_SHELLCODE | sed 's/\\/\\\\/g')
sed "s/<SHELLCODE>/$ENC_SHELLCODE/g" $SHELLCODE.c > temp.c
gcc -z execstack -o $SHELLCODE  temp.c

echo '  [+] House cleaning'
rm -rf *.o temp*

echo '  [+] Done!'

