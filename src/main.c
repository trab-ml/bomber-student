//
// Created by XuZY on 2023/11/20.
//
#include <stdio.h>
#include <stdlib.h>

#include "../include/player.h"
#include "../include/game.h"


int main(){
	int i,numPlayer = 1;
	game *g = init_game();
	player *p = (player*)malloc(sizeof(player));
	init_player(p,g);
	get_position(p);
	while (1){
		show_by_wall(g->map);
//		for (i = 0;i<numPlayer;i++){
		in_game(p);
//		}
	}
}