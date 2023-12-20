#ifndef TEST_CJSON_H
#define TEST_CJSON_H

#include "cJSON.h"
#include <stdbool.h>
#include "error_handler.h"

#define GAME_BOARD_SIZE 193
#define GET_QUERY_RESPONSE_SIZE 399
#define ERR_MSG_SIZE 18
#define INITIAL_CAPACITY 10

// Info about a map
typedef struct {
    int id;
    int width;
    int height;
    char *content;
} mapData;

// The mapslist
typedef struct {
    mapData *list;
    unsigned int capacity;
    unsigned int size;
} mapsData;

// Info about the GET maps/list query
typedef struct {
    mapsData mapList;
    int nbMapsList;
    char *action;
    char *statut;
    char *message;
} queryGetMapsList;

// Container for the database
typedef struct {
    queryGetMapsList mapsList;
    int is_initialized;
} DatabaseContainer;

void initGetResponse(queryGetMapsList *database);
void initializeDatabase();
void addMap(queryGetMapsList *database, mapData map);
void freeMapsData(mapsData *mapList);
char *parseInJSON(queryGetMapsList *database);
char *getResponseInJSON(queryGetMapsList *responseToGetMapsList);
char *getResponseInJSONFromDatabase();
char *getQueryErrorMessage(bool unknownErr);

#endif
