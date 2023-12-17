#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>
#include "client_list.h"
#include "error_handler.h"

const unsigned int BUFFER_LEN = 1024;
const unsigned int MAX_MISS = 128;
const unsigned int CLIENT_BLOC_SIZE = 1024;

/**
 * @brief initialise the list of clients
 * @param clients the list of clients
 * @return void
 */
void addClient(clientList *clients, struct sockaddr_in addr, const char *login, int client_socket)
{
    printf("[SERVER] Add client to clients list %s:%i, his login is %s\n",
           inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), login);

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
    clients->size++;
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

void freeClientList(clientList *clients) {
    if (clients == NULL) {
        return;
    }

    // Free the memory of each client's login
    for (unsigned int i = 0; i < clients->size; ++i) {
        free(clients->list[i].login);
    }

    // Free the memory of the list of clients
    free(clients->list);

    // Reset the client list
    clients->list = NULL;
    clients->capacity = 0;
    clients->size = 0;
}
