//
// Created by XuZY on 2023/11/20.
//

#include <stdlib.h>
#include <stdio.h>

#include "../include/player.h"
#include "../include/map.h"
#include "../include/game.h"

game * init_game(){
	game * g = (game*)malloc(sizeof(game));
	if (g == NULL) {
		exit(EXIT_FAILURE);
	}
	g->map = init_map();
	preprocess_map(g->map);
//	player * p1 = (player*)malloc(sizeof(player));
//	if (p1 == NULL) {
//		free(g);
//		exit(EXIT_FAILURE);
//	}
//	init_player(p1, g);
//	g->player1 = p1; // Assign the initialized player to the game's player1
//	get_position(p);
//	show_by_wall(g->map);
	return g;
}

