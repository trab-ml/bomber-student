#ifndef TEST_CJSON_H
#define TEST_CJSON_H

#include "cJSON.h"

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
int initGetResponse(queryGetMapsList *database)

// Ajoute une map à la liste de map
int addMap(queryGetMapsList *database, mapData map);

// Libère la mémoire occupée de la liste de maps
void freeMapsData(mapsData *mapList);

#endif
