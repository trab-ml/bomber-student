#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#define CLIENT_LOGIN_SIZE 10
#define INACTIVITY_TIMEOUT_SECONDS 60 * 10 // inactivity timeout of 10 minutes
#define THREAD_POOL_SIZE 5 // Max connection queue size
#define LOOKING_FOR_BOMBERSTUDENT_SERVER_MESSAGE "looking for bomberstudent servers"
#define MIN_MESSAGE_LENGTH 2 // Minimum length of a message

#include <stdbool.h>

typedef struct
{
    int client_socket;
    struct sockaddr_in client_address;
    clientList *clients;
} ThreadArgs;

bool isConnectionClosed(int len, const struct sockaddr_in *client_address, ThreadArgs *args);
void sendMessage(int client_socket, const char *message);
void sendMessageAndExit(ThreadArgs *args, const char *message);
bool validateMessageLength(ThreadArgs *args, char *buffer, const char *expectedMessage, const char *errorMessage);
bool listenClientsLookingForServer(ThreadArgs *args, char *buffer);
void *handleClientThread(void *args);
void processClientMessage(int client_socket, const char *buffer, clientList *clients);
void handleInactiveClients(clientList *clients);
ThreadArgs *initThreadArgs(int client_socket, struct sockaddr_in client_address, clientList *clients);
void destroyThreadPool();

void runServer(int server_socket, clientList *clients);
void cleanupServer(int server_socket, clientList *clients);

#endif