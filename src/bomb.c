//
// Created by XuZY on 2023/11/20.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/bomb.h"


void show_all_bomb(fifo *f) {
	node *current = f->front;
	while (current != NULL) {
		show_bomb(&current->b);
		current = current->next;
	}
}

void show_bomb(bomb *b){
	printf("bomb: <%d,%d>:",b->b_x,b->b_y);
	printf("{%d}\n",b->Bb);
}


fifo * create_fifo(){
	fifo *q = (fifo*)malloc(sizeof(fifo));
	q->front = q->rear = NULL;
	return q;
}

void enqueue(fifo *f,bomb b){
	node *new_node = (node*)malloc(sizeof(node));
	new_node ->b = b;
	new_node->next = NULL;

	if (f->rear == NULL){
		f->front = f->rear = new_node;
		return;
	}
	f->rear->next = new_node;
	f->rear = new_node;
}

bomb dequeue(fifo *f) {
	if (f->front == NULL) {
		printf("No more bomb from you.\n");
		return (bomb){0, 0, 0, false};
	}
	node *temp = f->front;
	bomb removed_bomb = temp->b;

	f->front = f->front->next;
	if (f->front == NULL) {
		f->rear = NULL;
	}
	free(temp);
	return removed_bomb;
}

bomb * get_first_bomb(fifo *f){
	if(f->front == NULL){
		return NULL;
	}
	return &(f->front->b);
}

bomb * get_sec_bomb(fifo *f){
	if(f->front == NULL){
		return NULL;
	}
	return &(f->front->next->b);
}
