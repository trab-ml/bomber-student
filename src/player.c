//
// Created by XuZY on 2023/11/19.
//

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <time.h>

#include "../include/player.h"
#include "../include/bomb.h"
#include "../include/game.h"

void init_player(player *p, game *g) {
	// 初始化玩家位置
	int pos;
	pos = get_birth_pos(g->map);
	p->x = pos % g->map->width;
	p->y = pos / g->map->width;
	// 玩家的信息
	p->gameRef = g; // 玩家所在的游戏
	p->show_info_player = show_info_player; // 显示玩家的信息
	p->speed = 1; // 玩家的速度
	p->impact_dist = 2; // 炸弹的范围
	p->nb_classic_b = 2; // 普通炸弹的数量
	p->nbcb = 0;
	// 玩家的移动
	p->move_up = move_up;
	p->move_down = move_down;
	p->move_left = move_left;
	p->move_right = move_right;
	// 放置炸弹
	p->pose_bomb = pose_bomb;
	p->mybomb = create_fifo();
}

int get_birth_pos(map *m) {
	int pos, x, y;
	srand(time(NULL));
	do {
		x = rand() % m->width;
		y = rand() % m->height;
		pos = y * m->width + x;
	} while (m->maps[pos].wallstate != Empty);
	return pos;
}

void show_info_player(player *p) {
	printf("-[%d,%d]", p->x, p->y);
	show_all_bomb(p->mybomb);
}

void move_up(player *p, map *m) {
	int newX, newY, newP, oldP;
	oldP = cal_position(p->x, p->y, m);
	newX = p->x;
	newY = p->y - p->speed;
	newP = cal_position(newX, newY, m);
	if (newY >= 0 && m->maps[newP].wallstate == Empty) {
		m->maps[oldP].wallstate = Empty;
		p->y = newY;
		m->maps[newP].wallstate = Player;
	}
}

void move_down(player *p, map *m) {
	int newX, newY, newP, oldP;
	oldP = cal_position(p->x, p->y, m);
	newX = p->x;
	newY = p->y + p->speed;
	newP = cal_position(newX, newY, m);
	if (m->maps[newP].wallstate == Empty) {
		m->maps[oldP].wallstate = Empty;
		p->y = newY;
		m->maps[newP].wallstate = Player;
	}
}

void move_right(player *p, map *m) {
	int newX, newY, newP, oldP;
	oldP = cal_position(p->x, p->y, m);
	newX = p->x + p->speed;
	newY = p->y;
	newP = cal_position(newX, newY, m);
	if (m->maps[newP].wallstate == Empty) {
		m->maps[oldP].wallstate = Empty;
		p->x = newX;
		m->maps[newP].wallstate = Player;
	}
}

void move_left(player *p, map *m) {
	int newX, newY, newP, oldP;
	oldP = cal_position(p->x, p->y, m);
	newX = p->x - p->speed;
	newY = p->y;
	newP = cal_position(newX, newY, m);
	if (m->maps[newP].wallstate == Empty) {
		m->maps[oldP].wallstate = Empty;
		p->x = newX;
		m->maps[newP].wallstate = Player;
	}
}

int cal_position(int x, int y, map *m) {
	return y * m->width + x;
}

void pose_bomb(player *p) {
	if (p->nbcb < p->nb_classic_b) {
		bomb newb;
		newb.b_x = p->x;
		newb.b_y = p->y;
		p->gameRef->map->maps[cal_position(p->x,p->y,p->gameRef->map)].wallstate = Bomb;
		newb.impact_dist = p->impact_dist;
		newb.Bb = false;
		newb.time_exp = clock();
		enqueue(p->mybomb, newb);
		p->nbcb++;
		printf("<pose bomb>");
		show_bomb(&newb);
	} else {
		printf("<nomore avaliable>");
	}
}

void get_position(player *p){
	int x = p->x;
	int y = p->y;
	int position = y * p->gameRef->map->width + x;
	p->gameRef->map->maps[position].wallstate = Player;
}

void in_game(player *p) {

	if (p == NULL) {
		perror("player is null");
	}
	printf("playing");
	char input = getchar();
	printf("\033[H\033[J");
	p->show_info_player(p);
	printf("Player position: (%d,%d) - ", p->x, p->y);
	printf("Enter:\n");
	switch (input) {
		case 'w': { // move
			p->move_up(p, p->gameRef->map);
			break;
		}
		case 's': {// move
			p->move_down(p, p->gameRef->map);
			break;
		}
		case 'a': {// move
			p->move_left(p, p->gameRef->map);
			break;
		}
		case 'd': {// move
			p->move_right(p, p->gameRef->map);
			break;
		}
		case '1': { // poser classic bomb
			p->pose_bomb(p);
			break;
		}
//		case '2': {// poser mine
//			p->pose_bomb()
//		}
//		case 'e': { // exploser 1er bomb
//			dequeue(p->mybomb);
//			p->nbcb--;
//			break;
//		}
		default: {
			printf("INVALIDE INPUT");
			break;
		}
	}
	if (get_first_bomb(p->mybomb) != NULL){
		int this_x,this_y;
		if ((clock() - get_first_bomb(p->mybomb)->time_exp) >= TIMEEXP){
			printf("EXPLOSE");
			this_x = get_first_bomb(p->mybomb)->b_x;
			this_y = get_first_bomb(p->mybomb)->b_y;
			if(in_filed(p,this_x,this_y)){
				printf("You are dead!");
				exit(EXIT_SUCCESS);
			}
			p->gameRef->map->maps[cal_position(this_x,this_y,p->gameRef->map)].wallstate = Empty;
			dequeue(p->mybomb);
			p->nbcb--;
		}else{
			this_x = get_first_bomb(p->mybomb)->b_x;
			this_y = get_first_bomb(p->mybomb)->b_y;
			p->gameRef->map->maps[cal_position(this_x,this_y,p->gameRef->map)].wallstate = Bomb;
		}
		if (get_sec_bomb(p->mybomb) != NULL){
			this_x = get_sec_bomb(p->mybomb)->b_x;
			this_y = get_sec_bomb(p->mybomb)->b_y;
			p->gameRef->map->maps[cal_position(this_x,this_y,p->gameRef->map)].wallstate = Bomb;
		}
	}
	while ((input = getchar()) != '\n' && input != EOF) {}
}

bool in_filed(players p,int x,int y){
	int this_x = p->x;
	int this_y = p->y;
	if ((this_x > x-2 && this_x < x+2) &&(this_y > y-2 && this_y < y+2)){
		return true;
	} else{
		return false;
	}
}