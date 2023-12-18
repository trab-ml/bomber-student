#include <stdio.h>
#include <stdbool.h>
#include "server_utils.h"
#include "query_handler.h"

// Global flag to track initialization status
int initialized = 0;

/**
 * @brief Handle the GET maps/list query
 * @param clientIndex Index of the client
 * @param client_socket Socket of the client
 * @param clients List of connected clients
*/
void handleGetMapsList(int clientIndex, int client_socket, clientList *clients)
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

void handleGameList(int clientIndex, int client_socket, clientList *clients) {
    printf("Handling game list\n");
}

void handleGameCreate(int clientIndex, int client_socket, clientList *clients) {
    printf("Handling game create\n");
}

void handleGameJoin(int clientIndex, int client_socket, clientList *clients) {
    printf("Handling game join\n");
}

void handlePlayerMove(int clientIndex, int client_socket, clientList *clients) {
    printf("Handling player move\n");
}

void handleAttackBomb(int clientIndex, int client_socket, clientList *clients) {
    printf("Handling attack bomb\n");
}

void handleAttackRemoteGo(int clientIndex, int client_socket, clientList *clients) {
    printf("Handling attack remote go\n");
}

void handleObjectNew(int clientIndex, int client_socket, clientList *clients) {
    printf("Handling object new\n");
}

void handleExit(int clientIndex, int client_socket, clientList *clients)
{
    printf("Closing socket\n");
    if (close(client_socket) != 0)
    {
        printf("Error closing socket\n");
        handleError(SOCKET_CLOSE_ERROR);
    }
    clients->list[clientIndex] = clients->list[--clients->size];
}

void handleUnsupported(int client_socket)
{
    printf("Handling unsupported request\n");
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
    unsigned clientIndex = findClientBySocket(client_socket, clients);
    if (clientIndex <= clients->size)
    {
        int i;
        for (i = 0; handlers[i].query != NULL; ++i)
        {
            int queryLen = strlen(handlers[i].query);
            if (strlen(buffer) - 1 == queryLen && strncmp(buffer, handlers[i].query, queryLen) == 0) // -1 to ignore \n
            {
                handlers[i].handler(clientIndex, client_socket, clients);
                return;
            }
        }
    }

    handleUnsupported(client_socket);
}