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
int invadersx = 0, invadersy = 0, invcount = 0, playerx = 0, playery = 0, enemylag = 0, househealth = 0;
int *invfire;
int space(int **grid, SDL_Event event, int t, int width, int height){
	if(t == 1){
		invcount = 0;
		invadersx = 0;
		invadersy = 0;

		for(int j = 2; j < 7; j++){
			for(int i = 2; i < width-2; i++){
				if(i % 2 == 0 && j % 2 == 0){
					grid[i][j] = ENEMY;
					invcount++;
					if(j > invadersy){
						invadersy = j;
					}
				}
			}
		}
		househealth = 0;
		for(int i = 2; i < width-2; i++){
			if(i % 2 == 0){
				grid[i][height-2] = HOUSE;
				househealth++;
			}
		}
		playerx = width/2;
		playery = height-1;
		enemylag = 0;

		invfire = (int *) calloc(width, sizeof(int));
		for(int i = 0; i < width; i++){
			invfire[i] = -1;
		}
	}
	//Control player
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_LEFT:
			if(playerx > 0){
				grid[playerx][playery] = EMPTY;
				playerx--;
			}
			break;
		case SDLK_RIGHT:
			if(playerx < width-1){
				grid[playerx][playery] = EMPTY;
				playerx++;
			}
			break;
		case SDLK_UP:
			if(grid[playerx][playery-1] == HOUSE){
				househealth--;
			}
			grid[playerx][playery-1] = PBULLET;
			break;
		}
	}

	grid[playerx][playery] = PLAYER;
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			//Move enemy bullets
			if(grid[i][j] == EBULLET && j == height-1){
				grid[i][j] = EMPTY;
			} else if(grid[i][j] == EBULLET && grid[i][j+1] == PLAYER){
				//Check whether you've died and lost
				printf("YOU LOSE!\n");
				free(invfire);
				return ARCADE;
			} else if(grid[i][j] == EBULLET && grid[i][j+1] == PBULLET){
				grid[i][j] = EMPTY;
				grid[i][j+1] = EMPTY;
			} else if(grid[i][j] == EBULLET && grid[i][j+1] == HOUSE){
				grid[i][j] = EMPTY;
				grid[i][j+1] = EMPTY;
				househealth--;
			} else if(grid[i][j] == EBULLET){
				grid[i][j] = EMPTY;
				grid[i][j+1] = MOVED;
			}
			//Move player bullets
			if(grid[i][j] == PBULLET && grid[i][j-1] == 0){
				grid[i][j] = EMPTY;
			} else if(grid[i][j] == PBULLET && grid[i][j-1] == ENEMY){
				grid[i][j] = EMPTY;
				grid[i][j-1] = EMPTY;
				invcount--;
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
	if(enemylag % LAG == 0){
		for(int j = invadersy % 2; j < height; j+=2){
			for(int i = invadersx; i < width; i+=2){
				//Move enemy
				if(grid[i][j] == ENEMY){
					invfire[i] = j;
					if(invadersx == 2 && invadersy % 2 == 0){
						grid[i][j] = EMPTY;
						grid[i][j+1] = ENEMY;
					} else if(invadersx == 0 && invadersy % 2 == 1){
						grid[i][j] = EMPTY;
						grid[i][j+1] = ENEMY;
					} else if(invadersy % 2 == 0){
						grid[i][j] = EMPTY;
						grid[i+1][j] = ENEMY;
					} else if(invadersy % 2 == 1){
						grid[i][j] = EMPTY;
						grid[i-1][j] = ENEMY;
					}
				}
			}
		}
		//Move enemy position markers
		if(invadersy == height-1){
			printf("YOU LOSE!\n");
			free(invfire);
			return ARCADE;
		} else if(invadersx == 2 && invadersy % 2 == 0){
			invadersy++;
		} else if(invadersx == 0 && invadersy % 2 == 1){
			invadersy++;
		} else if(invadersy % 2 == 0){
			invadersx++;
		} else if(invadersy % 2 == 1){
			invadersx--;
		}
		//Launch enemy bullet
		int fire = 0;
		do{
			fire = rand() % width;
		} while(invfire[fire] == -1 && invcount > 0);
		if(grid[fire][invfire[fire]+1] == ENEMY){
			grid[fire][invfire[fire]+2] = EBULLET;
		} else {
			grid[fire][invfire[fire]+1] = EBULLET;
		}
		//Clear firing line
		for(int i = 0; i < width; i++){
			invfire[i] = -1;
		}
	}
	enemylag++;
	//Check if all your houses have been destroyed
	if(househealth == 0){
		printf("YOU LOSE!\n");
		free(invfire);
		return ARCADE;
	}
	//Check whether you've killed the enemy and won
	if(invcount == 0){
		printf("YOU WIN!\n");
		free(invfire);
		return ARCADE;
	}
	return SPACE;
}