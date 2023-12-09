#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include "socket_utils.h"

#define BACKLOG 5

/**
 * @brief Initialise un socket.
 *
 * @param port est le port utilisé.
 * @return la file descriptor.
 */
int initTCPServerSocket(int port)
{
    int server_socket;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Échec de création du socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
    {
        perror("Échec de paramétrage du socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
    {
        perror("Échec d'attachement au port");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, BACKLOG) != 0)
    {
        perror("Échec de configuration du backlog");
        exit(EXIT_FAILURE);
    }

    return server_socket;
}
