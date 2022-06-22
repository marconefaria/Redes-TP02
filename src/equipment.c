// common functions implemented
#include "util.h"

// C libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// socket libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 1024

void usage(int argc, char **argv)
{
    printf("usage: %s <server IP> <server port>\n", argv[0]);
    printf("example: %s 127.0.0.1 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    const char *server_IP = argv[1];
    in_port_t server_port = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        logexit("Failed to create socket");

    if (argc < 3)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != addrparse(argv[1], argv[2], &storage))
    {
        usage(argc, argv);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    if (inet_pton(AF_INET, server_IP, &server_address.sin_addr.s_addr) <= 0)
        logexit("Failed to convert server address");

    server_address.sin_port = htons(server_port);

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        logexit("Failed to connect");

    char buf[BUFSZ];
    memset(buf, 0, BUFSZ);

    size_t count;
    unsigned total = 0;

    count = recv(sock, buf + total, BUFSZ - total, 0);
    puts(buf);

    while (1)
    {
        printf(">>> ");
        fgets(buf, BUFSZ - 1, stdin);

        count = send(sock, buf, strlen(buf), 0);
        if (count != strlen(buf))
        {
            logexit("send");
        }

        memset(buf, 0, BUFSZ);

        total = 0;
        count = recv(sock, buf + total, BUFSZ - total, 0);
        puts(buf);
        if (count == 0)
        {
            break;
        }
        total += count;
        memset(buf, 0, BUFSZ);
    }

    close(sock);

    printf("received %u bytes\n", total);

    exit(EXIT_SUCCESS);
}