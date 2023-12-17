#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "socket_utils.h"
#include "client_list.h"
#include "server_utils.h"
#include "error_handler.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        handleError(INVALID_ARGUMENT_ERROR);
    }

    int port = atoi(argv[1]);
    int server_socket = initTCPServerSocket(port);
    clientList clients = {NULL, 0, 0};
    runServer(server_socket, &clients);

    cleanupServer(server_socket, &clients);
    return EXIT_FAILURE;
}
