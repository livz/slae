/* 
    Usage :
    $ gcc -Wall -o sethostn sethostn.c 
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv) {
    int ret;
    char buf[32];

    ret = gethostname(buf, sizeof buf);

    if (-1 == ret) {
        fprintf(stderr, "%s: gethostname(2)\n", strerror(errno));
        exit(1);
    }
    printf("Previous host name = '%s'\n", buf);

    ret = sethostname(argv[1], strlen(argv[1])>15?15:strlen(argv[1]));
    if (-1 == ret) {
        fprintf(stderr, "%s: sethostname(2)\n", strerror(errno));
        exit(1);
    }

    return 0;
}
