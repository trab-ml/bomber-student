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

bool isConnectionClosed(int len, const struct sockaddr_in *client_address, ThreadArgs *args) {
    if (len <= 0) {
        printf("[SERVER] Connection closed %s:%i\n", inet_ntoa(client_address->sin_addr), ntohs(client_address->sin_port));
        close(args->client_socket);
        free(args);
        pthread_exit(NULL);
        return true;
    }
    return false;
}

void sendMessage(int client_socket, const char *message)
{
    if (send(client_socket, message, strlen(message), 0) < 0)
    {
        handleError(SOCKET_SEND_ERROR);
    }
}

void sendMessageAndExit(ThreadArgs *args, const char *message) {
    sendMessage(args->client_socket, message);
    close(args->client_socket);
    free(args);
    pthread_exit(NULL);
}

bool validateMessageLength(ThreadArgs *args, char *buffer, const char *expectedMessage, const char *acknowledgmentOfReceipt) {
    int expectedMsgLen = strlen(expectedMessage);
    int currentBufferLen = strlen(buffer);

    if (currentBufferLen - 1 != expectedMsgLen || strncmp(buffer, expectedMessage, expectedMsgLen) != 0) {
        return false;
    }

    sendMessage(args->client_socket, acknowledgmentOfReceipt);
    return true;
}

/**
 * @brief Ensure that the client is looking for a bomberstudent server
 * @param args Thread arguments
 * @param buffer Received message
 * @return true if the client is looking for a bomberstudent server, false otherwise
*/
bool listenClientsLookingForServer(ThreadArgs *args, char *buffer) {
    memset(buffer, 0, BUFFER_LEN + 1);
    int len = recv(args->client_socket, buffer, BUFFER_LEN, 0);
    buffer[len] = '\0';

    struct sockaddr_in *client_address = &(args->client_address);

    if (isConnectionClosed(len, client_address, args)) {
        return false;
    }

    return validateMessageLength(args, buffer, LOOKING_FOR_BOMBERSTUDENT_SERVER_MESSAGE, "\nhello i’m a bomberstudent server\n");
}

/**
 * @brief Handle a new client connection by reading messages and processing them
 * @param client_socket Socket of the client
 * @param client_address Address of the client
 * @param clients List of connected clients
 * @return (void *)
 */
void *handleClientThread(void *args)
{
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int client_socket = threadArgs->client_socket;
    struct sockaddr_in *client_address = &(threadArgs->client_address);
    clientList *clients = threadArgs->clients;

    char buffer[BUFFER_LEN + 1];
    int len;

    // ensure that the client is looking for a bomberstudent server
    if (listenClientsLookingForServer(threadArgs, buffer))
    {
        while (true)
        {
            memset(buffer, 0, sizeof(buffer)); // Empty the buffer
            len = recv(client_socket, buffer, BUFFER_LEN, 0);
            buffer[len] = '\0';

            isConnectionClosed(len, client_address, threadArgs);

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
                if (len + 1 >= CLIENT_LOGIN_SIZE)
                {
                    sendMessage(threadArgs->client_socket, "\nlogin too long!");
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

    return handleClientThread(args);
}

/**
 * @brief Process the received client message based on its content
 * @param client_socket Socket of the client
 * @param buffer Received message
 * @param clients List of connected clients
 * @return void
 */
void processClientMessage(int client_socket, const char *buffer, clientList *clients)
{
    char getMapQuery[] = "GET maps/list";
    char exitQuery[] = "exit";
    int getMapQueryLen = strlen(getMapQuery), exitQueryLen = strlen(exitQuery), currentBufferLen = strlen(buffer);

    if (currentBufferLen - 1 == getMapQueryLen && strncmp(buffer, getMapQuery, getMapQueryLen) == 0)
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
    else if (currentBufferLen - 1 == exitQueryLen && strncmp(buffer, exitQuery, exitQueryLen) == 0)
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
 * @brief Delete inactive clients by referring to the value of lastActivityTime
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
void cleanupServer(int server_socket, clientList *clients)
{
    // Close the server socket
    if (close(server_socket) != 0)
    {
        handleError(SOCKET_CLOSE_ERROR);
    }

    // Free the clients list
    freeClientList(clients);
}
