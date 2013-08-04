Simple techniques applied to make shellcode polymorphic
-------------------------------------------------------

* Add garbage/nop-like instructions  
e.g.:

    nop
    
    mov al,al  
    mov bl,bl  
    mov cl,cl  
    ...  
    mov ax,ax  
    mov bx,bx  
    mov cx,cx  
    ...  
    xchg ax,ax  
    xchg bx,bx  
    xchg cx,cx  
    ...  
    lea eax, [eax + 0x00]  
    lea eax, [eax + 0x00]  
    lea eax, [eax + 0x00]  
    ...  

* Add instructions without effect.  
e.g.:  
modify registers that don't affect the execution flow

* Switch between mov, (push + pop), (clear + add)  
e.g:  

| mov          |  push + pop   | clear + add  | 
| ------------ | ------------- |------------- |
| mov al, 0xb  | push byte 0xb | xor eax, eax | 
|              | pop eax       | add al, 0xb  |

    

* Switch between push and (mov + add + push)  
e.g:
|       push      |   mov + add + push  |
|-----------------| ------------------- |
| push 0x23456789 | mov esi, 0x12345678 |
|                 | add esi, 0x11111111 |
|                 | push esi            |


* Change between push and directly accessing the stack  
e.g.:
|       push      |         stack access          |
| --------------- | ----------------------------- |
| push 0x64777373 | mov dword [esp-4], 0x64777373 |
|                 |          sub esp, 4           | 


Shellcode mutation engines
--------------------------

[ASMutate][1]


  [1]: http://www.ktwo.ca/readme.html
