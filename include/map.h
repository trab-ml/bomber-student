//
// Created by XuZY on 2023/11/1.
//

#ifndef BOMBER_STUDENT_SRC_MAP_H_
#define BOMBER_STUDENT_SRC_MAP_H_

#include <stdio.h>
#include <stdlib.h>


typedef enum {
	Empty = 0, // 空地
	Destr = 1, // 可以破坏的
	Indes = 2,  // 不可破坏的
	Player = 3,
	Bomb = 4
}wallstate;


/**
maps: 所有地图的描述列表
		id: 地图的标识符
		width: 地图的宽度（单位：格子）
height: 地图的高度（单位：格子）
content: 地图的描述
=: 可以被炸弹爆炸破坏的墙壁
*: 不可被炸弹爆炸破坏的墙壁
-: 空闲的格子
 * */

typedef struct Wall {
	int w,h;
	int wallstate;
}wall;

typedef struct map{
	int id;
	int width,height;
	char* content;
	wall* maps;
//	wall *imaps;
}map;





void create_map1(map *m);
void show_map(map *m);
map * init_map();
void preprocess_map(map *m);
void show_by_wall(map *m);

#endif //BOMBER_STUDENT_SRC_MAP_H_
