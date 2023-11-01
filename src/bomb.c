//
// Created by XuZY on 2023/11/1.
//

#include "../include/bomb.h"

void show_all_bomb(fifo *f) {
	node *current = f->front;
	while (current != NULL) {
		show_bomb(&current->b);
		current = current->next;
	}
}

void show_bomb(bomb *b){
	printf("<%d,%d>:",b->b_x,b->b_y);
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
		printf("Queue is empty, cannot dequeue.\n");
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
