#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#define SERVER_MAX_SIZE_LOGIN 10
#define WELCOME_MESSAGE "Dites bonjour à "
#define INACTIVITY_TIMEOUT_SECONDS 60 * 10 // inactivity timeout of 10 minutes
#define THREAD_POOL_SIZE 5 // Max connection queue size

typedef struct
{
    int client_socket;
    struct sockaddr_in client_address;
    clientList *clients;
} ThreadArgs;

void sendMessage(int client_socket, const char *message);
void *handleClientThread(void *args);
void processClientMessage(int client_socket, const char *buffer, clientList *clients);
void handleInactiveClients(clientList *clients);
ThreadArgs *initThreadArgs(int client_socket, struct sockaddr_in client_address, clientList *clients);
void destroyThreadPool();

void initializeServer(int *server_socket, clientList *clients);
void runServer(int server_socket, clientList *clients);
void cleanupServer(int server_socket, clientList *clients);

#endif