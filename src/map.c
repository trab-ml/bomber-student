//
// Created by XuZY on 2023/11/1.
//

#include "../include/map.h"

/**
	map1 =
			"************************\n"
			"=----------------------=\n"
		 	"=----==============----=\n"
			"=----------****--------=\n"
			"=------****------------=\n"
 			"=----==============----=\n"
 			"=----------------------=\n"
 			"************************";
	map2 =
			"************************\n"
			"=----------------------=\n"
		 	"=----======**======----=\n"
			"=------*--------*------=\n"
			"=------*--------*------=\n"
 			"=----======**======----=\n"
 			"=----------------------=\n"
 			"************************";
**/

void create_map1(map *m){
//	map *m =(map*)malloc(sizeof(map));
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
//	m->imaps = (wall *)malloc(sizeof(wall)*m->width * m->height);
}

//void create_map2(){
//	map *m = (map*)malloc(sizeof(map));
//	m.height = 8;
//	m.width = 24;
//	m.id = 2;
//	m.content = 	"************************\n"
//								"=----------------------=\n"
//								"=----======**======----=\n"
//								"=------*--------*------=\n"
//								"=------*--------*------=\n"
//								"=----======**======----=\n"
//								"=----------------------=\n"
//								"************************";
//}

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
//					m->imaps[num].wallstate = Indes;
					break;
				}
				case '=':{
					m->maps[num].wallstate = Destr;
//					m->imaps[num].wallstate = Destr;
					break;
				}
				default:{
					m->maps[num].wallstate = Empty;
//					m->imaps[num].wallstate = Empty;
					break;
				}
			}
		}
	}
}

void show_one_wall(wall *w){
	switch (w->wallstate) {
		case 1: printf("=");break;
		case 2: printf("*");break;
		case 3: printf("P");break;
		case 4: printf("&");break;
		default : printf("-");break;
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
