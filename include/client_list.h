#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include <netinet/in.h>

#define SERVER_MAX_SIZE_LOGIN 10

extern const unsigned int BACKLOG;
extern const unsigned int BUFFER_LEN;
extern const unsigned int MAX_MISS;
extern const unsigned int CLIENT_BLOC_SIZE;

typedef struct {
    struct sockaddr_in addr;
    char login[SERVER_MAX_SIZE_LOGIN];
    unsigned counter;
} clientInfo;

typedef struct {
    clientInfo *list;
    unsigned int capacity;
    unsigned int size;
} clientList;

void addClient(clientList *clients, struct sockaddr_in addr, char *login);

#endif
