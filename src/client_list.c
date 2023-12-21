#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <assert.h>
#include "server_utils.h"
#include "error_handler.h"
#include "client_list.h"

/**
 * @brief initialise the list of clients
 * @param clients the list of clients
 * @return void
 */
void addClient(clientList *clients, struct sockaddr_in addr, const char *login, int client_socket)
{
    if (clients->size >= clients->capacity)
    {
        clients->capacity += CLIENT_BLOC_SIZE;
        clientInfo *newList = realloc(
            clients->list, clients->capacity * sizeof(clientInfo));

        if (!newList)
        {
            handleError(REALLOC_ERROR);
        }

        clients->list = newList;
    }

    clients->list[clients->size].addr = addr;
    clients->list[clients->size].client_socket = client_socket;
    strncpy(clients->list[clients->size].login, login, MAX_LOGIN_LEN - 1);
    clients->list[clients->size].login[MAX_LOGIN_LEN - 1] = '\0';
    clients->list[clients->size].lastActivityTime = time(NULL);
    // clients->list[clients->size].playerId; id of the player in the game | in args
    // clients->list[clients->size].numPlayer; number of the player in the game
    clients->size++;

    printf("[SERVER] Add client %s:%i to clients list, his login is %s\n",
           inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), login);

    // Broadcast the new client login to all clients
    broadcastNewClient(clients, login);
}

/**
 * @brief find a client in the list of clients
 * @param client_address the address of the client to find
 * @param clients the list of clients
 * @return the index of the client in the list, or the size of the list if the client is not found
 */
unsigned findClient(const struct sockaddr_in *client_address, const clientList *clients)
{
    for (unsigned i = 0; i < clients->size; i++)
    {
        if (client_address->sin_addr.s_addr == clients->list[i].addr.sin_addr.s_addr &&
            client_address->sin_port == clients->list[i].addr.sin_port)
        {
            return i;
        }
    }
    return clients->size;
}

/**
 * @brief find a client (by it's socket) in the list of clients
 * @param client_socket the socket of the client to find
 * @param clients the list of clients
 * @return the index of the client in the list, or the size of the list if the client is not found
 */
unsigned findClientBySocket(int client_socket, const clientList *clients)
{
    for (unsigned i = 0; i < clients->size; i++)
    {
        if (client_socket == clients->list[i].client_socket)
        {
            return i;
        }
    }
    return clients->size;
}

void freeClientList(clientList *clients)
{
    if (clients == NULL)
    {
        return;
    }

    // Free the memory of each client's login
    for (unsigned int i = 0; i < clients->size; ++i)
    {
        free(clients->list[i].login);
    }

    free(clients->list);
    clients->list = NULL;
    clients->capacity = 0;
    clients->size = 0;
}

/**
 * @brief Broadcast new client information to all clients
 * @param clients the list of clients
 * @param newClientLogin the login of the new client
 * @return void
 */
void broadcastNewClient(const clientList *clients, const char *newClientLogin)
{
    for (unsigned i = 0; i < clients->size; i++)
    {
        // Instead of broadcasting to the new client, we say welcome to him
        if (strcmp(clients->list[i].login, newClientLogin) == 0)
        {
            // Say welcome to new player
            char welcomeMessage[strlen(WELCOME_MESSAGE_FORMAT) + strlen(newClientLogin) + 1];
            snprintf(welcomeMessage, sizeof(welcomeMessage), WELCOME_MESSAGE_FORMAT, newClientLogin);
            sendMessage(clients->list[i].client_socket, welcomeMessage);
            continue;
        }

        // Broadcast the message to each connected client
        char broadcastMessage[strlen(NEW_CLIENT_MESSAGE_FORMAT) + strlen(newClientLogin) + 1];
        snprintf(broadcastMessage, sizeof(broadcastMessage), NEW_CLIENT_MESSAGE_FORMAT, newClientLogin);
        sendMessage(clients->list[i].client_socket, broadcastMessage);
    }
}