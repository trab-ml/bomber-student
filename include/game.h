//
// Created by XuZY on 2023/11/1.
//

#ifndef BOMBER_STUDENT_SRC_GAME_H_
#define BOMBER_STUDENT_SRC_GAME_H_

#include "player.h"
#include "bomb.h"
#include "map.h"

typedef struct OneGame{
	map * map;
	player * player1;
//	player * player2;
}game;


game * init_game();
void get_position(game *g);



#endif //BOMBER_STUDENT_SRC_GAME_H_
