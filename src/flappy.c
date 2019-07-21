/* By Ethan Hughes */
/* Written 7/21/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define ARCADE 0
#define FLAPPY 10

#define EMPTY -1
#define OBSTACLE 0
#define BIRD 1

#define HOLESIZE 5

int flappyy, spacing;
int flappy(int **grid, SDL_Event event, int game, int t, int width, int height){

	if(t == 1){
		spacing = height/2;
		flappyy = height/2;
	}

	//Apply gravity to bird
	grid[1][flappyy] = EMPTY;
	flappyy++;
	if(flappyy > height-1 || grid[1][flappyy] == OBSTACLE){
		printf("YOUR SCORE: %d\n", t);
		return ARCADE;
	} else {
		grid[1][flappyy] = BIRD;
	}

	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			grid[1][flappyy] = EMPTY;
			flappyy -= 2;
			if(grid[1][flappyy] == OBSTACLE){
				printf("YOUR SCORE: %d\n", t);
				return ARCADE;
			} else {
				grid[1][flappyy] = BIRD;
			}
			break;
		}
	}

	//Place new columns
	if(t % spacing == 0){
		int holeposition = rand() % (height-HOLESIZE);
		for(int j = 0; j < height; j++){
			if(j == holeposition){
				j += HOLESIZE;
			}
			grid[width-1][j] = OBSTACLE;
		}
	}

	//Move columns across the screen
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(grid[i][j] == OBSTACLE){
				grid[i][j] = EMPTY;
				if(i > 0){
					grid[i-1][j] = OBSTACLE;
					if(i-1 == 1 && j == flappyy){
						printf("YOUR SCORE: %d\n", t);
						return ARCADE;
					}
				}
			}
		}
	}

	return FLAPPY;
}
