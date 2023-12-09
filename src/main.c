#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "socket_utils.h"
#include "client_list.h"
#include "server_utils.h"

int main(int argc, char **argv)
{
    assert(argc == 2);
    int port = atoi(argv[1]);

    int server_socket = initTCPServerSocket(port);
    clientList clients = {NULL, 0, 0};

    startServer(server_socket, &clients);

    // On ne devrait jamais arriver ici
    return EXIT_FAILURE;
}
