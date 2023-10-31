#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

// gcc -o bin/testcJSON src/testcJSON2.c src/cJSON.c -Iinclude -lm
//      (src/cJSON.c pour lier le code cJSON à la compilation)
// ./bin/testcJSON2

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

// Fonction pour ajouter une map à la liste
int addMap(queryGetMapsList *database, mapData map) {
    if (database->mapList.size >= database->mapList.capacity) {
        database->mapList.capacity += 10;
        mapData *newList = (mapData *)realloc(database->mapList.list, database->mapList.capacity * sizeof(mapData));
        if (!newList) {
            perror("Erreur realloc");
            return 0; // Échec
        }
        database->mapList.list = newList;
    }

    database->mapList.list[database->mapList.size] = map;
    database->mapList.size++;
    return 1; // Réussite
}

// Fonction pour libérer la mémoire de la liste de maps
void freeMapsData(mapsData *mapList) {
    if (mapList->list) {
        for (unsigned int i = 0; i < mapList->size; i++) {
            free(mapList->list[i].content);
        }
        free(mapList->list);
    }
}

int main() {
    const char *jsonStr = "{"
                          "\"action\":\"maps/list\","
                          "\"statut\":\"200\","
                          "\"message\":\"ok\","
                          "\"nbMapsList\": 1,"
                          "\"maps\":["
                          "{"
                          "\"id\":0,"
                          "\"width\":24,"
                          "\"height\":8,"
                          "\"content\":\" ************************"
                          "=----------------------="
                          "=----==============----="
                          "=----------****--------="
                          "=------****------------="
                          "=----==============----="
                          "=----------------------="
                          "************************\""
                          "}"
                          "]"
                          "}";

    cJSON *root = cJSON_Parse(jsonStr);
    if (root == NULL) {
        printf("Erreur lors de la désérialisation du JSON.\n");
        return 1;
    }

    queryGetMapsList responseToGetMapsList;

    responseToGetMapsList.action = cJSON_GetObjectItem(root, "action")->valuestring;
    responseToGetMapsList.statut = cJSON_GetObjectItem(root, "statut")->valuestring;
    responseToGetMapsList.message = cJSON_GetObjectItem(root, "message")->valuestring;
    responseToGetMapsList.nbMapsList = cJSON_GetObjectItem(root, "nbMapsList")->valueint;

    cJSON *mapsArray = cJSON_GetObjectItem(root, "maps");
    cJSON *mapItem = cJSON_GetArrayItem(mapsArray, 0);

    mapData map;
    map.id = cJSON_GetObjectItem(mapItem, "id")->valueint;
    map.width = cJSON_GetObjectItem(mapItem, "width")->valueint;
    map.height = cJSON_GetObjectItem(mapItem, "height")->valueint;
    map.content = strdup(cJSON_GetObjectItem(mapItem, "content")->valuestring);

    if (!addMap(&responseToGetMapsList, map)) {
        // Gestion de l'erreur lors de l'ajout de la map
        freeMapsData(&responseToGetMapsList.mapList);
        cJSON_Delete(root);
        return 1;
    }

    cJSON_Delete(root); // Libération de la mémoire

    // Utilisez responseToGetMapsList ici
    // Maintenant, responseToGetMapsList contient les données de la réponse JSON dans une structure C
    // Vous pouvez accéder aux données de la liste de maps comme ceci :
    for (unsigned int i = 0; i < responseToGetMapsList.mapList.size; i++) {
        printf("ID de la map %d : %d\n", i, responseToGetMapsList.mapList.list[i].id);
        printf("Largeur de la map %d : %d\n", i, responseToGetMapsList.mapList.list[i].width);
        printf("Hauteur de la map %d : %d\n", i, responseToGetMapsList.mapList.list[i].height);
        printf("Contenu de la map %d :\n%s\n", i, responseToGetMapsList.mapList.list[i].content);
    }


    // Avant de quitter le programme, assurez-vous de libérer la mémoire de la liste de maps
    freeMapsData(&responseToGetMapsList.mapList);

    return 0;
}
