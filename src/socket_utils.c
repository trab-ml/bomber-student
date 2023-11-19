#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include "socket_utils.h"

/**
 * @brief Initialise un socket.
 *
 * @param port est le port utilisé.
 * @return la file descriptor.
 */
int initSocket(int port)
{
    int fdsocket;
    if ((fdsocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Échèc de création du socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(fdsocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
    {
        perror("Échèc de paramétrage du socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in adresse;

    adresse.sin_family = AF_INET;
    adresse.sin_addr.s_addr = INADDR_ANY;
    adresse.sin_port = htons(port);

    if (bind(fdsocket, (struct sockaddr *)&adresse, sizeof(adresse)) != 0)
    {
        perror("Échèc d'attachement au port");
        exit(EXIT_FAILURE);
    }

    return fdsocket;
}
