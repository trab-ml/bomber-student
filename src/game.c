//
// Created by XuZY on 2023/11/1.
//

#include "game.h"

game * init_game(){
	game * g = (game*)malloc(sizeof(game));
	if (g == NULL) {
		exit(EXIT_FAILURE);
	}
	g->map = init_map();
	preprocess_map(g->map);
	player * p1 = (player*)malloc(sizeof(player));
	if (p1 == NULL) {
		free(g);
		exit(EXIT_FAILURE);
	}
	init_player(p1, g->map->width, g->map->height);
	g->player1 = p1; // Assign the initialized player to the game's player1
	get_position(g);
//	show_by_wall(g->map);
	return g;
}

void get_position(game *g){
	int x = g->player1->x;
	int y = g->player1->y;
	int position = y * g->map->width + x;
	g->map->maps[position].wallstate = Player;
}