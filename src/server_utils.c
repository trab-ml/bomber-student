#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "test_cJSON.h"
#include "socket_utils.h"
#include "client_list.h"
#include "server_utils.h"
#include "error_handler.h"
#include <pthread.h>

// Global flag to track initialization status
int initialized = 0;

pthread_mutex_t clientsMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t threads[THREAD_POOL_SIZE];

void sendMessage(int client_socket, const char *message)
{
    if (send(client_socket, message, strlen(message), 0) < 0)
    {
        handleError(SOCKET_SEND_ERROR);
    }
}

/**
 * @brief Handles a new client connection by reading messages and processing them
 * @param client_socket Socket of the client
 * @param client_address Address of the client
 * @param clients List of connected clients
 */
void *handleClientThread(void *args)
{
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int client_socket = threadArgs->client_socket;
    struct sockaddr_in *client_address = &(threadArgs->client_address);
    clientList *clients = threadArgs->clients;

    char buffer[BUFFER_LEN + 1];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer)); // Empty the buffer
        int len = recv(client_socket, buffer, BUFFER_LEN, 0);
        buffer[len] = '\0';

        if (len <= 0)
        {
            printf("[SERVER] Connection closed %s:%i\n", inet_ntoa(client_address->sin_addr), ntohs(client_address->sin_port));
            close(client_socket);
            free(args);
            pthread_exit(NULL);
        }

        printf("[SERVER] Message received from %s:%i\n", inet_ntoa(client_address->sin_addr), ntohs(client_address->sin_port));

        // Lock the clients list mutex
        pthread_mutex_lock(&clientsMutex);

        unsigned pos = findClient(client_address, clients);
        if (pos < clients->size)
        {
            clients->list[pos].lastActivityTime = time(NULL);
            processClientMessage(client_socket, buffer, clients);
        }
        else
        {
            // Handle new client registration
            if (len < 2 || len + 1 >= SERVER_MAX_SIZE_LOGIN)
            {
                sendMessage(client_socket, "KO");
            }
            else
            {
                addClient(clients, *client_address, buffer, client_socket);
            }
        }

        // Unlock the clients list mutex
        pthread_mutex_unlock(&clientsMutex);
    }

    free(args);
    pthread_exit(NULL);
}

/**
 * @brief Processes the received client message based on its content
 * @param client_socket Socket of the client
 * @param buffer Received message
 * @param clients List of connected clients
 * @return void
 */
void processClientMessage(int client_socket, const char *buffer, clientList *clients)
{
    if (strncmp("GET maps/list", buffer, strlen("GET maps/list")) == 0)
    {
        char response[GET_QUERY_RESPONSE_SIZE + 1];
        queryGetMapsList responseToGetMapsList;

        if (!initialized)
        {
            initGetResponse(&responseToGetMapsList);
            initialized = 1;
        }

        char *jsonResponse = getResponseInJSON(&responseToGetMapsList);

        strncpy(response, jsonResponse, GET_QUERY_RESPONSE_SIZE);
        response[GET_QUERY_RESPONSE_SIZE] = '\0';
        sendMessage(client_socket, response);
    }
    else if (strncmp("exit", buffer, strlen("exit")) == 0)
    {
        unsigned pos = findClientBySocket(client_socket, clients);
        if (pos < clients->size)
        {
            sendMessage(client_socket, "Disconnected");

            if (close(client_socket) != 0)
            {
                handleError(SOCKET_CLOSE_ERROR);
            }
            clients->list[pos] = clients->list[--clients->size];
        }
    }
    else
    {
        char *response = getQueryErrorMessage(false);
        sendMessage(client_socket, response);
        free(response);
    }
}

/**
 * @brief Deletes inactive clients by referring to the value of lastActivityTime
 * @param clients List of connected clients
 * @return void
 */
void handleInactiveClients(clientList *clients)
{
    unsigned i = 0;
    while (i < clients->size)
    {
        if (time(NULL) - clients->list[i].lastActivityTime > INACTIVITY_TIMEOUT_SECONDS)
        {
            if (close(clients->list[i].client_socket) != 0)
            {
                handleError(SOCKET_CLOSE_ERROR);
            }

            clients->list[i] = clients->list[--clients->size];
        }
        else
        {
            i++;
        }
    }
}

/**
 * @brief Initialize thread arguments
 * @param client_socket Socket of the client
 * @param client_address Address of the client
 * @param clients List of connected clients
 * @return Initialized ThreadArgs structure
 */
ThreadArgs *initThreadArgs(int client_socket, struct sockaddr_in client_address, clientList *clients)
{
    ThreadArgs *args = malloc(sizeof(ThreadArgs));
    if (!args)
    {
        handleError(MALLOC_ERROR);
        return NULL;
    }

    args->client_socket = client_socket;
    args->client_address = client_address;
    args->clients = clients;

    return args;
}

/**
 * @brief Destroy the thread pool
 * @return void
 */
void destroyThreadPool()
{
    for (int i = THREAD_POOL_SIZE - 1; i >= 0; i--)
    {
        pthread_join(threads[i], NULL);
    }
    
    // Destroy the clients list mutex
    pthread_mutex_destroy(&clientsMutex);
}

/**
 * @brief Run the server and handles all incoming connections
 * @param server_socket Socket of the server
 * @param clients List of connected clients
 */
void runServer(int server_socket, clientList *clients)
{
    struct sockaddr_in client_address;
    socklen_t addr_len = sizeof(client_address);
    int client_socket, i;
    for (i = 0; i < THREAD_POOL_SIZE; i++)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_len);
        if (client_socket < 0)
        {
            handleError(ACCEPT_ERROR);
        }

        ThreadArgs *args = initThreadArgs(client_socket, client_address, clients);
        
        // Create a new thread to handle the connection
        if (pthread_create(&threads[i], NULL, handleClientThread, (void *)args) != 0)
        {
            handleError(THREAD_CREATION_ERROR);
        }

        // Detach the thread
        pthread_detach(threads[i]);
    }

    destroyThreadPool();
}

/**
 * @brief Cleanup the server by closing the server socket and freeing the clients list
 * @param server_socket Socket of the server
 * @param clients List of connected clients
 */
void cleanupServer(int server_socket, clientList *clients) {
    // Close the server socket
    if (close(server_socket) != 0) {
        handleError(SOCKET_CLOSE_ERROR);
    }

    // Free the clients list
    freeClientList(clients);
}
