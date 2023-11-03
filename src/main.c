//
// Created by XuZY on 2023/10/31.
//

#include "../include/player.h"
#include "../include/map.h"
#include "../include/game.h"


int main(){
//	map *m = init_map();
//	preprocess_map(m);
//	show_by_wall(m);
// -----------------
//	player p1;
//	init_player(&p1);
//	in_action(&p1);
//	show_map();
// -----------------
	game *g = init_game();
	g->player1->show_info_player(g->player1);
//	in_action(g->player1);
	while(1){
		show_by_wall(g->map);
		in_game(g->player1);
		get_position(g);
	}


}