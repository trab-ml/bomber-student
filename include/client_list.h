#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include <netinet/in.h>
#include <time.h>

// Maximum length of a login
#define MAX_LOGIN_LEN 20
#define NEW_CLIENT_MESSAGE_FORMAT "[SERVER] Player %s joined the game\n"
#define WELCOME_MESSAGE_FORMAT "[SERVER] Welcome %s\n"
#define BUFFER_LEN 1024
#define MAX_MISS 128
#define CLIENT_BLOC_SIZE 1024

// Client info
typedef struct {
    struct sockaddr_in addr;
    char login[MAX_LOGIN_LEN];
    int client_socket;
    int playerId;
    int numPlayer;
    time_t lastActivityTime;
} clientInfo;

// List of clients
typedef struct {
    clientInfo *list;
    unsigned int capacity;
    unsigned int size;
} clientList;

void addClient(clientList *clients, const struct sockaddr_in client_address, const char *login, int client_socket);
unsigned findClient(const struct sockaddr_in *client_address, const clientList *clients);
unsigned findClientBySocket(int client_socket, const clientList *clients);
void freeClientList(clientList *clients);
void broadcastNewClient(const clientList *clients, const char *newClientLogin);

#endif
