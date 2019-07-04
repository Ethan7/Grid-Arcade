/* By Ethan Hughes */
/* Written 7/3/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define ARCADE 0
#define CONWAY 8

#define EMPTY -1
#define LIFE 0

int conway(int **grid, SDL_Event event, int game, int t, int width, int height){

	int neighbor[width][height];
	if(t == 1){
		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){
				neighbor[i][j] = 0;
				//if(rand() % 8 == 0){
				//	grid[i][j] = LIFE;
				//}
			}
		}
	}
	//MEMORY ISSUES, GRID SOMETIMES DOESN't RESET
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			int neighbors = 0;
			if(i > 0){
				if(j > 0 && grid[i-1][j-1] == LIFE){
					neighbors++;
				}
				if(j < height-1 && grid[i-1][j+1] == LIFE){
					neighbors++;
				}
				if(grid[i-1][j] == LIFE){
					neighbors++;
				}
			}
			if(i < width-1){
				if(j > 0 && grid[i+1][j-1] == LIFE){
					neighbors++;
				}
				if(j < height-1 && grid[i+1][j+1] == LIFE){
					neighbors++;
				}
				if(grid[i+1][j] == LIFE){
					neighbors++;
				}
			}
			if(j > 0 && grid[i][j-1] == LIFE){
				neighbors++;
			}
			if(j < height-1 && grid[i][j+1] == LIFE){
				neighbors++;
			}
			neighbors = neighbors;
			neighbor[i][j] = neighbors;
		}
	}
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(neighbor[i][j] < 2){
				grid[i][j] = EMPTY;
			} else if(neighbor[i][j] == 2){
			} else if(neighbor[i][j] == 3){
				grid[i][j] = LIFE;
			} else {
				grid[i][j] = EMPTY;
			}
		}
	}
	return CONWAY;
}