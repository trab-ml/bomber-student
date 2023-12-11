#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include "socket_utils.h"
#include "error_handler.h"

#define BACKLOG 5

/**
 * @brief Initialise a TCP server socket.
 * @param port is the port to use.
 * @return the file descriptor.
 */
int initTCPServerSocket(int port)
{
    int server_socket;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        handleError(SOCKET_CREATION_ERROR);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
    {
        handleError(SETSOCKOPT_ERROR);
    }

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
    {
        handleError(BIND_ERROR);
    }

    if (listen(server_socket, BACKLOG) != 0)
    {
        handleError(LISTEN_ERROR);
    }

    return server_socket;
}
