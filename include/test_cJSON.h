#ifndef TEST_CJSON_H
#define TEST_CJSON_H

#include "cJSON.h"
#include <stdbool.h>

// Taille maximale pour un plateau du jeu
#define GAME_BOARD_SIZE 193
#define GET_QUERY_RESPONSE_SIZE 399
// Taille max pour un message d'erreur
#define ERR_MSG_SIZE 18

// Infos sur une map
typedef struct {
    int id;
    int width;
    int height;
    char *content;
} mapData;

// Infos sur une liste de maps
typedef struct {
    mapData *list;
    unsigned int capacity;
    unsigned int size;
} mapsData;

// Lorsque le client fait : GET maps/list
typedef struct {
    mapsData mapList;
    int nbMapsList;
    char *action;
    char *statut;
    char *message;
} queryGetMapsList;

// Initialise la base de données
int initGetResponse(queryGetMapsList *database);

// Ajoute une map à la liste de map
int addMap(queryGetMapsList *database, mapData map);

// Libère la mémoire occupée par la liste de maps
void freeMapsData(mapsData *mapList);

char *getResponseInJSON(queryGetMapsList *responseToGetMapsList);

char *getErrorMessage(bool unknowErr);

#endif
