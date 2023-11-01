#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_cJSON.h"

// gcc src/test_cJSON.c src/cJSON.c -Iinclude -o bin/test_cJSON

/**
 * @brief Initialise la base de données
 * @param database Pointeur vers la structure de données de la base
 * @return 0 en cas de succès, 1 en cas d'échec
 */
int initGetResponse(queryGetMapsList *database)
{
    // Initialise des valeurs de la base de données
    database->action = "maps/list";
    database->statut = "200";
    database->message = "ok";
    database->nbMapsList = 1;

    // Crée une structure de mapData
    mapData map = {0, 24, 8, "************************"
                             "=----------------------="
                             "=----==============----="
                             "=----------****--------="
                             "=------****------------="
                             "=----==============----="
                             "=----------------------="
                             "************************"};

    if (!addMap(database, map))
    {
        perror("Échec d'ajout de la map");
        freeMapsData(&(database->mapList));
        return 1;
    }

    return 0;
}

/**
 * @brief Ajoute une map à la liste de maps
 * @param database Pointeur vers la structure de données de la base
 * @param map Structure de mapData à ajouter
 * @return 0 en cas de succès, 1 en cas d'échec
 */
int addMap(queryGetMapsList *database, mapData map)
{
    if (database->mapList.size >= database->mapList.capacity)
    {
        database->mapList.capacity += 10;
        mapData *newList = (mapData *)realloc(database->mapList.list, database->mapList.capacity * sizeof(mapData));
        if (!newList)
        {
            perror("Erreur realloc");
            return 0;
        }
        database->mapList.list = newList;
    }

    database->mapList.list[database->mapList.size] = map;
    database->mapList.size++;
    return 1;
}

/**
 * @brief Libère les données de la liste de maps
 * @param mapList Pointeur vers les données à libérer
 */
void freeMapsData(mapsData *mapList)
{
    if (mapList->list)
    {
        for (unsigned int i = 0; i < mapList->size; i++)
        {
            free(mapList->list[i].content);
        }
        free(mapList->list);
    }
}

// Définit la fonction parseInJSON
/**
 * @brief Crée le JSON correspondant à une structure de donnée de type queryGetMapsList
 * @param database structure de type queryGetMapsList contenant les données
 * @return pointeur de chaîne de caractères contenant le JSON
*/
char *parseInJSON(queryGetMapsList *database)
{
    // Crée un objet cJSON pour représenter le JSON
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        printf("Erreur lors de la désérialisation du JSON.\n");
        return NULL; // Gestion de l'erreur
    }

    // Ajoute les éléments au JSON en utilisant les données de database
    cJSON_AddStringToObject(root, "action", database->action);
    cJSON_AddStringToObject(root, "statut", database->statut);
    cJSON_AddStringToObject(root, "message", database->message);
    cJSON_AddNumberToObject(root, "nbMapsList", database->nbMapsList);

    // Crée un tableau pour les objets "maps"
    cJSON *mapsArray = cJSON_CreateArray();

    // Ajoute chaque mapData à l'array
    for (unsigned int i = 0; i < database->mapList.size; i++)
    {
        cJSON *mapObject = cJSON_CreateObject();
        cJSON_AddNumberToObject(mapObject, "id", database->mapList.list[i].id);
        cJSON_AddNumberToObject(mapObject, "width", database->mapList.list[i].width);
        cJSON_AddNumberToObject(mapObject, "height", database->mapList.list[i].height);
        cJSON_AddStringToObject(mapObject, "content", database->mapList.list[i].content);
        cJSON_AddItemToArray(mapsArray, mapObject);
    }

    cJSON_AddItemToObject(root, "maps", mapsArray);

    // Convertit le cJSON en une chaîne JSON
    char *jsonStr = cJSON_Print(root);

    // Libère la mémoire allouée par cJSON
    cJSON_Delete(root);

    return jsonStr; // Renvoyer la chaîne JSON
}

/**
 * @brief retourne la réponse JSON correspondante à la requête "GET maps/list" 
 * @param responseToGetMapsList la structure contenant les infos à convertir en JSON
 * @return 0 en cas de succès
*/
int getResponseInJSON(queryGetMapsList *responseToGetMapsList) {
    // Crée et initialise une structure représentant une réponse de requête
    initGetResponse(responseToGetMapsList);

    // Convertit les données cJSON de la stuct responseToGetMapsList en JSON.
    char *jsonStr = parseInJSON(responseToGetMapsList);

    // Affiche la chaîne JSON résultante.
    printf("%s\n", jsonStr);

    free(jsonStr);
    return 0;
}
