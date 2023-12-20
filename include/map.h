//
// Created by XuZY on 2023/11/20.
//

#ifndef EDU_1P_C_PROJET_RESEAUX_V3_SRC_MAP_H_
#define EDU_1P_C_PROJET_RESEAUX_V3_SRC_MAP_H_


typedef enum {
	Empty = 5, // 空地
	Destr = 6, // 可以破坏的
	Indes = 7,  // 不可破坏的
	Player = 8,
	Bomb = 9
}wallstate;

typedef struct Wall {
	int w,h;
	int wallstate;
}wall;

typedef struct map{
	int id;
	int width,height;
	char* content;
	wall* maps;
}map;


void create_map1(map *m);
void show_map(map *m);
map * init_map();
void preprocess_map(map *m);
void show_by_wall(map *m);

#endif //EDU_1P_C_PROJET_RESEAUX_V3_SRC_MAP_H_
