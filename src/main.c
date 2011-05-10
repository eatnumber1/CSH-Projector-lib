#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORTNUMBER "7142"

#include "src/nec.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ip address>\n", argv[0]);
        return 1;
    }

    char *ip_c = argv[1];
    int ret, sock;

    struct in_addr *ip;
    struct sockaddr_in *sockaddr = NULL;
    struct addrinfo *p, hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((ret = getaddrinfo(ip_c, PORTNUMBER, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo err: %s\n", gai_strerror(ret));
        return 1;
    }

    for(p = res; p != NULL; p = p->ai_next) {
        if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            perror("socket");
            continue;
        }

        if (connect(sock, p->ai_addr, p->ai_addrlen) != 0) {
            perror("connect");
            continue;
        }
    }

    return 0;
}
