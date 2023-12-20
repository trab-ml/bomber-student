//
// Created by XuZY on 2023/11/20.
//

#ifndef EDU_1P_C_PROJET_RESEAUX_V3_SRC_GAME_H_
#define EDU_1P_C_PROJET_RESEAUX_V3_SRC_GAME_H_

#include "map.h"
#include "player.h"
#include <pthread.h>
struct Player ;

typedef struct OneGame{
	struct map * map;
	int numPlayers;
	struct Player **ap;
	pthread_mutex_t lock;
}game;

game * init_game(int numPlayer);

#endif //EDU_1P_C_PROJET_RESEAUX_V3_SRC_GAME_H_
