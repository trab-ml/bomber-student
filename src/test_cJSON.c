#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_cJSON.h"

int initGetResponse(queryGetMapsList *database)
{
    database->mapList.list = malloc(INITIAL_CAPACITY * sizeof(mapData));
    if (database->mapList.list == NULL)
    {
        perror("Memory allocation failed");
        return 1;
    }

    database->mapList.capacity = INITIAL_CAPACITY;
    database->mapList.size = 0;
    const char *action = "maps/list";
    const char *statut = "200";
    const char *message = "ok";
    int nbMapsList = 1;

    mapData map = {0, 24, 8, strdup("************************"
                                    "=----------------------="
                                    "=----==============----="
                                    "=----------****--------="
                                    "=------****------------="
                                    "=----==============----="
                                    "=----------------------="
                                    "************************")};

    if (!addMap(database, map))
    {
        perror("Failed to add map");
        freeMapsData(&(database->mapList));
        return 1;
    }

    database->action = strdup(action);
    database->statut = strdup(statut);
    database->message = strdup(message);
    database->nbMapsList = nbMapsList;

    return 0;
}

int addMap(queryGetMapsList *database, mapData map)
{
    if (database->mapList.size >= database->mapList.capacity)
    {
        database->mapList.capacity += INITIAL_CAPACITY;
        mapData *newList = realloc(database->mapList.list, database->mapList.capacity * sizeof(mapData));
        if (newList == NULL)
        {
            perror("Memory reallocation failed");
            return 0;
        }
        database->mapList.list = newList;
    }

    database->mapList.list[database->mapList.size] = map;
    database->mapList.size++;

    return 1;
}

void freeMapsData(mapsData *mapList)
{
    if (mapList->list)
    {
        for (size_t i = 0; i < mapList->size; i++)
        {
            free(mapList->list[i].content);
        }
        free(mapList->list);
    }
}

char *parseInJSON(queryGetMapsList *database)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        printf("Error creating JSON object.\n");
        return NULL;
    }

    cJSON_AddStringToObject(root, "action", database->action);
    cJSON_AddStringToObject(root, "statut", database->statut);
    cJSON_AddStringToObject(root, "message", database->message);
    cJSON_AddNumberToObject(root, "nbMapsList", database->nbMapsList);

    cJSON *mapsArray = cJSON_CreateArray();
    for (size_t i = 0; i < database->mapList.size; i++)
    {
        cJSON *mapObject = cJSON_CreateObject();
        cJSON_AddNumberToObject(mapObject, "id", database->mapList.list[i].id);
        cJSON_AddNumberToObject(mapObject, "width", database->mapList.list[i].width);
        cJSON_AddNumberToObject(mapObject, "height", database->mapList.list[i].height);
        cJSON_AddStringToObject(mapObject, "content", database->mapList.list[i].content);
        cJSON_AddItemToArray(mapsArray, mapObject);
    }

    cJSON_AddItemToObject(root, "maps", mapsArray);

    char *jsonStr = cJSON_Print(root);
    cJSON_Delete(root);

    return jsonStr;
}

/**
 * @brief retourne la réponse JSON correspondante à la requête "GET maps/list" 
 * @param responseToGetMapsList la structure contenant les infos à convertir en JSON
 * @return 0 en cas de succès
*/
char *getResponseInJSON(queryGetMapsList *responseToGetMapsList) {
    char *jsonStr = parseInJSON(responseToGetMapsList);
    return jsonStr;
}

/**
 * @brief retourne le message d'erreur approprié en cas d'échec de "GET maps/list"
 * @param unknowErr booléen servant à définir le message d'erreur
 * @return le message d'erreur au format JSON
*/
char *getErrorMessage(bool unknowErr) {

    int statut = 520;
    char msg[] = "Unknown Error";

    if (!unknowErr) {
        statut = 400;
        strcpy(msg, "Bad request");
    }

    // Crée un objet cJSON pour représenter le JSON
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        printf("Erreur lors de la création du JSON.\n");
        return NULL;
    }

    cJSON_AddNumberToObject(root, "statut", statut);
    cJSON_AddStringToObject(root, "message", msg);

    // Convertit l'objet cJSON en une chaîne JSON
    char *jsonStr = cJSON_Print(root);

    // printf("errMsg =>\n%s\n", jsonStr);

    // Libère la mémoire allouée par cJSON
    cJSON_Delete(root);

    return jsonStr;
}
