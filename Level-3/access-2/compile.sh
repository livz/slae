#!/bin/bash

# Title: Linux Egghunter Shellcode
# (compilation wrapper)
# Author Liviu I. 
#
# License http://creativecommons.org/licenses/by-sa/3.0/
# Legitimate use and research only
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

EGGHUNTER=egghunter-access2
SHELLCODE=shellcode
EGG=W00T

# Check number of arguments
if [ $# -ne 1 ]
then
    echo "Usage: `basename $0` {4 chars EGG mark}"
    echo " E.g.: `basename $0` W00T"
    echo "No egg specified. Using default $EGG"    
else
    # Truncate the input string to 4 characters and convert to hex
    EGG=`echo $1 | cut -c1-4`
    echo "  [*] Using egg " $EGG
fi

# Assume EGG is string. Convert to hex
EGGHEX=` python -c "print '0x'+'$EGG'[::-1].encode('hex')"`
echo '  [*] Egg hex:', $EGGHEX

echo '  [+] Replace egg with', $1
sed "s/<EGG>/$EGGHEX/g" $EGGHUNTER.nasm > temp.nasm

echo '  [+] Assembling egghunter'
nasm -f elf32 -o temp.o temp.nasm

echo '  [+] Linking egghunter'
ld -z execstack -o $EGGHUNTER temp.o

echo '  [*] Egghunter:'
HUNTER=$(echo `objdump -d $EGGHUNTER  | grep -v file | grep '[0-9a-f]:' | cut  -f 2 ` | sed 's/ /\\x/g' | sed 's/^/\\x/g')
echo $HUNTER

echo '  [+] Compile shellcode tester'
sed "s/<EGG>/$EGG/g" $SHELLCODE.c > temp.c
HUNTER=$(echo $HUNTER | sed 's/\\/\\\\/g')
sed "s/<EGGHUNTER>/$HUNTER/g" temp.c > temp1.c
gcc -z execstack -o $SHELLCODE  temp1.c

echo '  [+] House cleaning'
rm -rf *.o temp*

echo "  [+] Done! run ./$SHELLCODE"

