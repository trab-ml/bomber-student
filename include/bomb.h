//
// Created by XuZY on 2023/11/1.
//

#ifndef BOMBER_STUDENT_SRC_BOMB_H_
#define BOMBER_STUDENT_SRC_BOMB_H_

#include <stdlib.h>
#include <stdio.h>
#include "../include/player.h"

typedef struct bomb {
	int b_x, b_y;
	int impact_dist;
	bool Bb;
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

#endif //BOMBER_STUDENT_SRC_BOMB_H_
