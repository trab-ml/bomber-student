//
// Created by XuZY on 2023/10/31.
//

#ifndef BOMBER_STUDENT_SRC_PLAYER_H_
#define BOMBER_STUDENT_SRC_PLAYER_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "../include/bomb.h"

#define LENGTH 100
#define WIDTH 80

typedef struct Fifo Fifo;

//typedef struct bomb{
//	int b_x,b_y;
//	int impact_dist;
//	bool Bb;
//}bomb;

typedef struct Player{
	int x,y; // player's position
	// status
	void (*show_info_player)(struct Player *);
	int speed;
	int impact_dist;
	int nb_classic_b; // 普通炸弹
	int nb_mine; // 地雷数量
	int nb_remote_n; // 遥控炸弹
	//
	int nbcb; // 摆放的炸弹的数量
	// movement of player
	void (*move_up)(struct Player *);
	void (*move_down)(struct Player *);
	void (*move_left)(struct Player *);
	void (*move_right)(struct Player *);
	// action of player
	void (*pose_bomb)(struct Player *);

	// things;
	Fifo *mybomb;
}player,*players;

void init_player(player *p,int w,int h);

void move_up(player *p);
void move_down(player *p);
void move_right(player *p);
void move_left(player *p);

void pose_bomb(player *p);

//void show_bomb(bomb *b);

void show_info_player(player *p);

void in_action(player *p);
void in_game(player *p);

#endif //BOMBER_STUDENT_SRC_PLAYER_H_
