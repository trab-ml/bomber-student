#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_cJSON.h"
#include "error_handler.h"

/**
 * @brief initialise JSON response for "GET maps/list" request
 * @param database the structure containing the infos to convert to JSON
 * @return void
 */
void initGetResponse(queryGetMapsList *database)
{
    database->mapList.list = malloc(INITIAL_CAPACITY * sizeof(mapData));
    if (database->mapList.list == NULL)
    {
        handleError(MALLOC_ERROR);
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

    addMap(database, map);
    database->action = strdup(action);
    database->statut = strdup(statut);
    database->message = strdup(message);
    database->nbMapsList = nbMapsList;
}

/**
 * @brief add a map to the list of maps
 * @param database the structure containing the infos to convert to JSON
 * @param map the map to add
 * @return 0 in case of success
 */
void addMap(queryGetMapsList *database, mapData map)
{
    if (database->mapList.size >= database->mapList.capacity)
    {
        database->mapList.capacity += INITIAL_CAPACITY;
        mapData *newList = realloc(database->mapList.list, database->mapList.capacity * sizeof(mapData));
        if (newList == NULL)
        {
            freeMapsData(&(database->mapList));
            handleError(ADD_MAP_ERROR);
        }
        database->mapList.list = newList;
    }

    database->mapList.list[database->mapList.size] = map;
    database->mapList.size++;
}

/**
 * @brief free maps data
 * @param mapList the list of maps to free
 * @return void
 */
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

/**
 * @brief convert the structure to JSON
 * @param database the structure containing the infos to convert to JSON
 * @return the JSON string
 */
char *parseInJSON(queryGetMapsList *database)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        handleError(CJSON_CREATE_OBJECT_ERROR);
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
 * @brief return the JSON response for "GET maps/list" request
 * @param responseToGetMapsList the structure containing the infos to convert to JSON
 * @return the JSON string
 */
char *getResponseInJSON(queryGetMapsList *responseToGetMapsList)
{
    char *jsonStr = parseInJSON(responseToGetMapsList);
    return jsonStr;
}

/**
 * @brief return the JSON error message
 * @param unknowErr true if the error is unknown
 * @return the JSON string
 */
char *getQueryErrorMessage(bool unknowErr)
{

    int statut = 520;
    char msg[] = "Unknown Error";

    if (!unknowErr)
    {
        statut = 400;
        strcpy(msg, "Bad request");
    }

    // create a JSON object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        handleError(CJSON_CREATE_OBJECT_ERROR);
    }

    cJSON_AddNumberToObject(root, "statut", statut);
    cJSON_AddStringToObject(root, "message", msg);

    // Convert JSON object to string
    char *jsonStr = cJSON_Print(root);

    // printf("errMsg =>\n%s\n", jsonStr);

    // Delete JSON object
    cJSON_Delete(root);

    return jsonStr;
}
