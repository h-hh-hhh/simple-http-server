#ifdef __linux__
#define _DEFAULT_SOURCE
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULTPORT "10001"
#define MSGSIZ (BUFSIZ * 0x400)

int main(int argc, char **argv) {
    const char *port = DEFAULTPORT;
    if (argc < 2) {
        fprintf(stderr, "usage: %s [port=%s]\n", argv[0], DEFAULTPORT);
    }
    else {
        port = argv[1];
    }

    int status;
    struct sockaddr_storage remote_addr;
    socklen_t addr_size;
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, port, &hints, &res))) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    printf("Creating socket...\n");
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    printf("Binding...\n");
    bind(sockfd, res->ai_addr, res->ai_addrlen);

    // kanged error bypass 
    // Signed-off-by: PUNEET BALLABH
    int yes=1;
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    }

    printf("Listening on port %s...\n", port);
    listen(sockfd, 10);

    addr_size = sizeof remote_addr;
    char zipzorp[16];
    int con_sockfd;
    char *buf = malloc(BUFSIZ);
    char *msg = malloc(MSGSIZ);
    // the actual server part
    while (con_sockfd = accept(sockfd, (struct sockaddr *)&remote_addr, &addr_size)) {
        snprintf(
            zipzorp, 
            16, 
            "%u.%u.%u.%u", 
            (((struct sockaddr_in *)&remote_addr)->sin_addr.s_addr >> 0) & 0xFF, 
            (((struct sockaddr_in *)&remote_addr)->sin_addr.s_addr >> 8) & 0xFF, 
            (((struct sockaddr_in *)&remote_addr)->sin_addr.s_addr >> 16) & 0xFF, 
            (((struct sockaddr_in *)&remote_addr)->sin_addr.s_addr >> 24) & 0xFF
            );
        printf("Connected to %s\n", zipzorp);
        while ((status = recv(con_sockfd, buf, BUFSIZ, 0)) > 0) {
            memset(msg, 0, MSGSIZ);
            snprintf(
                msg, 
                BUFSIZ, 
                "HTTP/1.0 200 OK\n\n<html><head><title>TND</title></head><body><h1>h</h1><h2>Your request was:</h2><pre>%s</pre></body></html>",
                buf
                );
            printf("Bytes sent: %d\n", send(con_sockfd, msg, strlen(msg), 0));
            close(con_sockfd);
            memset(buf, 0, BUFSIZ);
        }
    }

    // cleanup
    close(con_sockfd);
    close(sockfd);
    free(buf);
    freeaddrinfo(res);
}