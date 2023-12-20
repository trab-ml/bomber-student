#include <stdio.h>
#include <stdbool.h>
#include "server_utils.h"
#include "query_handler.h"

/**
 * @brief Handle the GET maps/list query
 * @param clientIndex Index of the client
 * @param client_socket Socket of the client
 * @param clients List of connected clients
 */
void handleGetMapsList(ThreadArgs *threadArgs)
{
    char response[GET_QUERY_RESPONSE_SIZE + 1];
    initializeDatabase();

    char *jsonResponse = getResponseInJSONFromDatabase();

    strncpy(response, jsonResponse, GET_QUERY_RESPONSE_SIZE);
    response[GET_QUERY_RESPONSE_SIZE] = '\0';
    sendMessage(threadArgs->client_socket, response);
}

void handleGameList(ThreadArgs *threadArgs)
{
    printf("Handling game list\n");
}

void handleGameCreate(ThreadArgs *threadArgs)
{
    printf("Handling game create\n");
}

// int testmain(){
// 	int i,numPlayer = 2;
// 	game *g = init_game(numPlayer);
// 	pthread_t threads[numPlayer];
// 	g->numPlayers = numPlayer;
// 	player  **p = (player **)malloc(sizeof(player*) * numPlayer);
// 	for(i = 0;i<numPlayer;i++){
// 		p[i] = (player*)malloc(sizeof(player));
// 		init_player(p[i],g,i);
// 		get_position(p[i]);
// 		g->ap[i] = p[i];
// 		pthread_create(&threads[i],NULL,player_thread,(void *)g->ap[i]);
// 	}
// 	for (i = 0; i < numPlayer; ++i) {
// 		pthread_join(threads[i],NULL);
// 	}
// 	pthread_mutex_destroy(&g->lock);
// 	return 0;
// }

void handleGameJoin(ThreadArgs *threadArgs)
{
    clientList *clients = threadArgs->clients;
    // clients->list[clientsPos].playerId;
    
    printf("Handling game join\n");
}

void handlePlayerMove(ThreadArgs *threadArgs)
{
    printf("Handling player move\n");
}

void handleAttackBomb(ThreadArgs *threadArgs)
{
    printf("Handling attack bomb\n");
}

void handleAttackRemoteGo(ThreadArgs *threadArgs)
{
    printf("Handling attack remote go\n");
}

void handleObjectNew(ThreadArgs *threadArgs)
{
    printf("Handling object new\n");
}

void handleExit(ThreadArgs *threadArgs)
{
    clientList *clients = threadArgs->clients;
    printf("Closing socket\n");
    if (close(threadArgs->client_socket) != 0)
    {
        printf("Error closing socket\n");
        handleError(SOCKET_CLOSE_ERROR);
    }
    clients->list[threadArgs->client_index] = clients->list[--clients->size];
}

void handleUnsupported(int client_socket)
{
    printf("Handling unsupported request\n");
}

/**
 * @brief Process the received client message based on its content
 * @param client_socket Socket of the client
 * @param buffer Received message
 * @param clients List of connected clients
 * @return void
 */
void processClientMessage(ThreadArgs *threadArgs)
{
    printf("[SERVER] (%ld chars) in buffer: `%s`\n", strlen(threadArgs->buffer), threadArgs->buffer);
    // printf("[SERVER] (%ld chars) expected for GET_MAPS_LIST\n", strlen(GET_MAPS_LIST_QUERY));
    int i, queryLen;
    for (i = 0; handlers[i].query != NULL; i++)
    {
        // printf("[SERVER] Comparing `%s` with `%s`\n", threadArgs->buffer, handlers[i].query);
        queryLen = strlen(handlers[i].query);
        if ((strcmp(threadArgs->buffer, handlers[i].query) == 0)
            || (strlen(threadArgs->buffer) - 1 == queryLen && strncmp(threadArgs->buffer, handlers[i].query, queryLen) == 0)) // -1 to remove \n of the terminal
        {
            handlers[i].handler(threadArgs);
            return;
        }
    }

    handleUnsupported(threadArgs->client_socket);
}