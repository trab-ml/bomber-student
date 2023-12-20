//
// Created by XuZY on 2023/11/20.
//

#ifndef EDU_1P_C_PROJET_RESEAUX_V3_SRC_BOMB_H_
#define EDU_1P_C_PROJET_RESEAUX_V3_SRC_BOMB_H_

#include <stdbool.h>

typedef struct bomb {
	int b_x, b_y;
	int impact_dist;
	bool Bb;
	int time_exp;
} bomb;

typedef struct NodeB {
	bomb b;
	struct NodeB *next;
} node;

typedef struct Fifo {
	node *front;
	node *rear;
} fifo;


void show_all_bomb(fifo *f);
void show_bomb(bomb *b);
fifo * create_fifo();
void enqueue(fifo *f,bomb b);
bomb dequeue(fifo *f);
void show_bomb(bomb *b);
bomb * get_first_bomb(fifo *f);
bomb * get_sec_bomb(fifo *f);

#endif //EDU_1P_C_PROJET_RESEAUX_V3_SRC_BOMB_H_
