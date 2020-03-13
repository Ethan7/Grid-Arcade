/* By Ethan Hughes */
/* Written 7/19/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define ARCADE 0
#define FROGGER 7

#define EMPTY -1
#define LOG 0
#define CAR 1
#define FROG 2
#define WATER 3

int frogx, frogy, placeholder;
int *rowspeed, *carloglen, *spacing, **gridcopy;

void cleanup(int width){
	free(rowspeed);
	free(carloglen);
	free(spacing);
	for(int i = 0; i < width; i++){
		free(gridcopy[i]);
	}
	free(gridcopy);
}

int frogger(int **grid, SDL_Event event, int game, int t, int width, int height){

	if(t == 1){
		gridcopy = (int **) calloc(width, sizeof(int *));
		for(int i = 0; i < width; i++){
			gridcopy[i] = (int *) calloc(height, sizeof(int));
			for(int j = 1; j < height/2; j++){
				grid[i][j] = WATER;
			}
		}
		placeholder = EMPTY;
		frogx = width/2;
		frogy = height-1;
		rowspeed = (int *) calloc(height-1, sizeof(int));
		carloglen = (int *) calloc(height-1, sizeof(int));
		spacing = (int *) calloc(height-1, sizeof(int));
		for(int j = 0; j < height-1; j++){
			spacing[j] = rand() % 2;
			carloglen[j] = (rand() % 4)+1;
			rowspeed[j] = (rand() % 3) + 1;
			if(j % 2 == 0){
				rowspeed[j] *= -1;
			}
		}
		rowspeed[height/2] = 0;
	}

	grid[frogx][frogy] = placeholder;

	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(frogy == 1){
				printf("YOU WIN!\n");
				cleanup(width);
				return ARCADE;
			} else if(grid[frogx][frogy-1] == CAR || grid[frogx][frogy-1] == WATER){
				printf("YOU LOSE!\n");
				cleanup(width);
				return ARCADE;
			} else {
				frogy--;
			}
			break;
		case SDLK_DOWN:
			if(frogy == height-1){
			} else if(grid[frogx][frogy+1] == CAR || grid[frogx][frogy+1] == WATER){
				printf("YOU LOSE!\n");
				cleanup(width);
				return ARCADE;
			} else {
				frogy++;
			}
			break;
		case SDLK_LEFT:
			if(frogx == 0){
			} else if(grid[frogx-1][frogy] == CAR || grid[frogx-1][frogy] == WATER){
				printf("YOU LOSE!\n");
				cleanup(width);
				return ARCADE;
			} else {
				frogx--;
			}
			break;
		case SDLK_RIGHT:
			if(frogx == width-1){
			} else if(grid[frogx+1][frogy] == CAR || grid[frogx+1][frogy] == WATER){
				printf("YOU LOSE!\n");
				cleanup(width);
				return ARCADE;
			} else {
				frogx++;
			}
			break;
		}
	}

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			gridcopy[i][j] = grid[i][j];
		}
	}

	//Move platforms and cars along
	for(int i = 0; i < width; i++){
		for(int j = 1; j < height-1; j++){
			if(grid[i][j] == LOG && t % rowspeed[j] == 0){
				gridcopy[i][j] = WATER;
				if(rowspeed[j] > 0 && i < width-1){
					gridcopy[i+1][j] = LOG;
					if(frogx == i && frogy == j){
						frogx = i+1;
					}
				} else if(rowspeed[j] < 0 && i > 0){
					gridcopy[i-1][j] = LOG;
					if(frogx == i && frogy == j){
						frogx = i-1;
					}
				} else if(frogx == i && frogy == j){
					printf("YOU LOSE!\n");
					cleanup(width);
					return ARCADE;
				}
			} else if(grid[i][j] == CAR && t % rowspeed[j] == 0){
				gridcopy[i][j] = EMPTY;
				if(rowspeed[j] > 0 && i < width-1){
					gridcopy[i+1][j] = CAR;
					if(frogx == i+1 && frogy == j){
						printf("YOU LOSE!\n");
						cleanup(width);
						return ARCADE;
					}
				} else if(rowspeed[j] < 0 && i > 0){
					gridcopy[i-1][j] = CAR;
					if(frogx == i-1 && frogy == j){
						printf("YOU LOSE!\n");
						cleanup(width);
						return ARCADE;
					}
				}
			}
		}
	}

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height-1; j++){
			grid[i][j] = gridcopy[i][j];
		}
	}

	//Place new platforms and cars
	for(int j = 1; j < height-1; j++){
		if(j != height/2 && t % rowspeed[j] == 0){
			if(carloglen[j] > 0){
				if(spacing[j] == 1){
					carloglen[j]--;
				} else if(rowspeed[j] > 0){
					carloglen[j]--;
					if(j < height/2){
						grid[0][j] = LOG;
					} else if(j > height/2){
						grid[0][j] = CAR;
					}
				} else if(rowspeed[j] < 0) {
					carloglen[j]--;
					if(j < height/2){
						grid[width-1][j] = LOG;
					} else if(j > height/2){
						grid[width-1][j] = CAR;
					}
				}
			} else {
				if(spacing[j] == 0){
					spacing[j] = 1;
				} else {
					spacing[j] = 0;
				}
				carloglen[j] = (rand() % 4)+1;
			}
		}
	}
	placeholder = grid[frogx][frogy];
	grid[frogx][frogy] = FROG;

	return FROGGER;
}