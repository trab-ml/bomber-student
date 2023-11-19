#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "socket_utils.h"
#include "client_list.h"
#include "test_cJSON.h"

// gcc src/main.c src/socket_utils.c src/client_list.c src/test_cJSON.c src/cJSON.c -Iinclude -o bin/main
// [SERVER] bin/main 49151
// OR
// [SERVER] make; bin/main 49151
//
// [CLIENT] nc -u localhost 49151

#define SERVER_MAX_SIZE_LOGIN 10

int main(int argc, char **argv)
{
    assert(argc == 2);
    int port = atoi(argv[1]);

    int fdsocket = initSocket(port);
    clientList clients = {NULL, 0, 0};

    struct sockaddr_in clientAdresse;
    char buffer[BUFFER_LEN + 1];
    unsigned int addrLen = sizeof(clientAdresse);

    while (1)
    {
        // On vérifie si on a reçu un message
        int len = recvfrom(fdsocket, buffer, BUFFER_LEN, MSG_WAITALL,
                           (struct sockaddr *)&clientAdresse, &addrLen);
        if (len < 0)
        {
            perror("Problème à la réception");
            exit(EXIT_FAILURE);
        }

        printf("[SERVER] Receive %s:%i\n", inet_ntoa(clientAdresse.sin_addr),
               ntohs(clientAdresse.sin_port));

        if (len == -1 && errno != EAGAIN)
        {
            perror("Problème de socket");
            exit(EXIT_FAILURE);
        }
        else if (len > 0)
        {
            // check si le client est déjà connu.
            unsigned pos = 0;
            while (pos < clients.size &&
                   (clientAdresse.sin_addr.s_addr !=
                        clients.list[pos].addr.sin_addr.s_addr ||
                    clientAdresse.sin_port != clients.list[pos].addr.sin_port))
            {
                pos++;
            }

            if (pos >= clients.size)
            {
                if (len < 2 || len + 1 >= SERVER_MAX_SIZE_LOGIN)
                {
                    // Spécifie au client qu'il y'a erreur.
                    if (sendto(fdsocket, "", 0, MSG_DONTWAIT,
                               (struct sockaddr *)&clientAdresse, addrLen) < 0)
                    {
                        perror("Problème à l'envoie");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // Spécifie au client que tout est OK.
                    if (sendto(fdsocket, " ", 1, MSG_DONTWAIT,
                               (struct sockaddr *)&clientAdresse, addrLen) < 0)
                    {
                        perror("Problème à l'envoie");
                        exit(EXIT_FAILURE);
                    }

                    // Ajoute le terminateur de chaîne
                    buffer[len] = '\0';

                    // on prévient tout le monde que le nouveau client vient d'arriver.
                    char message[strlen("Dite bonjour à ") + strlen(buffer) + 1];
                    strcpy(message, "Dite bonjour à ");
                    strcat(message, buffer);

                    // on prévient tout le monde.
                    for (unsigned i = 0; i < clients.size; i++)
                    {
                        if (sendto(fdsocket, message, strlen(message), MSG_DONTWAIT,
                                   (struct sockaddr *)&clients.list[i].addr, addrLen) < 0)
                        {
                            perror("Problème à l'envoie");
                            exit(EXIT_FAILURE);
                        }
                    }

                    // on l'ajoute à la liste.
                    addClient(&clients, clientAdresse, buffer);
                }
            }
            else
            {
                // Ajoute le terminateur de chaîne
                buffer[len] = '\0';
                // Le client a t-il fait une requête "GET maps/list" ?
                if (strncmp("GET maps/list", buffer, strlen("GET maps/list")) == 0)
                {
                    char *jsonResponse = malloc(GET_QUERY_RESPONSE_SIZE * sizeof(char));
                    char response[GET_QUERY_RESPONSE_SIZE + 1];
                    queryGetMapsList responseToGetMapsList;
                    
                    jsonResponse = getResponseInJSON(&responseToGetMapsList);
                    response[GET_QUERY_RESPONSE_SIZE] = '\0';
                    strcpy(response, jsonResponse);
                    // strcat(response, buffer);

                    // Libère la mémoire allouée 
                    free(jsonResponse);

                    if (sendto(fdsocket, response, strlen(response), MSG_DONTWAIT,
                               (struct sockaddr *)&clients.list[pos].addr, addrLen) < 0)
                    {
                        perror("Problème à l'envoie");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // char response[] = "Bad Request !\n"; // or Unknown Error !

                    int checkSend = -1;
                    char *response = malloc(ERR_MSG_SIZE * sizeof(char));

                    response = getErrorMessage(false);
                    checkSend = sendto(fdsocket, response, strlen(response), MSG_DONTWAIT,
                               (struct sockaddr *)&clients.list[pos].addr, addrLen);

                    // Libère la mémoire allouée 
                    free(response);

                    if (checkSend < 0)
                    {
                        perror("Problème à l'envoie");
                        exit(EXIT_FAILURE);
                    }

                    // when it should be "Unknown Error !"?
                }
            }
        }

        // Supprime les clients inactifs (déconnectés).
        for (unsigned i = 0; i < clients.size;)
        {
            if (clients.list[i].counter > 0)
                i++;
            else
                clients.list[i] = clients.list[--clients.size];
        }
    }

    return EXIT_FAILURE;
}