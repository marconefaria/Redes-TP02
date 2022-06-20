#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Practical.h"

void DieWithUserMessage(const char *msg, const char *detail)
{
    fputs(msg, stderr);
    fputs(": ", stderr);
    fputs(detail, stderr);
    fputc('\n', stderr);
    exit(1);
}

void DieWithSystemMessage(const char *msg)
{
    perror(msg);
    exit(1);
}

int BroadcastSender(int argc, char *argv[])
{
    if (argc != 3) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Port> <String to send>");

    in_port_t port = htons((in_port_t)atoi(argv[2]));

    struct sockaddr_storage destStorage;
    memset(&destStorage, 0, sizeof(destStorage));

    size_t addrSize = 0;

    struct sockaddr_in *destAddr4 = (struct sockaddr_in *)&destStorage;
    destAddr4->sin_family = AF_INET;
    destAddr4->sin_port = port;
    destAddr4->sin_addr.s_addr = INADDR_BROADCAST;
    addrSize = sizeof(struct sockaddr_in);

    struct sockaddr *destAddress = (struct sockaddr *)&destStorage;

    size_t msgLen = strlen(argv[3]);
    if (msgLen > MAXSTRINGLENGTH) // Input string fits?
        DieWithUserMessage("String too long", argv[3]);

    // Create socket for sending/receiving datagrams
    int sock = socket(destAddress->sa_family, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
        DieWithSystemMessage("socket() failed");

    // Set socket to allow broadcast
    int broadcastPerm = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastPerm,
                   sizeof(broadcastPerm)) < 0)
        DieWithSystemMessage("setsockopt() failed");

    for (;;)
    { // Run forever
        // Broadcast msgString in datagram to clients every 3 seconds
        ssize_t numBytes = sendto(sock, argv[3], msgLen, 0, destAddress, addrSize);
        if (numBytes < 0)
            DieWithSystemMessage("sendto() failed");
        else if (numBytes != msgLen)
            DieWithUserMessage("sendto()", "sent unexpected number of bytes");

        sleep(3); // Avoid flooding the network }
        // NOT REACHED
    }
}