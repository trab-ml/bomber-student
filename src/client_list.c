#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>
#include "client_list.h"

const unsigned int BACKLOG = 7;
const unsigned int BUFFER_LEN = 1024;
const unsigned int MAX_MISS = 128;
const unsigned int CLIENT_BLOC_SIZE = 1024;

/**
 * @brief Ajoute un client à la liste.
 *
 * @param clients est la liste des clients.
 * @param addr est l'adresse IP du client.
 * @param login est une chaîne de caractères permettant de nommer l'utilisateur.
 */
void addClient(clientList *clients, struct sockaddr_in addr, char *login)
{
    printf("[SERVER] Add a new client to the list %s:%i, his login is %s\n", 
        inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), login);

    if (clients->size >= clients->capacity)
    {
        clients->capacity += CLIENT_BLOC_SIZE;
        clients->list = (clientInfo *)realloc(
            clients->list, clients->capacity * sizeof(clientInfo));
        if (!clients->list)
        {
            perror("Erreur malloc");
            exit(EXIT_FAILURE);
        }
    }

    clients->list[clients->size].addr = addr;
    clients->list[clients->size].counter = MAX_MISS;
    strcpy(clients->list[clients->size].login, login);
    assert(!strcmp(clients->list[clients->size].login, login));

    clients->size++;
} // addClient