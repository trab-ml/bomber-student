//
// Created by XuZY on 2023/11/20.
//

#ifndef EDU_1P_C_PROJET_RESEAUX_V3_SRC_GAME_H_
#define EDU_1P_C_PROJET_RESEAUX_V3_SRC_GAME_H_

#include "map.h"

typedef struct OneGame{
	struct map * map;
}game;

game * init_game();

#endif //EDU_1P_C_PROJET_RESEAUX_V3_SRC_GAME_H_
