#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    struct addrinfo *results, *rp;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /*
        this is a client socket so we don't need AI_PASSIVE flag here because
        thats only for sockets that bind to an address, port.

        socket is just an endpoint for communication.
        -------------The server is NOT a socket-----------------------
    */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int sfd, s;
    s = getaddrinfo("localhost", "3000", &hints, &results);

    if (s != 0) {
        printf("failed %d\n", s);
        return 1;
    }

    for (rp = results; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) break;
        close(sfd);
    }
    freeaddrinfo(results);

    if (rp == NULL) {
        fprintf(stderr, "Could not bind\n");
        return 1;
    }

    char buff[1024];

    read(sfd, buff, 250);
    printf("%s\n", buff);

    close(sfd);
}