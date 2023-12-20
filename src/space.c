/* By Ethan Hughes */
/* Written 7/19/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define SPACE 6

#define EMPTY -1
#define PLAYER 1
#define ENEMY 0
#define EBULLET 2
#define PBULLET 3
#define HOUSE 4
#define MOVED -2

#define LAG 8
*/
int invaders_x = 0, invaders_y = 0, invader_count = 0, player_x = 0, player_y = 0, enemy_lag = 0, house_health = 0;
int *invader_fire;
int space(int **grid, SDL_Event event, int t, int width, int height){
	if(t == 1){
		invader_count = 0;
		invaders_x = 0;
		invaders_y = 0;

		for(int j = 2; j < 7; j++){
			for(int i = 2; i < width-2; i++){
				if(i % 2 == 0 && j % 2 == 0){
					grid[i][j] = ENEMY;
					invader_count++;
					if(j > invaders_y){
						invaders_y = j;
					}
				}
			}
		}
		house_health = 0;
		for(int i = 2; i < width-2; i++){
			if(i % 2 == 0){
				grid[i][height-2] = HOUSE;
				house_health++;
			}
		}
		player_x = width/2;
		player_y = height-1;
		enemy_lag = 0;

		invader_fire = (int *) calloc(width, sizeof(int));
		for(int i = 0; i < width; i++){
			invader_fire[i] = -1;
		}
	}
	//Control player
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_LEFT:
			if(player_x > 0){
				grid[player_x][player_y] = EMPTY;
				player_x--;
			}
			break;
		case SDLK_RIGHT:
			if(player_x < width-1){
				grid[player_x][player_y] = EMPTY;
				player_x++;
			}
			break;
		case SDLK_UP:
			if(grid[player_x][player_y-1] == HOUSE){
				house_health--;
			}
			grid[player_x][player_y-1] = PBULLET;
			break;
		}
	}

	grid[player_x][player_y] = PLAYER;
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			//Move enemy bullets
			if(grid[i][j] == EBULLET && j == height-1){
				grid[i][j] = EMPTY;
			} else if(grid[i][j] == EBULLET && grid[i][j+1] == PLAYER){
				//Check whether you've died and lost
				printf("YOU LOSE!\n");
				free(invader_fire);
				return ARCADE;
			} else if(grid[i][j] == EBULLET && grid[i][j+1] == PBULLET){
				grid[i][j] = EMPTY;
				grid[i][j+1] = EMPTY;
			} else if(grid[i][j] == EBULLET && grid[i][j+1] == HOUSE){
				grid[i][j] = EMPTY;
				grid[i][j+1] = EMPTY;
				house_health--;
			} else if(grid[i][j] == EBULLET){
				grid[i][j] = EMPTY;
				grid[i][j+1] = MOVED;
			}
			//Move player bullets
			if(grid[i][j] == PBULLET && grid[i][j-1] == HOUSE){
				grid[i][j] = EMPTY;
			} else if(grid[i][j] == PBULLET && grid[i][j-1] == ENEMY){
				grid[i][j] = EMPTY;
				grid[i][j-1] = EMPTY;
				invader_count--;
			} else if(grid[i][j] == PBULLET && grid[i][j-1] == EBULLET){
				grid[i][j] = EMPTY;
				grid[i][j-1] = EMPTY;
			} else if(grid[i][j] == PBULLET){
				grid[i][j] = EMPTY;
				grid[i][j-1] = PBULLET;
			}
		}
	}
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(grid[i][j] == MOVED){
				grid[i][j] = EBULLET;
			}
		}
	}
	if(enemy_lag % LAG == 0){
		for(int j = invaders_y % 2; j < height; j+=2){
			for(int i = invaders_x; i < width; i+=2){
				//Move enemy
				if(grid[i][j] == ENEMY){
					invader_fire[i] = j;
					if(invaders_x == 2 && invaders_y % 2 == 0){
						grid[i][j] = EMPTY;
						grid[i][j+1] = ENEMY;
					} else if(invaders_x == 0 && invaders_y % 2 == 1){
						grid[i][j] = EMPTY;
						grid[i][j+1] = ENEMY;
					} else if(invaders_y % 2 == 0){
						grid[i][j] = EMPTY;
						grid[i+1][j] = ENEMY;
					} else if(invaders_y % 2 == 1){
						grid[i][j] = EMPTY;
						grid[i-1][j] = ENEMY;
					}
				}
			}
		}
		//Move enemy position markers
		if(invaders_y == height-1){
			printf("YOU LOSE!\n");
			free(invader_fire);
			return ARCADE;
		} else if(invaders_x == 2 && invaders_y % 2 == 0){
			invaders_y++;
		} else if(invaders_x == 0 && invaders_y % 2 == 1){
			invaders_y++;
		} else if(invaders_y % 2 == 0){
			invaders_x++;
		} else if(invaders_y % 2 == 1){
			invaders_x--;
		}
		//Launch enemy bullet
		int fire = 0;
		do{
			fire = rand() % width;
		} while(invader_fire[fire] == -1 && invader_count > 0);
		if(grid[fire][invader_fire[fire]+1] == ENEMY){
			grid[fire][invader_fire[fire]+2] = EBULLET;
		} else {
			grid[fire][invader_fire[fire]+1] = EBULLET;
		}
		//Clear firing line
		for(int i = 0; i < width; i++){
			invader_fire[i] = -1;
		}
	}
	enemy_lag++;
	//Check if all your houses have been destroyed
	if(house_health == 0){
		printf("YOU LOSE!\n");
		free(invader_fire);
		return ARCADE;
	}
	//Check whether you've killed the enemy and won
	if(invader_count == 0){
		printf("YOU WIN!\n");
		free(invader_fire);
		return ARCADE;
	}
	return SPACE;
}