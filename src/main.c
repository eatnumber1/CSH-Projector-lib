#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORTNUMBER "7142"

#include "include/nec.h"

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

    struct nec_msg_hdr* hdr = malloc(sizeof(struct nec_msg_hdr));
    struct nec_msg* msg = malloc(sizeof(struct nec_msg));
    memset(hdr, 0, sizeof(struct nec_msg_hdr));
    memset(msg, 0, sizeof(struct nec_msg));

    hdr->command = 0xA200;
    hdr->projector_id = 0x01;
    hdr->model_code = 0x2;
    hdr->data_len = 0x02;
    msg->hdr = hdr;
    printf("%d\n", sizeof(*hdr));

    printf("Is there an error? %x\n", nec_checkerrs(msg));
    free(hdr);
    free(msg);

    freeaddrinfo(res);

    return 0;
}
