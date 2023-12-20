//
// Created by XuZY on 2023/11/19.
//

#ifndef EDU_1P_C_PROJET_RESEAUX_V3_SRC_PLAYER_H_
#define EDU_1P_C_PROJET_RESEAUX_V3_SRC_PLAYER_H_

#include "bomb.h"
#include "game.h"
#define TIMEEXP 300 // ms


// --------

// --------

// --------
typedef struct Player {
	int x, y; // player's position
	bool isAlive;
	int id;
	// status
	void (*show_info_player)(struct Player *);
	struct OneGame *gameRef;
	int speed;
	int impact_dist;
	int nb_classic_b; // 普通炸弹
	int nb_mine; // 地雷数量
	int nb_remote_n; // 遥控炸弹

	//
	int nbcb; // 摆放的炸弹的数量
	// movement of player
	void (*move_up)(struct Player *, struct map *);
	void (*move_down)(struct Player *, struct map *);
	void (*move_left)(struct Player *, struct map *);
	void (*move_right)(struct Player *, struct map *);
	// action of player
	void (*pose_bomb)(struct Player *);

	// things;
	struct Fifo *mybomb;
} player, *players;

void init_player(player *p, game *g,int i);

int get_birth_pos(map *m);

void show_info_player(player *p);

void move_up(player *p, map *m);
void move_down(player *p, map *m);
void move_right(player *p, map *m);
void move_left(player *p, map *m);

int cal_position(int x, int y, map *m);
void get_position(player *p);
//int  get_position(player *p);
void pose_bomb(player *p);

void in_game(player *p);
bool in_filed(players p,int x,int y);

// ----
void explose_bomb(bomb *b,game *g);
void affect(game *g,int x,int y);
#endif //EDU_1P_C_PROJET_RESEAUX_V3_SRC_PLAYER_H_
