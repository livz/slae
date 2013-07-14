#!/bin/bash

PROG_NAME=shell_bind_tcp
EXPECTED_ARGS=1
E_BADARGS=10
E_BADPORT=11

# Check number of arguments
if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: `basename $0` {port}"
  exit $E_BADARGS
fi

# Check port number
echo $1 | grep -E -q '^[0-9]+$'
RETVAL=$?

if [ $RETVAL -eq 0 ] 
then 
    PORT=0x`printf "%04x" $1 | cut -b 3-4``printf "%04x" $1 | cut -b 1-2`
    echo '[+] Replacing port number with' $PORT
    sed "s/<PORT>/$PORT/g" $PROG_NAME.nasm > temp.nasm
else
    echo '[-] Bad port agument provided:' $1
    exit $E_BADPORT
fi

echo '[+] Assembling with Nasm ... '
nasm -f elf32 -o temp.o temp.nasm

echo '[+] Linking ...'
ld -z execstack -o $PROG_NAME temp.o

echo '[+] Cleaning ...'
rm -f temp.* *~

echo '[+] Done!'

