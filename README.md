SLAE
====

## SLAE Assignments
This contains the files needed for SecurityTube SLAE exam

### **Assignment 1 - Shell Bind TCP shellcode**
* Configurable listening port
* C source code
* Assembly source
* Bash compile script
* Wrapper C program

### **Assignment 2 - Shell Reverse TCP shellcode**
* Configurable destination IP and port
* C source code
* Assembly source
* Bash compile script
* Wrapper C program

### **Assignment 3 - Egghunter shellcode**
* access() egghunter (39 bytes)
* access() egghunter reloaded (35 bytes)
* sigaction() egghunter 30 bytes
* sigaction() egghunter reloaded 28 bytes

### **Assignment 4 - Custom insertion encoder**
* Custom encoder scheme
* Configurable garbage amount

### **Assignment 5 - Metasploit Shellcode analysis**
* linux/x86/adduser  
Create a new user with UID 0

* linux/x86/shell_find_port  
Spawn a shell on an established connection

* linux/x86/meterpreter/reverse_tcp  
Connect back to the attacker, Staged meterpreter server

### **Assignment 6 - Shell-storm polymorphic shellcodes**
* Original: [setuid(); execve(); exit(0)][1]
* Original: [execve-chmod 0777 /etc/shadow][2]
* Original: [push reboot()][3]

### **Assignment 7 - Shellcode crypter**
* AES-128 CBC encrypted shellcode
 - python crypter (python-crypto)
 - C decrypter (PolarSSL)

### **Miscellaneous shellcodes**
* Change hostname shellcode
* TCP Shell Bind + password 

----------



  [1]: http://shell-storm.org/shellcode/files/shellcode-549.php
  [2]: http://shell-storm.org/shellcode/files/shellcode-828.php
  [3]: http://shell-storm.org/shellcode/files/shellcode-69.php
