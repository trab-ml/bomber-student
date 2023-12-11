#include <stdio.h>
#include "error_handler.h"

void handleError(ErrorCode error)
{
    printf("[SERVER] Error: %s (code: %d)\n", getErrorMessage(error), error);
    exit(EXIT_FAILURE);
}

void handleFailedConnection(ErrorCode error)
{
    printf("[SERVER] Error: %s (code: %d)\n", getErrorMessage(error), error);
    exit(EXIT_FAILURE);
}

// void handleFailedConnexionError(int len, struct sockaddr_in *client_address, int client_socket) {
//     printf("[SERVER] Connection closed or receiving error from client %s:%i\n", inet_ntoa(client_address->sin_addr), ntohs(client_address->sin_port));
//     close(client_socket);
//     exit(EXIT_FAILURE);
// }

/**
 * @brief get the error message from the error code
 * @param error the error code
 * @return the error message
*/
const char *getErrorMessage(ErrorCode error)
{
    switch (error)
    {
    case INVALID_ARGUMENT_ERROR:
        return "Invalid argument error";
    case MALLOC_ERROR:
        return "Malloc error";
    case REALLOC_ERROR:
        return "Realloc error";
    case INIT_GET_RESPONSE_ERROR:
        return "Init Get response error";
    case ADD_MAP_ERROR:
        return "Add map error";
    case CJSON_CREATE_OBJECT_ERROR:
        return "cJSON create object error";
    case SOCKET_CREATION_ERROR:
        return "Socket creation error";
    case SETSOCKOPT_ERROR:
        return "setsockopt error";
    case BIND_ERROR:
        return "Bind error";
    case LISTEN_ERROR:
        return "Listen error";
    case ACCEPT_ERROR:
        return "Accept error";
    case SOCKET_RECV_ERROR:
        return "Socket receive error";
    case SOCKET_SEND_ERROR:
        return "Socket send error";
    case SOCKET_CLOSE_ERROR:
        return "Socket close error";
    case CLIENT_LIST_ADD_ERROR:
        return "Client list add error";
    case GET_QUERY_ERROR:
        return "Get query error";
    default:
        return "Unknown error";
    }
}