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
//	init_queue((bomb_queue *)&p->mybomb);
	p->mybomb = create_fifo();
//	p->mybomb = (bomb *)malloc(sizeof(bomb) * p->nb_classic_b);
}

void move_up(player *p) {
	p->y -= p->speed;
}

void move_down(player *p) {
	p->y += p->speed;
}

void move_right(player *p) {
	p->x += p->speed;
}

void move_left(player *p) {
	p->x -= p->speed;
}

void pose_bomb(player *p){
	if (p->nbcb < p->nb_classic_b){
		bomb newb;
		newb.b_x = p->x;
		newb.b_y = p->y;
		newb.impact_dist = p->impact_dist;
		newb.Bb = false;
		enqueue(p->mybomb,newb);
		p->nbcb++;
		printf("<pose bomb>");
		show_bomb(&newb);
	}else{
		printf("<nomore avaliable>");
	}
}



//void pose_bomb(player *p) {
//	if (p->nbcb < p->nb_classic_b){
//
//		bomb *b = &p->mybomb[p->nbcb];
//		b->b_x = p->x;
//		b->b_y = p->y;
//		b->impact_dist = p->impact_dist;
//		b->Bb = false;
//		p->nbcb ++;
//		printf("<pose pomb>");
//		show_bomb(b);
//	}else{
//		printf("<no more avaliable>");
//	}
//}

//void show_bomb(bomb *b){
//	printf("<%d,%d>:",b->b_x,b->b_y);
//	printf("{%d}\n",b->Bb);
//}


void show_info_player(player *p){
	printf("[%d,%d]\n",p->x,p->y);
	show_all_bomb(p->mybomb);
}

void in_action(player *p) {
	printf("player is playing");
	char input;
	printf("Player position: (%d, %d)\n", p->x, p->y);
	printf("Enter move (w/a/s/d)\n");

	while ((input = getchar()) != 'q') { // 按 'q' 退出
//		clear_line();
		printf("Player position: (%d,%d) - ", p->x, p->y);
		printf("Enter:\n");
		switch (input) {
			case 'w': { // move
				p->move_up(p);
				break;
			}
			case 's': {// move
				p->move_down(p);
				break;
			}
			case 'a': {// move
				p->move_left(p);
				break;
			}
			case 'd': {// move
				p->move_right(p);
				break;
			}
			case 'p': { // poser bomb
				p->pose_bomb(p);
				break;
			}
			case 'z':{ // show player's info
				p->show_info_player(p);
				break;
			}
			case 'e':{ // exploser 1er bomb


			}
			default:{
				printf("INVALIDE INPUT");
				break;
			}
		}
		while((input = getchar()) != '\n' && input != EOF){

		}
	}
}