#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "socket_utils.h"
#include "client_list.h"
#include "test_cJSON.h"
#include <unistd.h>
#include <time.h>

#define SERVER_MAX_SIZE_LOGIN 10
#define WELCOME_MESSAGE "Dites bonjour à "
#define INACTIVITY_TIMEOUT_SECONDS 60 * 10 // Temps d'inactivité autorisé avant déconnexion

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

void startServer(int server_socket, clientList *clients);
void handleNewConnection(int client_socket, struct sockaddr_in *client_address, clientList *clients);
void processClientMessage(int client_socket, const char *buffer, clientList *clients);
void handleInactiveClients(clientList *clients);

int main(int argc, char **argv)
{
    assert(argc == 2);
    int port = atoi(argv[1]);

    int server_socket = initTCPServerSocket(port);
    clientList clients = {NULL, 0, 0};

    startServer(server_socket, &clients);

    return EXIT_FAILURE;
}

void startServer(int server_socket, clientList *clients) {
    struct sockaddr_in client_address;
    socklen_t addr_len = sizeof(client_address);

    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_len);
        if (client_socket == -1) {
            handleError("Problème lors de l'acceptation de la connexion");
            exit(EXIT_FAILURE);
        }

        printf("[SERVER] Avant handleNewConnection\n");
        handleNewConnection(client_socket, &client_address, clients);
        printf("[SERVER] Après handleNewConnection\n");

        // Suppression des clients inactifs
        handleInactiveClients(clients);
    }
}

void handleNewConnection(int client_socket, struct sockaddr_in *client_address, clientList *clients)
{
    char buffer[BUFFER_LEN + 1];
    int len = recv(client_socket, buffer, BUFFER_LEN, 0);

    if (len == 0)
    {
        // Connexion fermée par le client
        printf("[SERVER] Connexion fermée par le client %s:%i\n", inet_ntoa(client_address->sin_addr), ntohs(client_address->sin_port));
        close(client_socket);
        return;
    }
    else if (len < 0)
    {
        // Erreur lors de la réception
        handleError("Problème à la réception");
        close(client_socket);
        return;
    }

    printf("[SERVER] Nouvelle connexion de %s:%i\n", inet_ntoa(client_address->sin_addr), ntohs(client_address->sin_port));

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

        printf("Processing Client Msg\n");

        processClientMessage(client_socket, buffer, clients);
    }
}

void processClientMessage(int client_socket, const char *buffer, clientList *clients)
{
    if (strncmp("GET maps/list", buffer, strlen("GET maps/list")) == 0)
    {
        char *jsonResponse = malloc(GET_QUERY_RESPONSE_SIZE * sizeof(char));
        char response[GET_QUERY_RESPONSE_SIZE + 1];
        queryGetMapsList responseToGetMapsList;

        jsonResponse = getResponseInJSON(&responseToGetMapsList);
        response[GET_QUERY_RESPONSE_SIZE] = '\0';
        strcpy(response, jsonResponse);

        free(jsonResponse);
        sendMessage(client_socket, response);
    }
    else if (strncmp("DISCONNECT", buffer, strlen("DISCONNECT")) == 0)
    {
        sendMessage(client_socket, "Disconnected");
        close(client_socket);
    }
    else
    {
        sendMessage(client_socket, "Received your message");
    }
}

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
