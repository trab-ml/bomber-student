#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "socket_utils.h"
#include "client_list.h"
#include "test_cJSON.h"
#include "server_utils.h"

void handleError(const char *message)
{
    perror(message);
}

void sendMessage(int client_socket, const char *message)
{
    if (send(client_socket, message, strlen(message), 0) < 0)
    {
        handleError("Problème à l'envoi");
    }
}

void startServer(int server_socket, clientList *clients)
{
    struct sockaddr_in client_address;
    socklen_t addr_len = sizeof(client_address);

    while (1)
    {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_len);
        if (client_socket == -1)
        {
            handleError("Problème lors de l'acceptation de la connexion");
            exit(EXIT_FAILURE);
        }

        printf("[SERVER] Nouvelle connexion de %s:%i\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        handleNewConnection(client_socket, &client_address, clients);
        handleInactiveClients(clients);
    }
}

void handleNewConnection(int client_socket, struct sockaddr_in *client_address, clientList *clients)
{
    while (1)
    {
        char buffer[BUFFER_LEN + 1];
        int len = recv(client_socket, buffer, BUFFER_LEN, 0);

        if (len <= 0)
        {
            // Connexion fermée ou erreur lors de la réception
            printf("[SERVER] Connexion fermée ou erreur de réception par le client %s:%i\n", inet_ntoa(client_address->sin_addr), ntohs(client_address->sin_port));
            close(client_socket);
            return;
        }

        printf("[SERVER] Message reçu de %s:%i\n", inet_ntoa(client_address->sin_addr), ntohs(client_address->sin_port));

        unsigned pos = findClient(client_address, clients);
        if (pos >= clients->size)
        {
            if (len < 2 || len + 1 >= SERVER_MAX_SIZE_LOGIN)
            {
                sendMessage(client_socket, "KO");
            }
            else
            {
                addClient(clients, *client_address, buffer, client_socket);
            }
        }
        else
        {
            buffer[len] = '\0';
            clients->list[pos].lastActivityTime = time(NULL);
            processClientMessage(client_socket, buffer, clients);
        }
    }
}

/**
 * @brief Traite le message reçu du client
 * @param client_socket Socket du client
 * @param buffer Message reçu
 * @param clients Liste des clients connectés
 * @return void
 */
void processClientMessage(int client_socket, const char *buffer, clientList *clients)
{
    if (strncmp("GET maps/list", buffer, strlen("GET maps/list")) == 0)
    {
        char response[GET_QUERY_RESPONSE_SIZE + 1];
        queryGetMapsList responseToGetMapsList;
        char *jsonResponse = getResponseInJSON(&responseToGetMapsList);

        strncpy(response, jsonResponse, GET_QUERY_RESPONSE_SIZE);
        response[GET_QUERY_RESPONSE_SIZE] = '\0';
        sendMessage(client_socket, response);
    }
    else if (strncmp("DISC", buffer, strlen("DISC")) == 0)
    {
        unsigned pos = findClientBySocket(client_socket, clients);
        if (pos < clients->size)
        {
            sendMessage(client_socket, "Disconnected");

            if (close(client_socket) != 0)
            {
                handleError("Problème lors de la fermeture du socket client");
                exit(EXIT_FAILURE);
            }
            clients->list[pos] = clients->list[--clients->size];
        }
    }
    else
    {
        char *response = malloc(ERR_MSG_SIZE * sizeof(char));
        response = getErrorMessage(false);

        sendMessage(client_socket, response);
        free(response);
    }
}

/**
 * @brief Supprime les clients inactifs en se réferant à la valeur de lastActivityTime
 * @param clients Liste des clients connectés
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
                handleError("Problème lors de la fermeture du socket client");
                exit(EXIT_FAILURE);
            }

            clients->list[i] = clients->list[--clients->size];
        }
        else
        {
            i++;
        }
    }
}
