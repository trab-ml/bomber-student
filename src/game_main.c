//
// Created by XuZY on 2023/11/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "player.h"
#include "game.h"


/**
 * up --> w
 * down --> s
 * left --> a
 * rigth --> d
*/

//
// Created by XuZY on 2023/11/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "../include/player.h"
#include "../include/game.h"

void get_game_input(game *g) {
	int id;
	char action;
//	printf("Enter ID and Action (e.g., '0 w'): ");
	scanf("%d %c", &id, &action);
	if (id >= 0 && id < g->numPlayers) {
		player *p = g->ap[id];
		if (p != NULL && p->isAlive) {
			switch (action) {
				case 'z': p->move_up(p, g->map); break;
				case 's': p->move_down(p, g->map); break;
				case 'q': p->move_left(p, g->map); break;
				case 'd': p->move_right(p, g->map); break;
				case 'p': p->pose_bomb(p); break;
				default: printf("INVALIDE INPUT\n"); break;
			}
		}
	} else {
		printf("Invalid player ID\n");
	}
}
void * player_thread(void *arg){
	player *p = (player *)arg;
	while (p->isAlive){
		pthread_mutex_lock(&p->gameRef->lock);
		show_by_wall(p->gameRef->map);
		get_game_input(p->gameRef);
//		in_game(p);
		refresh(p);
		pthread_mutex_unlock(&p->gameRef->lock);
		usleep(10);
	}
	return NULL;
}

// int main(){
// 	int i,numPlayer = 2;
// 	game *g = init_game(numPlayer);
// 	pthread_t threads[numPlayer];
// 	g->numPlayers = numPlayer;
// 	player  **p = (player **)malloc(sizeof(player*) * numPlayer);
// 	for(i = 0;i<numPlayer;i++){
// 		p[i] = (player*)malloc(sizeof(player));
// 		init_player(p[i],g,i);
// 		get_position(p[i]);
// 		g->ap[i] = p[i];
// 		pthread_create(&threads[i],NULL,player_thread,(void *)g->ap[i]);
// 	}
// 	for (i = 0; i < numPlayer; ++i) {
// 		pthread_join(threads[i],NULL);
// 	}
// 	pthread_mutex_destroy(&g->lock);
// 	return 0;
// }