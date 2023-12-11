#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include <netinet/in.h>
#include <time.h>

#define SERVER_MAX_SIZE_LOGIN 10

extern const unsigned int BACKLOG;
extern const unsigned int BUFFER_LEN;
extern const unsigned int MAX_MISS;
extern const unsigned int CLIENT_BLOC_SIZE;

// Infos d'un client
typedef struct {
    struct sockaddr_in addr;
    char login[SERVER_MAX_SIZE_LOGIN];
    int client_socket;
    time_t lastActivityTime;
} clientInfo;

// Liste des clients
typedef struct {
    clientInfo *list;
    unsigned int capacity;
    unsigned int size;
} clientList;

void addClient(clientList *clients, const struct sockaddr_in client_address, const char *login, int client_socket);
unsigned findClient(const struct sockaddr_in *client_address, const clientList *clients);
unsigned findClientBySocket(int client_socket, const clientList *clients);

#endif
