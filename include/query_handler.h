#ifndef QUERY_HANDLER_H
#define QUERY_HANDLER_H

#include "client_list.h"

#define DISCONNECTED_MESSAGE "Disconnected"

// Handler function prototypes
void handleGetMapsList(int clientIndex, int client_socket, clientList *clients);
void handleGameList(int clientIndex, int client_socket, clientList *clients);
void handleGameCreate(int clientIndex, int client_socket, clientList *clients);
void handleGameJoin(int clientIndex, int client_socket, clientList *clients);
void handlePlayerMove(int clientIndex, int client_socket, clientList *clients);
void handleAttackBomb(int clientIndex, int client_socket, clientList *clients);
void handleAttackRemoteGo(int clientIndex, int client_socket, clientList *clients);
void handleObjectNew(int clientIndex, int client_socket, clientList *clients);
void handleExit(int clientIndex, int client_socket, clientList *clients);
void handleUnsupported(int client_socket);
void processClientMessage(int client_socket, const char *buffer, clientList *clients);

// Constants for (query, handler) pairs
#define GET_MAPS_LIST_QUERY "GET maps/list"
#define GAME_LIST_QUERY "GET game/list"
#define GAME_CREATE_QUERY "POST game/create"
#define GAME_JOIN_QUERY "POST game/join"
#define PLAYER_MOVE_QUERY "POST player/move"
#define ATTACK_BOMB_QUERY "POST attack/bomb"
#define ATTACK_REMOTE_GO_QUERY "POST attack/remote/go"
#define OBJECT_NEW_QUERY "POST object/new"
#define EXIT_QUERY "exit"

// Struct for (query, handler) pairs
typedef struct {
    const char *query;
    void (*handler)(int, int, clientList*);
} QueryHandler;

// Handlers array
QueryHandler handlers[] = {
    {GET_MAPS_LIST_QUERY, handleGetMapsList},
    {GAME_LIST_QUERY, handleGameList},
    {GAME_CREATE_QUERY, handleGameCreate},
    {GAME_JOIN_QUERY, handleGameJoin},
    {PLAYER_MOVE_QUERY, handlePlayerMove},
    {ATTACK_BOMB_QUERY, handleAttackBomb},
    {ATTACK_REMOTE_GO_QUERY, handleAttackRemoteGo},
    {OBJECT_NEW_QUERY, handleObjectNew},
    {EXIT_QUERY, handleExit},
    {NULL, NULL}
};

#endif
