//
// Created by XuZY on 2023/11/20.
//

#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include "map.h"
#include "game.h"

game * init_game(int numPlayer){
	game * g = (game*)malloc(sizeof(game));
	if (g == NULL) {
		exit(EXIT_FAILURE);
	}
	pthread_mutex_init(&g->lock,NULL);
	g->ap = (player**)malloc(sizeof(player*) * numPlayer);
	for (int i = 0; i < numPlayer; i++) {
		g->ap[i] = NULL;
	}
	g->map = init_map();
	preprocess_map(g->map);
	return g;
}