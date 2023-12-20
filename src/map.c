//
// Created by XuZY on 2023/11/20.
//
#include <stdio.h>
#include <stdlib.h>

#include "../include/map.h"

void create_map1(map *m){
	m->height = 8;
	m->width = 24;
	m->id = 1;
	m->content = 	"************************"
								"=----------------------="
								"=----==============----="
								"=----------****--------="
								"=------****------------="
								"=----==============----="
								"=----------------------="
								"************************";
	m->maps = (wall *)malloc(sizeof(wall)*m->width * m->height);
}


map * init_map(){
	map *m = (map*)malloc(sizeof(map));
	create_map1(m);
	return m;
}

void show_map(map *m){
	printf("%d [%d,%d]\n",m->id,m->width,m->height);
	for (int i = 0; i <m->height ; ++i) {
		for (int j = 0; j < m->width; ++j) {
			printf("%c",m->content[i*m->width + j]);
		}
		printf("\n");
	}
}

void preprocess_map(map *m){
	int num;
	for (int i = 0; i <m->height ; ++i) {
		for (int j = 0; j < m->width; ++j) {
			num = i*m->width + j;
			m->maps[num].w = j;
			m->maps[num].h = i;
			switch (m->content[i*m->width + j]) {
				case '*':{
					m->maps[num].wallstate = Indes;
					break;
				}
				case '=':{
					m->maps[num].wallstate = Destr;
					break;
				}
				default:{
					m->maps[num].wallstate = Empty;
					break;
				}
			}
		}
	}
}

void show_one_wall(wall *w){
	switch (w->wallstate) {
		case Destr: printf("=");break;
		case Indes: printf("*");break;
		case Player: printf("P");break;
		case Bomb: printf("&");break;
		case Empty: printf("-");break;
		default: printf("%d",w->wallstate);break;
	}
}

void show_by_wall(map *m){
	for (int i = 0; i < m->height; ++i) {
		for (int j = 0; j < m->width; ++j) {
			show_one_wall(&m->maps[i*m->width + j]);
		}
		printf("\n");
	}
}

