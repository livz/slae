#!/bin/bash
#
# SLAE - Assignment 2
# Title: Shell Reverse TCP
# Author Liviu I. 
#
# License http://creativecommons.org/licenses/by-sa/3.0/
# Legitimate use and research only
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

PROG_NAME=shell_reverse_tcp
EXPECTED_ARGS=2
E_BADARGS=10
E_BADPORT=11
E_BADIP=12

# Check number of arguments
if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: `basename $0` {ip} {port}"
  exit $E_BADARGS
fi

# Validate IP address
# http://www.linuxjournal.com/content/validating-ip-address-bash-script
function valid_ip()
{
    local  ip=$1
    local  stat=1

    if [[ $ip =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]]; then
        OIFS=$IFS
        IFS='.'
        ip=($ip)
        IFS=$OIFS
        [[ ${ip[0]} -le 255 && ${ip[1]} -le 255 \
            && ${ip[2]} -le 255 && ${ip[3]} -le 255 ]]
        stat=$?
    fi
    return $stat
}

# Check IP address
if valid_ip $1
then
    B1=$(printf "%02x" `echo $1 | cut -d '.' -f 1`)
    B2=$(printf "%02x" `echo $1 | cut -d '.' -f 2`)
    B3=$(printf "%02x" `echo $1 | cut -d '.' -f 3`)
    B4=$(printf "%02x" `echo $1 | cut -d '.' -f 4`)  
    IP=0x$B4$B3$B2$B1
    echo '[+] Replacing IP with' $IP
    sed "s/<IP>/$IP/g" $PROG_NAME.nasm > temp1.nasm
else
    echo '[-] Bad IP agument provided:' $1
    exit $E_BADIP
fi

# Check port number
echo $2 | grep -E -q '^[0-9]+$'
RETVAL=$?

if [ $RETVAL -eq 0 ] 
then 
    PORT=0x`printf "%04x" $2 | cut -b 3-4``printf "%04x" $2 | cut -b 1-2`
    echo '[+] Replacing port number with' $PORT
    sed "s/<PORT>/$PORT/g" temp1.nasm > temp.nasm
else
    echo '[-] Bad port agument provided:' $1
    exit $E_BADPORT
fi

echo '[+] Assembling with Nasm ... '
nasm -f elf32 -o temp.o temp.nasm

echo '[+] Linking ...'
ld -z execstack -o $PROG_NAME temp.o

echo '[+] Cleaning ...'
rm -f temp* *~ *.o

echo '[+] Done!'

