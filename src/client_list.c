#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>
#include "client_list.h"
#include "error_handler.h"

const unsigned int BACKLOG = 7;
const unsigned int BUFFER_LEN = 1024;
const unsigned int MAX_MISS = 128;
const unsigned int CLIENT_BLOC_SIZE = 1024;

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
    strcpy(clients->list[clients->size].login, login);
    clients->list[clients->size].lastActivityTime = time(NULL);
    clients->size++;
}

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
