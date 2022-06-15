// common  functions implemented
#include "common.h"
#include "handlers.h"

// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// socket libraries
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

// constants using in this file
#define BUFSZ 500
#define EQUIPMENT_NUMBER 4
#define SENSOR_NUMBER 4
#define SENSORS ((EQUIPMENT_NUMBER) * (SENSOR_NUMBER)) - 1
#define SIZE_ID 3

void usage(int argc, char **argv)
{
    printf("usage: %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage))
    {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1)
    {
        logexit("socket");
    }

    int enable = 1;
    if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    {
        logexit("setsockopt");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage)))
    {
        logexit("bind");
    }

    if (0 != listen(s, 10))
    {
        logexit("listen");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("bound to %s, waiting connections\n", addrstr);

    float DATA[EQUIPMENT_NUMBER][SENSOR_NUMBER];
    for (int i = 1; i <= EQUIPMENT_NUMBER; i++)
    {
        for (int j = 1; j <= SENSOR_NUMBER; j++)
        {
            DATA[i][j] = -1.00;
        }
    }

    while (1)
    {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        int csock = accept(s, caddr, &caddrlen);
        if (csock == -1)
        {
            logexit("accept");
        }

        char caddrstr[BUFSZ];
        addrtostr(caddr, caddrstr, BUFSZ);
        printf("[log] connection from %s\n", caddrstr);

        char buf[BUFSZ];
        memset(buf, 0, BUFSZ);
        size_t count;

        while (1)
        {
            memset(buf, 0, BUFSZ);
            count = recv(csock, buf, BUFSZ - 1, 0);
            printf("[msg] %s, %d bytes: %s\n", caddrstr, (int)count, buf);

            handle(buf, DATA);
            count = send(csock, buf, strlen(buf), 0);
            if (count != strlen(buf))
            {
                logexit("send");
            }
        }
        close(csock);
    }

    exit(EXIT_SUCCESS);
}