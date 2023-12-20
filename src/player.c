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

void init_player(player *p, game *g,int i) {
	// 初始化玩家位置
	int pos;
	p->id = i;
	pos = get_birth_pos(g->map);
	p->x = pos % g->map->width;
	p->y = pos / g->map->width;
	p->isAlive = true;
	// 玩家的信息
	p->gameRef = g; // 玩家所在的游戏
	p->gameRef->ap[i] = p;
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
//	pthread_mutex_lock(&p->gameRef->lock);
	int newX, newY, newP, oldP;
	oldP = cal_position(p->x, p->y, m);
	newX = p->x;
	newY = p->y - p->speed;
	newP = cal_position(newX, newY, m);
	if (newY >= 0 && m->maps[newP].wallstate == Empty) {
		m->maps[oldP].wallstate = Empty;
		p->y = newY;
		m->maps[newP].wallstate = p->id;
	}
//	pthread_mutex_unlock(&p->gameRef->lock);
}

void move_down(player *p, map *m) {
//	pthread_mutex_lock(&p->gameRef->lock);
	int newX, newY, newP, oldP;
	oldP = cal_position(p->x, p->y, m);
	newX = p->x;
	newY = p->y + p->speed;
	newP = cal_position(newX, newY, m);
	if (m->maps[newP].wallstate == Empty) {
		m->maps[oldP].wallstate = Empty;
		p->y = newY;
		m->maps[newP].wallstate = p->id;
	}
//	pthread_mutex_unlock(&p->gameRef->lock);
}

void move_right(player *p, map *m) {
//	pthread_mutex_lock(&p->gameRef->lock);
	int newX, newY, newP, oldP;
	oldP = cal_position(p->x, p->y, m);
	newX = p->x + p->speed;
	newY = p->y;
	newP = cal_position(newX, newY, m);
	if (m->maps[newP].wallstate == Empty) {
		m->maps[oldP].wallstate = Empty;
		p->x = newX;
		m->maps[newP].wallstate = p->id;
	}
//	pthread_mutex_unlock(&p->gameRef->lock);
}

void move_left(player *p, map *m) {
//	pthread_mutex_lock(&p->gameRef->lock);
	int newX, newY, newP, oldP;
	oldP = cal_position(p->x, p->y, m);
	newX = p->x - p->speed;
	newY = p->y;
	newP = cal_position(newX, newY, m);
	if (m->maps[newP].wallstate == Empty) {
		m->maps[oldP].wallstate = Empty;
		p->x = newX;
		m->maps[newP].wallstate = p->id;
	}
//	pthread_mutex_unlock(&p->gameRef->lock);
}

int cal_position(int x, int y, map *m) {
	return y * m->width + x;
}

void pose_bomb(player *p) {
//	pthread_mutex_lock(&p->gameRef->lock);
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
//	pthread_mutex_unlock(&p->gameRef->lock);
}

void get_position(player *p){
	int x = p->x;
	int y = p->y;
	int position = y * p->gameRef->map->width + x;
	p->gameRef->map->maps[position].wallstate = p->id;
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
	p->gameRef->map->maps[cal_position(p->x,p->y,p->gameRef->map)].wallstate = p->id;
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
		case 'p': { // poser classic bomb
			p->pose_bomb(p);
			break;
		}
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
			p->gameRef->map->maps[cal_position(this_x,this_y,p->gameRef->map)].wallstate = Empty;
			bomb b = dequeue(p->mybomb);
			explose_bomb(&b,p->gameRef);
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


void refresh(player *p){
	if (p == NULL){
		perror("player is null");
		return;
	}
	printf("playing\n");
	p->show_info_player(p);
	printf("Player position: (%d,%d)\n", p->x, p->y);
	if (get_first_bomb(p->mybomb) != NULL){
		int this_x,this_y;
		if ((clock() - get_first_bomb(p->mybomb)->time_exp) >= TIMEEXP){
			printf("EXPLOSE");
			this_x = get_first_bomb(p->mybomb)->b_x;
			this_y = get_first_bomb(p->mybomb)->b_y;
			p->gameRef->map->maps[cal_position(this_x,this_y,p->gameRef->map)].wallstate = Empty;
			bomb b = dequeue(p->mybomb);
			explose_bomb(&b,p->gameRef);
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
}

void explose_bomb(bomb *b, game *g) {
	int dx, dy;
	int range;
	int bomb_x, bomb_y;
	range = b->impact_dist;
	bomb_x = b->b_x;
	bomb_y = b->b_y;

	for (int r = 0; r <= range; r++) {
		for (dy = -r; dy <= r; dy++) {
			for (dx = -r; dx <= r; dx++) {
				int pos = cal_position(bomb_x + dx, bomb_y + dy, g->map);
				if (pos >= 0 && pos < g->map->width * g->map->height) {
					affect(g, bomb_x + dx, bomb_y + dy);
				}
			}
		}
	}
}

void affect(game *g,int x,int y){
	int i;
	if (x < 0 || x >= g->map->width || y < 0 || y >= g->map->height){
		return;
	}
	int pos = y * g->map->width + x ;
	printf("%d|",pos);
	if (g->map->maps[pos].wallstate == Destr){
		g->map->maps[pos].wallstate = Empty;
	}
	for (i = 0;i<g->numPlayers;i++){
		if (x == g->ap[i]->x && y == g->ap[i]->y){
			g->ap[i]->isAlive = false;
			g->map->maps[pos].wallstate = Empty;
		}
	}
}