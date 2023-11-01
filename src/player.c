//
// Created by XuZY on 2023/10/31.
//

#include "../include/player.h"

void init_player(player *p) {
	// position
	srand(time(NULL));
	p->x = rand() % LENGTH;
	p->y = rand() % WIDTH;
	// status
	p->show_info_player = show_info_player;
	p->speed = 1;
	p->impact_dist = 2;
	p->nb_classic_b = 2;
	//
	p->nbcb = 0; //
	// movement
	p->move_up = move_up;
	p->move_down = move_down;
	p->move_left = move_left;
	p->move_right = move_right;
	//
	p->pose_bomb = pose_bomb;
	//
	p->mybomb = (bomb *)malloc(sizeof(bomb) * p->nb_classic_b);
}


/**|***************** 左上角第一个点的坐标为(0,0)
 * |*								*
 * |*								*
 * |*								*
 * |*								*
 * |*								*
 * |*								*
 * |*								*
 * |******************
 */

void move_up(player *p) {
	p->y += p->speed;
}

void move_down(player *p) {
	p->y -= p->speed;
}

void move_right(player *p) {
	p->x += p->speed;
}

void move_left(player *p) {
	p->x -= p->speed;
}

void pose_bomb(player *p) {
	if (p->nbcb < p->nb_classic_b){
		bomb *b = (bomb *)malloc(sizeof(bomb));
		b->b_x = p->x;
		b->b_y = p->y;
		b->impact_dist = p->impact_dist;
		b->Bb = false;
		p->nbcb ++;
		printf("<pose pomb>");
		show_bomb(b);
	}else{
		printf("no more avaliable");
	}
}

void show_bomb(bomb *b){
	printf("<%d,%d>:",b->b_x,b->b_y);
	printf("{%d}\n",b->Bb);
}


void show_info_player(player *p){
	printf("[%d,%d]\n",p->x,p->y);
	for (int i = 0; i < p->nbcb+1; ++i) {
		show_bomb(&p->mybomb[i]);
	}
}

void in_action(player *p) {
	printf("player is playing");
	char input;
	printf("Player position: (%d, %d)\n", p->x, p->y);
	printf("Enter move (w/a/s/d): ");
	while ((input = getchar()) != 'q') { // 按 'q' 退出
		printf("Player position: (%d, %d)\n", p->x, p->y);
		printf("Enter:");
		switch (input) {
			case 'w': {
				p->move_up(p);
//				sprintf(msg,"move up to [%d,%d]",p->x,p->y);
//				fprintf(log,"%s\n",msg);
				break;
			}
			case 's': {
				p->move_down(p);
				break;
			}
			case 'a': {
				p->move_left(p);
				break;
			}
			case 'd': {
				p->move_right(p);
				break;
			}
			case 'p': {
				p->pose_bomb(p);
			}
			case 'z':{
				p->show_info_player(p);
			}
			default:{
				continue;
			}
		}
	}
}