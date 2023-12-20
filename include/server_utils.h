#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#define CLIENT_LOGIN_MIN_SIZE 3
#define CLIENT_LOGIN_MAX_SIZE 50
#define INACTIVITY_TIMEOUT_SECONDS 60 * 10 // inactivity timeout of 10 minutes
#define THREAD_POOL_SIZE 100 // Max connection queue size
#define LOOKING_FOR_BOMBERSTUDENT_SERVER_MESSAGE "looking for bomberstudent servers"
#define MIN_MESSAGE_LENGTH 2 // Minimum length of a message
#define DISCONNECTED_MESSAGE "Disconnected"
#define SERVER_HELLO_MESSAGE "hello i’m a bomberstudent server"

#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "test_cJSON.h"
#include "socket_utils.h"
#include "client_list.h"
#include "error_handler.h"
#include <stdbool.h>

// Arguments for the thread handling a client
typedef struct
{
    int thread_id;
    int client_index;
    int client_socket;
    struct sockaddr_in client_address;
    char buffer[BUFFER_LEN + 1];
    clientList *clients;
} ThreadArgs;

bool isConnectionClosed(int len, const struct sockaddr_in *client_address, ThreadArgs *args);
bool checkTriggerMessage(ThreadArgs *args, char *buffer);
bool isLookingForServer(ThreadArgs *args, char *buffer);
void closeSocket(ThreadArgs *args);
bool isCompliantLogin(char *login);

void sendMessage(int client_socket, const char *message);
void sendMessageAndExit(ThreadArgs *args, const char *message);

void *handleClientThread(void *args);
void processClientMessage(ThreadArgs *args);
void handleInactiveClients(clientList *clients);

ThreadArgs *initThreadArgs(int client_socket, struct sockaddr_in client_address, clientList *clients);
void joinAndFreeThread(pthread_t *thread);
void freeThreadArgs(ThreadArgs *args);
void destroyThreadPool();

void runServer(int server_socket, clientList *clients);
void cleanupServer(int server_socket, clientList *clients);

#endif