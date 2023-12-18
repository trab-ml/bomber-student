#include <stdio.h>
#include <stdlib.h>
#include "socket_utils.h"

/**
 * @brief Initialise a TCP server socket.
 * @param port is the port to use.
 * @return the file descriptor.
 */
int initTCPServerSocket(int port)
{
    int server_socket = createServerSocket();
    setSocketOptions(server_socket);
    bindServerSocket(server_socket, port);
    listenOnSocket(server_socket);

    return server_socket;
}

/**
 * @brief Create a TCP server socket.
 * @return the file descriptor.
 */
int createServerSocket()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        handleError(SOCKET_CREATION_ERROR);
    }
    return server_socket;
}

/**
 * @brief Set socket options.
 * @param server_socket is the server socket file descriptor.
 */
void setSocketOptions(int server_socket)
{
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
    {
        handleError(SETSOCKOPT_ERROR);
    }
}

/**
 * @brief Bind the server socket.
 * @param server_socket is the server socket file descriptor.
 * @param port is the port to bind to.
 */
void bindServerSocket(int server_socket, int port)
{
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
    {
        handleError(BIND_ERROR);
    }
}

/**
 * @brief Listen on the server socket.
 * @param server_socket is the server socket file descriptor.
 */
void listenOnSocket(int server_socket)
{
    if (listen(server_socket, BACKLOG) != 0)
    {
        handleError(LISTEN_ERROR);
    }
}
