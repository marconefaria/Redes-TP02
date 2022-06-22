#pragma once
#include <stdlib.h>
#include <arpa/inet.h>

#define EQUIPMENT_NUMBER 15
#define REMOVE "remove"
#define LIST "list"
#define CLOSE "close"
#define SPACE ' '

// control number of equipments
float DATA[EQUIPMENT_NUMBER];

void logexit(const char *msg);

int addrparse(const char *addrstr, const char *portstr,
              struct sockaddr_storage *storage);

void addrtostr(const struct sockaddr *addr, char *str, size_t strsize);

int server_sockaddr_init(const char *portstr, struct sockaddr_storage *storage);

int countEntryNumbers(char temporary_string[]);