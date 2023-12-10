#ifndef TEST_CJSON_H
#define TEST_CJSON_H

#include "cJSON.h"
#include <stdbool.h>

#define GAME_BOARD_SIZE 193
#define GET_QUERY_RESPONSE_SIZE 399
#define ERR_MSG_SIZE 18
#define INITIAL_CAPACITY 10

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

int initGetResponse(queryGetMapsList *database);
int addMap(queryGetMapsList *database, mapData map);
void freeMapsData(mapsData *mapList);
char *parseInJSON(queryGetMapsList *database);
char *getResponseInJSON(queryGetMapsList *responseToGetMapsList);
char *getErrorMessage(bool unknownErr);

#endif
