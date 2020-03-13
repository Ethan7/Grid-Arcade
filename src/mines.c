/* By Ethan Hughes */
/* Written 11/13/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ARCADE 0
#define MINES 13

#define EMPTY -1
#define MINE 1
#define FLAG 4
#define CLEAR 0

//Number defines
#define ONE 8
#define TWO 9
#define THREE 10
#define FOUR 11
#define FIVE 12
#define SIX 13
#define SEVEN 14
#define EIGHT 15

//Recursive Flood-fill algorithm
void sweep(int buttonx, int buttony, int **undergrid, int **grid, int width, int height){
	int mines = 0;
	if(buttonx > 0){
		if(buttony > 0 && undergrid[buttonx-1][buttony-1] == MINE){
			mines++;
		}
		if(buttony < height-1 && undergrid[buttonx-1][buttony+1] == MINE){
			mines++;
		}
		if(undergrid[buttonx-1][buttony] == MINE){
			mines++;
		}
	}
	if(buttonx < width-1){
		if(buttony > 0 && undergrid[buttonx+1][buttony-1] == MINE){
			mines++;
		}
		if(buttony < height-1 && undergrid[buttonx+1][buttony+1] == MINE){
			mines++;
		}
		if(undergrid[buttonx+1][buttony] == MINE){
			mines++;
		}
	}
	if(buttony > 0 && undergrid[buttonx][buttony-1] == MINE){
		mines++;
	}
	if(buttony < height-1 && undergrid[buttonx][buttony+1] == MINE){
		mines++;
	}
	if(mines == 0){
		grid[buttonx][buttony] = CLEAR;
		if(buttonx > 0){
			if(buttony > 0 && grid[buttonx-1][buttony-1] == EMPTY){
				sweep(buttonx-1, buttony-1, undergrid, grid, width, height);
			}
			if(buttony < height-1 && grid[buttonx-1][buttony+1] == EMPTY){
				sweep(buttonx-1, buttony+1, undergrid, grid, width, height);
			}
			if(grid[buttonx-1][buttony] == EMPTY){
				sweep(buttonx-1, buttony, undergrid, grid, width, height);
			}
		}
		if(buttonx < width-1){
			if(buttony > 0 && grid[buttonx+1][buttony-1] == EMPTY){
				sweep(buttonx+1, buttony-1, undergrid, grid, width, height);
			}
			if(buttony < height-1 && grid[buttonx+1][buttony+1] == EMPTY){
				sweep(buttonx+1, buttony+1, undergrid, grid, width, height);
			}
			if(grid[buttonx+1][buttony] == EMPTY){
				sweep(buttonx+1, buttony, undergrid, grid, width, height);
			}
		}
		if(buttony > 0 && grid[buttonx][buttony-1] == EMPTY){
			sweep(buttonx, buttony-1, undergrid, grid, width, height);
		}
		if(buttony < height-1 && grid[buttonx][buttony+1] == EMPTY){
			sweep(buttonx, buttony+1, undergrid, grid, width, height);
		}
	} else {
		grid[buttonx][buttony] = 7+mines;
	}
}

int **undergrid;
int started;
int mines(int **grid, SDL_Event eventbutton, int game, int t, int cellsize, int width, int height){

	if(t == 1){
		started = 0;
		srand(time(NULL));
		//Place all mines
		undergrid = (int **) calloc(width, sizeof(int *));
		for(int i = 0; i < width; i++){
			undergrid[i] = (int *) calloc(height, sizeof(int));
			for(int j = 0; j < height; j++){
				if(rand() % 5 == 0){
					undergrid[i][j] = MINE;
				} else {
					undergrid[i][j] = CLEAR;
				}
			}
		}
	}

	//Respond to user input
	if(eventbutton.type == SDL_MOUSEBUTTONUP){
		int buttonx = eventbutton.button.x / cellsize;
		int buttony = eventbutton.button.y / cellsize;
		if(eventbutton.button.button == SDL_BUTTON_LEFT && buttonx < width && buttony < height && buttonx > -1 && buttony > -1){
			if(started == 0){
				//Clear out 3x3 area around first click to ensure you have a jumping off point
				started = 1;
				undergrid[buttonx][buttony] = CLEAR;
				if(buttonx > 0){
					if(buttony > 0){
						undergrid[buttonx-1][buttony-1] = CLEAR;
					}
					if(buttony < height-1){
						undergrid[buttonx-1][buttony+1] = CLEAR;
					}
					undergrid[buttonx-1][buttony] = CLEAR;
				}
				if(buttonx < width-1){
					if(buttony > 0){
						undergrid[buttonx+1][buttony-1] = CLEAR;
					}
					if(buttony < height-1){
						undergrid[buttonx+1][buttony+1] = CLEAR;
					}
					undergrid[buttonx+1][buttony] = CLEAR;
				}
				if(buttony > 0){
					undergrid[buttonx][buttony-1] = CLEAR;
				}
				if(buttony < height-1){
					undergrid[buttonx][buttony+1] = CLEAR;
				}
			}
			if(undergrid[buttonx][buttony] == MINE){
				for(int i = 0; i < width; i++){
					for(int j = 0; j < height; j++){
						if(undergrid[i][j] == MINE){
							grid[i][j] = MINE;
						}
					}
				}
				printf("YOU LOSE!\n");
				return ARCADE;
			} else {
				sweep(buttonx, buttony, undergrid, grid, width, height);
				//Check if all mines have been found
				int winflag = 1;
				for(int i = 0; i < width; i++){
					for(int j = 0; j < height; j++){
						if((grid[i][j] == EMPTY || grid[i][j] == FLAG) && undergrid[i][j] == CLEAR){
							winflag = 0;
						}
					}
				}
				if(winflag == 1){
					printf("YOU WIN!\n");
					return ARCADE;
				}
			}
		} else if(eventbutton.button.button == SDL_BUTTON_RIGHT && buttonx < width && buttony < height && buttonx > -1 && buttony > -1){
			//Place or remove flag
			if(grid[buttonx][buttony] == EMPTY){
				grid[buttonx][buttony] = FLAG;
			} else if(grid[buttonx][buttony] == FLAG){
				grid[buttonx][buttony] = EMPTY;
			}
		}
	}

	return MINES;
}