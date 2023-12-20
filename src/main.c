//
// Created by XuZY on 2023/11/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../include/player.h"
#include "../include/game.h"




int main(){
	int i,numPlayer = 2;
	game *g = init_game(numPlayer);
	g->numPlayers = numPlayer;
	player **p = (player**)malloc(sizeof(player*) * numPlayer);
	for(i = 0;i<numPlayer;i++){
		p[i] = (player*)malloc(sizeof(player));
		init_player(p[i],g,i);
		get_position(p[i]);
		g->ap[i] = p[i];
	}
	for (;;) {
		for (i = 0; i < numPlayer; ++i) {

			if(p[i]->isAlive){
				show_by_wall(g->map);
				in_game(p[i]);
			}
		}
	}


}