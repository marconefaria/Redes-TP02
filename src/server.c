// common function implemented
#include "util.h"

// C libraries
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// socket libraries
#include <sys/socket.h>
#include <sys/types.h>

// constants using in this file
#define BUFSZ 1024
int equipment_id = 0;

#define EQUIPMENT_NUMBER 15
#define LIST "list"
#define EQUIPMENT "equipment"
#define CLOSE "close"
#define REQUEST "request"

void usage(int argc, char **argv)
{
    printf("usage: %s <server port>\n", argv[0]);
    printf("example: %s 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

struct client_data
{
    int csock;
    int id;
    struct sockaddr_storage storage;
};

void *client_thread(void *data)
{
    char buf[BUFSZ];
    memset(buf, 0, BUFSZ);
    size_t count;
    equipment_id++;

    struct client_data *cdata = (struct client_data *)data;
    struct sockaddr *caddr = (struct sockaddr *)(&cdata->storage);

    cdata->id = equipment_id;

    char caddrstr[BUFSZ];
    addrtostr(caddr, caddrstr, BUFSZ);

    for (int i = 1; i <= EQUIPMENT_NUMBER; i++)
    {
        if (DATA[i] == -1)
        {
            equipment_id = i;
            break;
        }
    }

    if (equipment_id > 15)
    {
        printf("Equipment limit exceeded\n");
        sprintf(buf, "Equipment limit exceeded\n");

        count = send(cdata->csock, buf, strlen(buf), 0);

        if (count != strlen(buf))
        {
            logexit("send");
        }

        exit(EXIT_SUCCESS);
    }
    else
    {
        cdata->id > 10 ? sprintf(buf, "New ID: %d\n", cdata->id) : sprintf(buf, "New ID: 0%d\n", cdata->id);
        count = send(cdata->csock, buf, strlen(buf), 0);

        if (count == strlen(buf))
        {
            DATA[cdata->id] = rand() % 1000 / 100.0;
        }

        equipment_id < 10 ? printf("Equipment 0%d added\n", cdata->id) : printf("Equipment %d added\n", cdata->id);

        while (1)
        {
            memset(buf, 0, BUFSZ);
            count = recv(cdata->csock, buf, BUFSZ - 1, 0);

            char msg[BUFSZ];

            strcpy(msg, buf);
            memset(buf, 0, BUFSZ);

            int currentPosition = 0;
            int entryNumbers = countEntryNumbers(msg);

            char *entries[entryNumbers + 2];
            char aux[16];
            int positionReq = 0;

            entries[0] = strtok(msg, " ");
            for (currentPosition = 1; currentPosition < entryNumbers; currentPosition++)
            {
                entries[currentPosition] = strtok(NULL, " ");
            }

            if (strcmp(LIST, entries[0]) == 0)
            {
                for (int i = 1; i <= EQUIPMENT_NUMBER; i++)
                {
                    if (DATA[i] != -1)
                    {
                        if (i < 10)
                        {
                            sprintf(aux, "0%d ", i);
                        }
                        else
                        {
                            sprintf(aux, "%d ", i);
                        }
                        strcat(buf, aux);
                    }
                }

                strcat(buf, " ");
            }
            else if (strcmp(CLOSE, entries[0]) == 0)
            {
                cdata->id > 10 ? printf("Equipment %d removed\n", cdata->id) : printf("Equipment 0%d removed\n", cdata->id);
                DATA[cdata->id] = -1.0;
                sprintf(buf, "Successful removal\n");
            }
            else if (strcmp(REQUEST, entries[0]) == 0)
            {
                positionReq = atoi(entries[3]);
                if (DATA[positionReq] != -1.00)
                {
                    positionReq > 10 ? sprintf(buf, "Value from %d: %.2f\n", positionReq, DATA[positionReq]) : sprintf(buf, "Value from 0%d: %.2f\n", positionReq, DATA[positionReq]);
                }
                else
                {
                    positionReq > 10 ? printf("Equipment %d not found\n", positionReq) : printf("Equipment 0%d not found\n", positionReq);
                    positionReq > 10 ? sprintf(buf, "Equipment %d not found\n", positionReq) : sprintf(buf, "Equipment 0%d not found\n", positionReq);
                }
            }
            else
            {
                printf("Invalid command\n");
                sprintf(buf, "Invalid command\n");
            }

            count = send(cdata->csock, buf, strlen(buf), 0);

            if (count != strlen(buf))
            {
                logexit("send");
            }
        }
    }

    close(cdata->csock);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], &storage))
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

    for (int i = 1; i <= EQUIPMENT_NUMBER; i++)
    {
        DATA[i] = -1.00;
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("bound to %s, waiting connections\n", addrstr);

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

        struct client_data *cdata = malloc(sizeof(*cdata));
        if (!cdata)
        {
            logexit("malloc");
        }

        cdata->csock = csock;
        memcpy(&(cdata->storage), &cstorage, sizeof(cstorage));

        pthread_t tid;
        pthread_create(&tid, NULL, client_thread, cdata);
    }

    pthread_exit(EXIT_SUCCESS);
    exit(EXIT_SUCCESS);
}