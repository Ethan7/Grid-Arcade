/* By Ethan Hughes */
/* Written 7/19/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define FROGGER 7

#define EMPTY -1
#define LOG 0
#define CAR 1
#define FROG 2
#define WATER 3
*/
int frog_x, frog_y, placeholder;
int *row_speed, *car_log_length, *spacing, **grid_copy;

//Memory cleanup for end of frame
void cleanup(int width){
	free(row_speed);
	free(car_log_length);
	free(spacing);
	for(int i = 0; i < width; i++){
		free(grid_copy[i]);
	}
	free(grid_copy);
}

int frogger(int **grid, SDL_Event event, int t, int width, int height){

	if(t == 1){
		grid_copy = (int **) calloc(width, sizeof(int *));
		for(int i = 0; i < width; i++){
			grid_copy[i] = (int *) calloc(height, sizeof(int));
			for(int j = 1; j < height/2; j++){
				grid[i][j] = WATER;
			}
		}
		placeholder = EMPTY;
		frog_x = width/2;
		frog_y = height-1;
		row_speed = (int *) calloc(height-1, sizeof(int));
		car_log_length = (int *) calloc(height-1, sizeof(int));
		spacing = (int *) calloc(height-1, sizeof(int));
		for(int j = 0; j < height-1; j++){
			spacing[j] = rand() % 2;
			car_log_length[j] = (rand() % 4)+1;
			row_speed[j] = (rand() % 3) + 1;
			if(j % 2 == 0){
				row_speed[j] *= -1;
			}
		}
		row_speed[height/2] = 0;
	}

	grid[frog_x][frog_y] = placeholder;

	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(frog_y == 1){
				printf("YOU WIN!\n");
				cleanup(width);
				return ARCADE;
			} else if(grid[frog_x][frog_y-1] == CAR || grid[frog_x][frog_y-1] == WATER){
				printf("YOU LOSE!\n");
				cleanup(width);
				return ARCADE;
			} else {
				frog_y--;
			}
			break;
		case SDLK_DOWN:
			if(frog_y == height-1){
			} else if(grid[frog_x][frog_y+1] == CAR || grid[frog_x][frog_y+1] == WATER){
				printf("YOU LOSE!\n");
				cleanup(width);
				return ARCADE;
			} else {
				frog_y++;
			}
			break;
		case SDLK_LEFT:
			if(frog_x == 0){
			} else if(grid[frog_x-1][frog_y] == CAR || grid[frog_x-1][frog_y] == WATER){
				printf("YOU LOSE!\n");
				cleanup(width);
				return ARCADE;
			} else {
				frog_x--;
			}
			break;
		case SDLK_RIGHT:
			if(frog_x == width-1){
			} else if(grid[frog_x+1][frog_y] == CAR || grid[frog_x+1][frog_y] == WATER){
				printf("YOU LOSE!\n");
				cleanup(width);
				return ARCADE;
			} else {
				frog_x++;
			}
			break;
		}
	}

	//Store grid
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			grid_copy[i][j] = grid[i][j];
		}
	}

	//Move platforms and cars along
	for(int i = 0; i < width; i++){
		for(int j = 1; j < height-1; j++){
			if(grid[i][j] == LOG && t % row_speed[j] == 0){
				grid_copy[i][j] = WATER;
				if(row_speed[j] > 0 && i < width-1){
					grid_copy[i+1][j] = LOG;
					if(frog_x == i && frog_y == j){
						frog_x = i+1;
					}
				} else if(row_speed[j] < 0 && i > 0){
					grid_copy[i-1][j] = LOG;
					if(frog_x == i && frog_y == j){
						frog_x = i-1;
					}
				} else if(frog_x == i && frog_y == j){
					printf("YOU LOSE!\n");
					cleanup(width);
					return ARCADE;
				}
			} else if(grid[i][j] == CAR && t % row_speed[j] == 0){
				grid_copy[i][j] = EMPTY;
				if(row_speed[j] > 0 && i < width-1){
					grid_copy[i+1][j] = CAR;
					if(frog_x == i+1 && frog_y == j){
						printf("YOU LOSE!\n");
						cleanup(width);
						return ARCADE;
					}
				} else if(row_speed[j] < 0 && i > 0){
					grid_copy[i-1][j] = CAR;
					if(frog_x == i-1 && frog_y == j){
						printf("YOU LOSE!\n");
						cleanup(width);
						return ARCADE;
					}
				}
			}
		}
	}

	//Update grid
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height-1; j++){
			grid[i][j] = grid_copy[i][j];
		}
	}

	//Place new platforms and cars
	for(int j = 1; j < height-1; j++){
		if(j != height/2 && t % row_speed[j] == 0){
			if(car_log_length[j] > 0){
				if(spacing[j] == 1){
					car_log_length[j]--;
				} else if(row_speed[j] > 0){
					car_log_length[j]--;
					if(j < height/2){
						grid[0][j] = LOG;
					} else if(j > height/2){
						grid[0][j] = CAR;
					}
				} else if(row_speed[j] < 0) {
					car_log_length[j]--;
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
				car_log_length[j] = (rand() % 4)+1;
			}
		}
	}
	placeholder = grid[frog_x][frog_y];
	grid[frog_x][frog_y] = FROG;

	return FROGGER;
}