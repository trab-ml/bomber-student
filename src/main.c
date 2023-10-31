#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "socket_utils.h"
#include "client_list.h"

// gcc src/main.c src/socket_utils.c src/client_list.c -Iinclude -o bin/main 

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
                    // specify to the client we got an error.
                    if (sendto(fdsocket, "", 0, MSG_DONTWAIT,
                               (struct sockaddr *)&clientAdresse, addrLen) < 0)
                    {
                        perror("Problème à l'envoie");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // specify to the client that everything is OK.
                    if (sendto(fdsocket, " ", 1, MSG_DONTWAIT,
                               (struct sockaddr *)&clientAdresse, addrLen) < 0)
                    {
                        perror("Problème à l'envoie");
                        exit(EXIT_FAILURE);
                    }

                    // nouveau client
                    buffer[len - 1] = '\0';

                    // on prévient tous le monde que le nouveau client vient d'arriver.
                    char message[strlen(buffer) + strlen("Dite bonjour à ") + 1];
                    strcpy(message, "Dite bonjour à ");
                    strcat(message, buffer);

                    // on previent tous le monde.
                    for (unsigned i = 0; i < clients.size; i++)
                    {
                        if (sendto(fdsocket, message, strlen(message), MSG_DONTWAIT,
                                   (struct sockaddr *)&clients.list[i].addr, addrLen) < 0)
                        {
                            perror("Problème à l'envoie");
                            exit(EXIT_FAILURE);
                        }
                    }

                    // on l'ajoute dans la liste.
                    addClient(&clients, clientAdresse, buffer);
                }
            }
            else
            {
                // Ajout du terminateur de chaîne
                buffer[len] = '\0';
                char response[strlen(clients.list[pos].login) + 3 + strlen(buffer) + 1];
                strcpy(response, clients.list[pos].login);
                strcat(response, " : ");
                strcat(response, buffer);

                // On broadcast le message.
                for (unsigned i = 0; i < clients.size; i++)
                    if (i != pos)
                    {
                        clients.list[i].counter--;
                        if (sendto(fdsocket, response, strlen(response), MSG_DONTWAIT,
                                   (struct sockaddr *)&clients.list[i].addr, addrLen) < 0)
                        {
                            perror("Problème à l'envoie");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                        clients.list[i].counter = MAX_MISS;
            }
        }

        // remove the ghosts.
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